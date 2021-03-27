import json
import socket
import select
import datetime 
from time import sleep

if __name__ == '__main__':  
    #import socket
    IP = "127.0.0.1"
    PORT = 9000
    # Initial 
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((IP,PORT))  
    sock.listen(5)
    print("TCP ui server IP = %s ,PORT = %s" %(IP,PORT))
    print("Tcp ui server start...")
    # While loop.
    while True:  
        connection,address = sock.accept()
        try:
            connection.settimeout(5)
            _tcp_rev = connection.recv(1024).decode("utf-8")
            print("Recived data = %s" %(_tcp_rev))
            _tcp_send = datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
            connection.send(_tcp_send.encode('utf-8'))
            print("Send data = %s" %(_tcp_send))
        except socket.timeout:
            print("Handle tcp ui process ip %s connect time out!"%(address))
        except Exception as e:
            print("Handle tcp ui process err msg = %s"%(e))
        connection.close()