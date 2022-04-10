use std::io::prelude::*;
use std::path::Iter;
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

    stream.read(&mut buf).unwrap();
    let request = String::from_utf8_lossy(&buf[..]);
    println!("Request: {}", request);
    let mut path = request.split("/");

    let action = path
        .next()
        .expect("you dumb ass :skull: why you sending 0 (zero) bytes");

    if action == "R" {
        let resp = "1.02";
        println!("Read instruction, sending 1.02 back");

        stream.write(resp.as_bytes()).unwrap();
        stream.flush().unwrap();
    } else if action == "U" {
        let resp = "1.02";
        stream.write(resp.as_bytes()).unwrap();
        stream.flush().unwrap();
    }
}

fn get(path: Iter) {}

fn update(path: String) {}
