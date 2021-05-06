import socket
import codecs

if __name__ == '__main__':
    print("PyScript to test MT Server 1.0.1")
    userSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    userSocket.connect(("127.0.0.1", 5150))
    print(f'{201:08b}' + f'{37:032b}' + ''.join(format(ord(i), '08b') for i in "{username: \"user1\", password: \"1234\"}"))
    userSocket.send(('' + f'{201:08b}' + f'{37:032b}'.join(format(ord(i), '08b') for i in "{username: \"user1\", password: \"1234\"}")).encode())
    ans = userSocket.recv(1024).decode()
    print(ans)


    #userSocket.send(''.join(format(202, '08b')).join(format(61, '32b')).join(format(ord(i), '08b') for i in "{username: \"user1\", password: \"1234\", mail: \"usr1@gmail.com\"}").encode())
    #print(userSocket.recv(1024).decode())