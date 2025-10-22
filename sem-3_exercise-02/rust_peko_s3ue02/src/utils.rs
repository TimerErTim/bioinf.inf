use rand::{distr::Alphanumeric, Rng};
use serde::Serialize;
use std::fs::{self, File};
use std::io::{self, BufRead, BufReader, BufWriter, Write};
use std::path::Path;
use std::sync::Arc;
use sysinfo::{ProcessRefreshKind, ProcessesToUpdate, RefreshKind, System};

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

#[derive(Debug, Serialize, Clone)]
pub struct SystemInfo {
    pub os: String,
    pub arch: String,
    pub kernel_version: Option<String>,
    pub long_os_version: Option<String>,
    pub host_name: Option<String>,
    pub total_memory_kb: u64,
    pub cpu_brand: String,
    pub cpu_frequency_mhz: u64,
    pub cpu_cores_logical: usize,
}

pub fn collect_system_info() -> SystemInfo {
    let mut sys = System::new_all();
    sys.refresh_all();

    let cpus = sys.cpus();
    let cpu_brand = cpus
        .get(0)
        .map(|c| c.brand().to_string())
        .unwrap_or_else(|| "unknown".to_string());
    let cpu_frequency_mhz = cpus.iter().map(|c| c.frequency()).max().unwrap_or(0);
    let cpu_cores_logical = cpus.len();

    SystemInfo {
        os: std::env::consts::OS.to_string(),
        arch: std::env::consts::ARCH.to_string(),
        kernel_version: System::kernel_version(),
        long_os_version: System::long_os_version(),
        host_name: System::host_name(),
        total_memory_kb: (sys.total_memory() / 1024) as u64,
        cpu_brand,
        cpu_frequency_mhz,
        cpu_cores_logical,
    }
}

pub struct PeakMemTracker {
    peak_used_kb: std::sync::Arc<std::sync::atomic::AtomicU64>,
    _handle: std::thread::JoinHandle<()>,
    stop_flag: std::sync::Arc<std::sync::atomic::AtomicBool>,
}

impl PeakMemTracker {
    pub fn start() -> Self {
        Self::start_with_rate(std::time::Duration::from_millis(1))
    }
    
    pub fn start_with_rate(sample_rate: std::time::Duration) -> Self {
        let peak_used_kb = std::sync::Arc::new(std::sync::atomic::AtomicU64::new(0));
        let stop_flag = std::sync::Arc::new(std::sync::atomic::AtomicBool::new(false));
        
        let peak_clone = Arc::clone(&peak_used_kb);
        let stop_clone = Arc::clone(&stop_flag);
        
        let pid = sysinfo::get_current_pid().unwrap();
        let _handle = std::thread::spawn(move || {   
            let mut sys = sysinfo::System::new_with_specifics(
                RefreshKind::nothing().with_processes(
                    ProcessRefreshKind::nothing().with_memory()
                )
            );         
            while !stop_clone.load(std::sync::atomic::Ordering::Relaxed) {
                sys.refresh_processes(ProcessesToUpdate::Some(&[pid]), true);
                let process_kb = if let Some(process) = sys.process(pid) {
                    process.memory() / 1024
                } else {
                    continue;
                };
                
                let mut peak_process_kb = peak_clone.load(std::sync::atomic::Ordering::Relaxed);
                while process_kb > peak_process_kb {
                    match peak_clone.compare_exchange_weak(
                        peak_process_kb,
                        process_kb,
                        std::sync::atomic::Ordering::Relaxed,
                        std::sync::atomic::Ordering::Relaxed
                    ) {
                        Ok(_) => break,
                        Err(actual) => peak_process_kb = actual,
                    }
                }
                
                std::thread::sleep(sample_rate);
            }
        });
        
        Self { 
            peak_used_kb, 
            _handle,
            stop_flag,
        }
    }
    
    pub fn peak_kb(&self) -> u64 { 
        (self.peak_used_kb.load(std::sync::atomic::Ordering::Relaxed)) as u64 
    }
    
    pub fn peak_mb(&self) -> u64 { 
        (self.peak_used_kb.load(std::sync::atomic::Ordering::Relaxed) / 1024) as u64 
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
        "system": collect_system_info(),
        "entries": metrics
    });
    let json_string = serde_json::to_string_pretty(&wrapper).unwrap();
    w.write_all(json_string.as_bytes())?;
    Ok(())
}





