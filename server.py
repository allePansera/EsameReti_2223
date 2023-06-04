#!/usr/bin/env python3

import socket
import os
import sys
import time
import re
import traceback
from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE,SIG_DFL)

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 8081  # Port to listen on (non-privileged ports are > 1023)


def find_class(ip_octet):
	"""
	La funzione valuta la classe fornito il primo ottetto per un IP
	"""
	if(ip_octet >= 0 and ip_octet <= 127):
		return "A"
	elif(ip_octet >=128 and ip_octet <= 191):
		return "B"
	elif(ip_octet >= 192 and ip_octet <= 223):
		return "C"
	elif(ip_octet >= 224 and ip_octet <= 239):
		return "D"
	else:
		return "E"

def find_net_broadcast(ip_class, ip_array):
	"""
	La funzione calcola la NET ID  e il Broadcast per l'array fornito sulla base della classe ricevuta
	"""
	net_id, broad = "", ""
	try:
		if ip_class == "A":
			return ".".join([str(i) for i in ip_array[:1]])+".0.0.0", ".".join([str(i) for i in ip_array[:1]])+".255.255.255"
		elif ip_class == "B":
			return ".".join([ str(i) for i in ip_array[:2]])+".0.0", ".".join([str(i) for i in ip_array[:2]])+".255.255"
		else:
			return ".".join([ str(i) for i in ip_array[:3]])+".0", ".".join([str(i) for i in ip_array[:3]])+".255"

	except Exception as e:
		traceback.print_exc()
		return "", ""

def process_ip(ip_rcv):
	"""
	La funzione ritorna la classe per l'ip e se prevista anche NETid e Indirizzo Boradcast.
	E' attesa una stringa, verrà eseguito lo split per il processing.
	"""
	response = ""
	try:
		octet_int_array = [int(octet) for octet in ip_rcv.split(".")]
		ip_class = find_class(octet_int_array[0])
		response += ip_class
		if ip_class in ["A", "B", "C"]:
			return response+f' {str(find_net_broadcast(ip_class, octet_int_array))}'
		else:
			return response
	except Exception as e:
		traceback.print_exc()
		return "INTERNAL ERROR !!"

def check_ip(ip_rcv):
	"""
	La funzione ritorna un valore booleano che identifica la correttezza della stringa ricevuta
	"""
	pattern_match = re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip_rcv)
	return True if pattern_match else False


def parse_request(conn):
	while True:
		ip_rcv = conn.recv(1024).decode('utf-8')
		if check_ip(ip_rcv):
			conn.sendall(process_ip(ip_rcv).encode('utf-8'))
		else:
			conn.sendall("ERRORE".encode())



# main
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.bind((HOST, PORT))
	s.listen()
	while True:
		conn, addr = s.accept()
		child_pid = os.fork()
		if child_pid == 0:
			parse_request(conn)
			sys.exit()
		else:
			conn.close()

