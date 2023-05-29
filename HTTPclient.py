import socket, sys

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 8081
URL = sys.argv[1] if len(sys.argv) == 2 else Exception("Definire l'url da richiedere")
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    request = f"""GET {URL} HTTP/1.1\r\nHost: localhost\r\n\r\n"""
    s.sendall(request.encode('utf-8'))
    print("Risposta:\n\n{}".format(s.recv(1024).decode('utf-8')))
    while True:
    	data = s.recv(1024).decode('utf-8')
    	print(data)
