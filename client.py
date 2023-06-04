import socket, sys

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 8081
IP = sys.argv[1] if len(sys.argv) == 2 else Exception("Definire l'url da richiedere")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.connect((HOST, PORT))
	s.sendall(IP.encode('utf-8'))
	data = s.recv(1024).decode('utf-8')
	print(data)
