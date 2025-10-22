use std::io;
use std::marker::PhantomData;

pub trait MergeReader<T>: Sized {
    type Writer: MergeWriter<T, Reader = Self>;
    fn get(&mut self) -> io::Result<T>;
    fn advance(&mut self) -> io::Result<bool>;
    fn is_exhausted(&mut self) -> io::Result<bool>;
    fn into_writer(self) -> io::Result<Self::Writer>;
}

pub trait MergeWriter<T>: Sized {
    type Reader: MergeReader<T, Writer = Self>;
    fn append(&mut self, value: T) -> io::Result<()>;
    fn into_reader(self) -> io::Result<Self::Reader>;
}

pub struct MergeSorter<T> { _phantom: PhantomData<T> }

impl<T: Ord + Clone> MergeSorter<T> {
    pub fn new() -> Self { Self { _phantom: PhantomData } }

    pub fn merge_step<L, R, W>(
        &self,
        reader_l: &mut L,
        reader_r: &mut R,
        writer: &mut W,
        chunk_size_per_reader: usize,
    ) -> io::Result<bool>
    where
        L: MergeReader<T>,
        R: MergeReader<T>,
        W: MergeWriter<T>,
    {
        let mut l_count = 0usize;
        let mut r_count = 0usize;
        let mut l_exhausted = reader_l.is_exhausted()?;
        let mut r_exhausted = reader_r.is_exhausted()?;

        while l_count < chunk_size_per_reader && r_count < chunk_size_per_reader && !l_exhausted && !r_exhausted {
            let left_val = reader_l.get()?;
            let right_val = reader_r.get()?;
            if left_val <= right_val {
                writer.append(left_val)?;
                l_exhausted = !reader_l.advance()?;
                l_count += 1;
            } else {
                writer.append(right_val)?;
                r_exhausted = !reader_r.advance()?;
                r_count += 1;
            }
        }

        while l_count < chunk_size_per_reader && !l_exhausted {
            let v = reader_l.get()?;
            writer.append(v)?;
            l_exhausted = !reader_l.advance()?;
            l_count += 1;
        }

        while r_count < chunk_size_per_reader && !r_exhausted {
            let v = reader_r.get()?;
            writer.append(v)?;
            r_exhausted = !reader_r.advance()?;
            r_count += 1;
        }

        Ok(!l_exhausted || !r_exhausted)
    }

    pub fn merge<L, R>(
        &self,
        sorted_l: &mut L,
        sorted_r: &mut R,
        writer_l: &mut L::Writer,
        writer_r: &mut R::Writer,
        chunk_size: usize,
    ) -> io::Result<()>
    where
        L: MergeReader<T>,
        R: MergeReader<T>,
    {
        let mut write_left = true;
        loop {
            if write_left {
                if !self.merge_step(sorted_l, sorted_r, writer_l, chunk_size)? { break; }
            } else {
                if !self.merge_step(sorted_l, sorted_r, writer_r, chunk_size)? { break; }
            }
            write_left = !write_left;
        }
        Ok(())
    }

    pub fn sort<L, R>(
        &self,
        mut reader_l: L,
        mut reader_r: R,
        mut writer_l: L::Writer,
        mut writer_r: R::Writer,
        total_size: usize,
    ) -> io::Result<(L, R, L::Writer, R::Writer)>
    where
        L: MergeReader<T>,
        R: MergeReader<T>,
    {
        let mut chunk_size = 1usize;
        loop {
            self.merge(&mut reader_l, &mut reader_r, &mut writer_l, &mut writer_r, chunk_size)?;
            chunk_size *= 2;
            if chunk_size > total_size / 2 { break; }
            let new_reader_l = writer_l.into_reader()?;
            let new_reader_r = writer_r.into_reader()?;
            let new_writer_l = reader_l.into_writer()?;
            let new_writer_r = reader_r.into_writer()?;
            reader_l = new_reader_l;
            reader_r = new_reader_r;
            writer_l = new_writer_l;
            writer_r = new_writer_r;
        }
        Ok((reader_l, reader_r, writer_l, writer_r))
    }

    pub fn split<S>(
        &self,
        source: &mut S,
        left: &mut S::Writer,
        right: &mut S::Writer,
    ) -> io::Result<usize>
    where
        S: MergeReader<T>,
    {
        let mut count = 0usize;
        let mut to_left = true;
        while !source.is_exhausted()? {
            let v = source.get()?;
            if to_left { left.append(v)?; } else { right.append(v)?; }
            to_left = !to_left;
            source.advance()?; // move to next
            count += 1;
        }
        Ok(count)
    }

    pub fn complete_sort<S>(
        &self,
        mut unsorted_source: S,
        mut buffer1: S::Writer,
        mut buffer2: S::Writer,
        buffer3: S::Writer,
        buffer4: S::Writer,
    ) -> io::Result<S>
    where
        S: MergeReader<T>,
    {
        let total_size = self.split(&mut unsorted_source, &mut buffer1, &mut buffer2)?;
        let reader_l = buffer1.into_reader()?;
        let reader_r = buffer2.into_reader()?;
        let (mut reader_l, mut reader_r, _, _) = self.sort(reader_l, reader_r, buffer3, buffer4, total_size)?;
        let mut sink = unsorted_source.into_writer()?;
        self.merge_step(&mut reader_l, &mut reader_r, &mut sink, total_size)?;
        sink.into_reader()
    }
}


