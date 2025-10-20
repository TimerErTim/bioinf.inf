use std::io;

use crate::merge_sort::{MergeReader, MergeWriter};

pub struct InMemoryReader<T: Clone> {
    data: Vec<T>,
    cursor: usize,
}

impl<T: Clone> InMemoryReader<T> {
    pub fn from_vec(data: Vec<T>) -> Self { Self { data, cursor: 0 } }
}

impl<T: Clone> MergeReader<T> for InMemoryReader<T> {
    type Writer = InMemoryWriter<T>;

    fn get(&mut self) -> io::Result<T> {
        if self.cursor >= self.data.len() { return Err(io::Error::new(io::ErrorKind::UnexpectedEof, "no more")); }
        Ok(self.data[self.cursor].clone())
    }
    fn advance(&mut self) -> io::Result<bool> {
        if self.cursor >= self.data.len() { return Ok(false); }
        self.cursor += 1;
        Ok(self.cursor < self.data.len())
    }
    fn is_exhausted(&mut self) -> io::Result<bool> { Ok(self.cursor >= self.data.len()) }
    fn into_writer(self) -> io::Result<Self::Writer> {
        let mut v = self.data;
        v.clear();
        Ok(InMemoryWriter { data: v })
    }
}

pub struct InMemoryWriter<T: Clone> {
    pub(crate) data: Vec<T>,
}

impl<T: Clone> InMemoryWriter<T> {
    pub fn new() -> Self { Self { data: Vec::new() } }
}

impl<T: Clone> MergeWriter<T> for InMemoryWriter<T> {
    type Reader = InMemoryReader<T>;
    fn append(&mut self, value: T) -> io::Result<()> { self.data.push(value); Ok(()) }
    fn into_reader(self) -> io::Result<Self::Reader> { Ok(InMemoryReader { data: self.data, cursor: 0 }) }
}


