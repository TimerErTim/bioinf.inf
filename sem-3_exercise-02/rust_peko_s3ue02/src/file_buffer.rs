use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, BufReader, BufWriter, Read, Write};
use std::path::PathBuf;

use crate::merge_sort::{MergeReader, MergeWriter};

pub struct FileMergeReader {
    path: PathBuf,
    reader: BufReader<File>,
    buffer: Option<String>,
}

impl FileMergeReader {
    pub fn open(path: impl Into<PathBuf>) -> io::Result<Self> {
        let path = path.into();
        let file = File::open(&path)?;
        Ok(Self { path, reader: BufReader::new(file), buffer: None })
    }
    fn ensure_buffer(&mut self) -> io::Result<bool> {
        if self.buffer.is_some() { return Ok(true); }
        let mut s = String::new();
        s.clear();
        // read next whitespace-separated token
        let mut bytes = Vec::new();
        loop {
            let mut byte = [0u8;1];
            let read = self.reader.read(&mut byte)?;
            if read == 0 { break; }
            let b = byte[0];
            if b.is_ascii_whitespace() {
                if !bytes.is_empty() { break; } else { continue; }
            } else {
                bytes.push(b);
            }
        }
        if bytes.is_empty() { return Ok(false); }
        s = String::from_utf8(bytes).map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
        self.buffer = Some(s);
        Ok(true)
    }
}

impl MergeReader<String> for FileMergeReader {
    type Writer = FileMergeWriter;
    fn get(&mut self) -> io::Result<String> {
        if !self.ensure_buffer()? { return Err(io::Error::new(io::ErrorKind::UnexpectedEof, "no token")); }
        Ok(self.buffer.as_ref().unwrap().clone())
    }
    fn advance(&mut self) -> io::Result<bool> {
        if !self.ensure_buffer()? { return Ok(false); }
        self.buffer = None;
        self.ensure_buffer()
    }
    fn is_exhausted(&mut self) -> io::Result<bool> { Ok(!self.ensure_buffer()?) }
    fn into_writer(self) -> io::Result<Self::Writer> { FileMergeWriter::create(&self.path) }
}

pub struct FileMergeWriter {
    path: PathBuf,
    writer: BufWriter<File>,
    first: bool,
}

impl FileMergeWriter {
    pub fn create(path: impl Into<PathBuf>) -> io::Result<Self> {
        let path = path.into();
        let file = OpenOptions::new().create(true).write(true).truncate(true).open(&path)?;
        Ok(Self { path, writer: BufWriter::new(file), first: true })
    }
}

impl MergeWriter<String> for FileMergeWriter {
    type Reader = FileMergeReader;
    fn append(&mut self, value: String) -> io::Result<()> {
        if !self.first { self.writer.write_all(b" ")?; }
        self.first = false;
        self.writer.write_all(value.as_bytes())
    }
    fn into_reader(self) -> io::Result<Self::Reader> {
        self.writer.into_inner()?.sync_all()?;
        FileMergeReader::open(&self.path)
    }
}


