import json
import socket
from time import sleep

def test_socket_client():
    # sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock.connect(('127.0.0.1', 9000))
    _cnt =0
    # IP =  "127.0.0.1" # "192.168.0.202"
    # IP =  "192.168.0.237"
    IP =  "192.168.0.207"
    PORT = 9999
    while(True):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((IP, PORT))
            sock.send(('Socket server= %s port = %s'%(IP,PORT)).encode('utf-8'))
            print("connect cnt = %d"%(_cnt))
        except Exception as e:
            print(e)
        finally:
            sleep(0.1)
    sock.close()

if __name__ == '__main__':
    test_socket_client()