use rand::{distr::Alphanumeric, Rng};
use std::fs::File;
use std::io::{self, BufWriter, Write};

pub fn random_string(len: usize) -> String {
    rand::rng()
        .sample_iter(&Alphanumeric)
        .take(len)
        .map(char::from)
        .collect()
}

pub fn fill_randomly(path: &str, n: usize, len: usize) -> io::Result<()> {
    let file = File::create(path)?;
    let mut w = BufWriter::new(file);
    for i in 0..n {
        if i > 0 { w.write_all(b" ")?; }
        let s = random_string(len);
        w.write_all(s.as_bytes())?;
    }
    Ok(())
}





