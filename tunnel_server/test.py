#!/usr/bin/env python




#client
import socket
(c_host,c_port) = ("wtf.rackexp.org",10001)
c = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
c.connect((c_host,c_port))


#server
import socket
(s_host,s_port) = ("127.0.0.1",10002)
server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind((s_host,s_port))
server.listen(1)
(cs,(host,port))  = server.accept()


