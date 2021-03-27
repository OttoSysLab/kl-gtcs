import json
import socket
import select
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
            _send = "{ANS313,20200902 09:25:38,0C05,1,3.2,OK,3.700,780.0,2.17,280.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0,2.900,900.0,2.50,250.0}"
            print("Recived data = %s" %(_tcp_rev))              
            connection.send(_tcp_rev.encode('utf-8'))
            print("Send data = %s" %(_tcp_rev))
        except socket.timeout:
            # log.error("Handle tcp ui process ip %s connect time out!"%(address))
            print("Handle tcp ui process ip %s connect time out!"%(address))
        except Exception as e:
            # log.error("Handle tcp ui process err msg = %s"%(e))
            print("Handle tcp ui process err msg = %s"%(e))
        connection.close()