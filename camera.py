import socket
import struct

SERVER_HOST = '0.0.0.0'  # Listen on all network interfaces
SERVER_PORT = 3333
BUFFER_SIZE = 1024  # 4 KB buffer size

def save_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)

def start_server():
    # Create a TCP/IP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Bind the socket to the address and port
    server_socket.bind((SERVER_HOST, SERVER_PORT))
    
    # Listen for incoming connections (1 pending connection max)
    server_socket.listen(1)
    print(f"[*] Listening on {SERVER_HOST}:{SERVER_PORT}")
    
    while True:
        # Wait for a connection
        print("[*] Waiting for a connection...")
        client_socket, client_addr = server_socket.accept()
        print(f"[*] Accepted connection from {client_addr[0]}:{client_addr[1]}")
        
        try:
            chunk = b""
            
            # Receive the file data
            received_bytes = 0
            file_data = b""
            while chunk[:4] != b"noam":
                chunk = client_socket.recv(BUFFER_SIZE)
                if not chunk:
                    break
                file_data += chunk            
            # Assume the file name sent by the client is 'file.png'
            filename = 'received_file.png'
            save_file(filename, file_data)
            
            print(f"[*] File received and saved as {filename}")
        
        except Exception as e:
            print(f"Error: {e}")
        
        finally:
            # Close the connection
            client_socket.close()
            print("[*] Connection closed\n")

if __name__ == "__main__":
    start_server()
