mod merge_sort;
mod in_memory;
mod file_buffer;
mod utils;

use std::fs; 
use std::io;
use std::time::{Duration, Instant};

use merge_sort::{MergeSorter, MergeReader, MergeWriter};

fn bench_in_memory(n: usize, len: usize) -> io::Result<Duration> {
    let data: Vec<String> = (0..n).map(|_| utils::random_string(len)).collect();
    let reader = in_memory::InMemoryReader::from_vec(data);
    let b1 = in_memory::InMemoryWriter::<String>::new();
    let b2 = in_memory::InMemoryWriter::<String>::new();
    let b3 = in_memory::InMemoryWriter::<String>::new();
    let b4 = in_memory::InMemoryWriter::<String>::new();
    let sorter = MergeSorter::<String>::new();

    let start = Instant::now();
    let mut r = sorter.complete_sort(reader, b1, b2, b3, b4)?;
    let elapsed = start.elapsed();

    // validate sorted
    let mut prev: Option<String> = None;
    while !r.is_exhausted()? {
        let cur = r.get()?;
        if let Some(p) = prev.as_ref() { assert!(p <= &cur); }
        prev = Some(cur);
        r.advance()?;
    }

    Ok(elapsed)
}

fn bench_on_disk(n: usize, len: usize) -> io::Result<Duration> {
    let src = "tmp/bench_src.txt";
    let b1 = "tmp/bench_b1.txt";
    let b2 = "tmp/bench_b2.txt";
    let b3 = "tmp/bench_b3.txt";
    let b4 = "tmp/bench_b4.txt";
    let _ = fs::remove_file(src);
    let _ = fs::remove_file(b1);
    let _ = fs::remove_file(b2);
    let _ = fs::remove_file(b3);
    let _ = fs::remove_file(b4);
    utils::fill_randomly(src, n, len)?;

    let reader = file_buffer::FileMergeReader::open(src)?;
    let buf1 = file_buffer::FileMergeWriter::create(b1)?;
    let buf2 = file_buffer::FileMergeWriter::create(b2)?;
    let buf3 = file_buffer::FileMergeWriter::create(b3)?;
    let buf4 = file_buffer::FileMergeWriter::create(b4)?;

    let sorter = MergeSorter::<String>::new();
    let start = Instant::now();
    let mut r = sorter.complete_sort(reader, buf1, buf2, buf3, buf4)?;
    let elapsed = start.elapsed();

    // validate
    let mut prev: Option<String> = None;
    while !r.is_exhausted()? {
        let cur = r.get()?;
        if let Some(p) = prev.as_ref() { assert!(p <= &cur); }
        prev = Some(cur);
        r.advance()?;
    }
    Ok(elapsed)
}

fn main() -> io::Result<()> {
    let sizes = [1000, 20000, 500000, 1000000];
    let string_lens = [20, 50, 100];

    for &len in &string_lens {
        for &n in &sizes {
            let mem = bench_in_memory(n, len)?;
            println!("in-memory len={} n={} -> {:?}", len, n, mem);
        }
    }

    for &len in &string_lens {
        for &n in &sizes {
            let disk = bench_on_disk(n, len)?;
            println!("on-disk   len={} n={} -> {:?}", len, n, disk);
        }
    }
    
    Ok(())
}
