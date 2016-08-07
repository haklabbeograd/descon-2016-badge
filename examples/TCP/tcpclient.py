from socket import socket, AF_INET, SOCK_STREAM

connection = socket(AF_INET, SOCK_STREAM)

connection.connect(("127.0.0.1", 9001))

while True:
    data = raw_input("Input: ")
    if(data == "??q"):
        exit(0)

    connection.send(data)

