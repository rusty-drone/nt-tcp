import socket


def main(host, port):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((host, port))

    msg = client.recv(256)
    print(msg.decode("utf-8"))



if __name__ == "__main__":
    host = "0.0.0.0"
    port = 1234
    main(host, port)
