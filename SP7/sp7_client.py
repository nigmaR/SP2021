import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.0.37', 15000))
s.sendall("Hi Niko, It's ROman, lets go bowling?!".encode("utf-8"))
data = s.recv(1024)
s.close()
print ("Received MSG:", data.decode("utf-8"))