import json
import socket
from time import sleep

def test_socket_client():
    # sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock.connect(('127.0.0.1', 9000))
    _cnt =0
    # IP =  "127.0.0.1" # "192.168.0.202"
    IP =  "192.168.0.207"
    PORT = 9002
    while(True):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # sock.connect(('127.0.0.1', 9999))
            sock.connect((IP, PORT))
            sock.send(('Socket server= %s port = %s'%(IP,PORT)).encode('utf-8'))
            # str = sock.recv(1024)
            # print(str.decode("utf-8"))
            # _cnt+=1
            print("connect cnt = %d"%(_cnt))
            # sock.close()
        except Exception as e:
            print(e)
            _cnt = 0
        finally:
            sock.close()
        sleep(0.1)

if __name__ == '__main__':
    test_socket_client()