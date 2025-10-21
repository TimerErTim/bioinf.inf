use rand::{distr::Alphanumeric, Rng};
use serde::Serialize;
use std::fs::{self, File};
use std::io::{self, BufWriter, Write};
use std::path::Path;
use sysinfo::{MemoryRefreshKind, RefreshKind, System};

pub fn random_string(len: usize) -> String {
    rand::rng()
        .sample_iter(&Alphanumeric)
        .take(len)
        .map(char::from)
        .collect()
}

pub fn fill_randomly<P: AsRef<Path>>(path: P, n: usize, len: usize) -> io::Result<()> {
    let file = File::create(path)?;
    let mut w = BufWriter::new(file);
    for i in 0..n {
        if i > 0 { w.write_all(b" ")?; }
        let s = random_string(len);
        w.write_all(s.as_bytes())?;
    }
    Ok(())
}

#[derive(Debug, Serialize, Clone)]
pub struct RunMetrics<'a> {
    pub mode: &'a str,
    pub n: usize,
    pub len: usize,
    pub elapsed_ms: u128,
    pub peak_mem_kb: u64,
}

pub struct PeakMemTracker {
    peak_used_bytes: std::sync::Arc<std::sync::atomic::AtomicU64>,
    _handle: std::thread::JoinHandle<()>,
    stop_flag: std::sync::Arc<std::sync::atomic::AtomicBool>,
}

impl PeakMemTracker {
    pub fn start() -> Self {
        Self::start_with_rate(std::time::Duration::from_millis(1))
    }
    
    pub fn start_with_rate(sample_rate: std::time::Duration) -> Self {
        let peak_used_bytes = std::sync::Arc::new(std::sync::atomic::AtomicU64::new(0));
        let stop_flag = std::sync::Arc::new(std::sync::atomic::AtomicBool::new(false));
        
        let peak_clone = peak_used_bytes.clone();
        let stop_clone = stop_flag.clone();
        
        let handle = std::thread::spawn(move || {
            let mut sys = System::new_with_specifics(RefreshKind::nothing().with_memory(MemoryRefreshKind::everything().without_swap()));
            
            while !stop_clone.load(std::sync::atomic::Ordering::Relaxed) {
                sys.refresh_memory_specifics(MemoryRefreshKind::everything().without_swap());
                let used = sys.used_memory();
                
                let mut current_peak = peak_clone.load(std::sync::atomic::Ordering::Relaxed);
                while used > current_peak {
                    match peak_clone.compare_exchange_weak(
                        current_peak,
                        used,
                        std::sync::atomic::Ordering::Relaxed,
                        std::sync::atomic::Ordering::Relaxed
                    ) {
                        Ok(_) => break,
                        Err(actual) => current_peak = actual,
                    }
                }
                
                std::thread::sleep(sample_rate);
            }
        });
        
        Self { 
            peak_used_bytes, 
            _handle: handle,
            stop_flag,
        }
    }
    
    pub fn peak_kb(&self) -> u64 { 
        (self.peak_used_bytes.load(std::sync::atomic::Ordering::Relaxed) / 1024) as u64 
    }
    
    pub fn peak_mb(&self) -> u64 { 
        (self.peak_used_bytes.load(std::sync::atomic::Ordering::Relaxed) / (1024 * 1024)) as u64 
    }
}

impl Drop for PeakMemTracker {
    fn drop(&mut self) {
        self.stop_flag.store(true, std::sync::atomic::Ordering::Relaxed);
    }
}

pub fn write_metrics<P: AsRef<Path>>(path: P, metrics: &[RunMetrics<'_>]) -> io::Result<()> {
    if let Some(parent) = path.as_ref().parent() { fs::create_dir_all(parent)?; }
    let file = File::create(path)?;
    let mut w = BufWriter::new(file);
    let wrapper = serde_json::json!({
        "entries": metrics
    });
    let json_string = serde_json::to_string_pretty(&wrapper).unwrap();
    w.write_all(json_string.as_bytes())?;
    Ok(())
}





