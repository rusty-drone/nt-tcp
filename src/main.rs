use std::io::prelude::Read;
use std::str::Split;
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
    let mut path = request.split("/").into_iter();

    let action = path.next().expect("didn't send action");

    match action {
        "R" => read(path),
        "U" => update(path),
        _ => println!("not an action!"),
    }
}

fn read(path: Split<&str>) {
    // println!("{}", path.as_str());
    println!("Read Action");
    path.for_each(|path| println!("{}", path));
}

fn update(path: Split<&str>) {
    println!("Update Action");
    path.for_each(|path| println!("{}", path));
}
