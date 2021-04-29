import socket

if __name__ == '__main__':
    print("PyScript to test MT Server 1.0.1")
    userSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    userSocket.connect(("127.0.0.1", 5150))
    userSocket.send("hello".encode())
    print(userSocket.recv(1024).decode())