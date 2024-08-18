import socket

# Server setup
host = '127.0.0.1'
port = 4444

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(1)

print(f"Listening on {host}:{port}")

# Accept the connection
client_socket, addr = server_socket.accept()
print(f"Got a connection from {addr}")

try:
    while True:
        # Send the 'dir' command
        command = input(">> ") + "\n"
        client_socket.send(command.encode('utf-8'))

        # Receive the output
        data = client_socket.recv(4096)  # Adjust buffer size as needed
        if not data:
            break
        print(data.decode('utf-8', errors='replace')[:-1])

finally:
    client_socket.close()
    server_socket.close()
