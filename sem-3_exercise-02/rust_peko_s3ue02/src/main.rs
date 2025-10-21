mod merge_sort;
mod in_memory;
mod file_buffer;
mod utils;

use std::fs; 
use std::io;
use std::time::{Duration, Instant};
use std::path::PathBuf;

use merge_sort::{MergeSorter, MergeReader, MergeWriter};
use utils::{PeakMemTracker, RunMetrics, write_metrics};
use tempfile::tempdir;

fn bench_in_memory(n: usize, len: usize) -> io::Result<(Duration, u64)> {
    let data: Vec<String> = (0..n).map(|_| utils::random_string(len)).collect();
    let reader = in_memory::InMemoryReader::from_vec(data);
    let b1 = in_memory::InMemoryWriter::<String>::new();
    let b2 = in_memory::InMemoryWriter::<String>::new();
    let b3 = in_memory::InMemoryWriter::<String>::new();
    let b4 = in_memory::InMemoryWriter::<String>::new();
    let sorter = MergeSorter::<String>::new();

    let mut tracker = PeakMemTracker::start();
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

    Ok((elapsed, tracker.peak_mb()))
}

fn bench_on_disk(n: usize, len: usize) -> io::Result<(Duration, u64)> {
    let tmp = tempdir()?; // system-native temp dir, auto-cleanup
    let base: PathBuf = tmp.path().to_path_buf();
    let src = base.join("bench_src.txt");
    let b1 = base.join("bench_b1.txt");
    let b2 = base.join("bench_b2.txt");
    let b3 = base.join("bench_b3.txt");
    let b4 = base.join("bench_b4.txt");
    let _ = fs::remove_file(&src);
    let _ = fs::remove_file(&b1);
    let _ = fs::remove_file(&b2);
    let _ = fs::remove_file(&b3);
    let _ = fs::remove_file(&b4);
    utils::fill_randomly(&src, n, len)?;

    let reader = file_buffer::FileMergeReader::open(&src)?;
    let buf1 = file_buffer::FileMergeWriter::create(&b1)?;
    let buf2 = file_buffer::FileMergeWriter::create(&b2)?;
    let buf3 = file_buffer::FileMergeWriter::create(&b3)?;
    let buf4 = file_buffer::FileMergeWriter::create(&b4)?;

    let sorter = MergeSorter::<String>::new();
    let mut tracker = PeakMemTracker::start();
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
    Ok((elapsed, tracker.peak_mb()))
}

fn main() -> io::Result<()> {
    let sizes = [1000, 20000, 500000, 1000000];
    let string_lens = [20, 50, 100];

    let mut results: Vec<RunMetrics> = Vec::new();
    for &len in &string_lens {
        for &n in &sizes {
            let (dur, peak_kb) = bench_in_memory(n, len)?;
            println!("in-memory len={} n={} -> {:?}, peak={}KB", len, n, dur, peak_kb);
            results.push(RunMetrics { mode: "in_memory", n, len, elapsed_ms: dur.as_millis(), peak_mem_kb: peak_kb });
        }
    }

    for &len in &string_lens {
        for &n in &sizes {
            let (dur, peak_kb) = bench_on_disk(n, len)?;
            println!("on-disk   len={} n={} -> {:?}, peak={}KB", len, n, dur, peak_kb);
            results.push(RunMetrics { mode: "on_disk", n, len, elapsed_ms: dur.as_millis(), peak_mem_kb: peak_kb });
        }
    }
    write_metrics("out/results.json", &results)?;
    
    Ok(())
}
