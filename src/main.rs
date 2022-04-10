use std::io::prelude::*;
use std::{net::TcpListener, net::TcpStream};

fn main() {
    init(1234);
}

fn init(port: i32) {
    let port = port.to_string();
    let addr = format!("{}:{}", "0.0.0.0", port);

    println!("Opening TcpListener on {}...", addr);
    let listener = TcpListener::bind(addr).unwrap();
    println!("opened TcpListener!");

    for stream in listener.incoming() {
        let stream = stream.unwrap();
        handler(stream)
    }
}

fn handler(mut stream: TcpStream) {
    let mut buf = [0; 256];

    // stream.read(&mut buf).unwrap();
    // println!("requests: {}", String::from_utf8_lossy(&buf[..]));

    let content = "hello world";

    stream.write(content.as_bytes()).unwrap();
    stream.flush().unwrap();
}
