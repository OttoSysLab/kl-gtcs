import json
import socket
import select
from time import sleep

if __name__ == '__main__':  
    #import socket
    # IP = "127.0.0.1"
    IP = "192.168.0.183"
    PORT = 9000

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #sock.bind((IP, PORT))  
    sock.setblocking(False)
    sock.listen()
    inputs = [sock, ]

    print("server ip = %s ,port = %s" %(IP,PORT))
    print("server start...")
    
    while True:  
        r_list, w_list, e_list = select.select(inputs, [], [], 1)
        for event in r_list:
            if event == sock:
                # print("新的客户端连接")
                new_sock, addr = event.accept()
                inputs.append(new_sock)
            else:
                data = event.recv(1024)
                if data:
                    # print("接收到客户端信息")
                    _msg = data.decode("utf8")
                    print(_msg)
                    # event.send(_msg.encode("utf8"))
                else:
                    # print("客户端断开连接")
                    inputs.remove(event)