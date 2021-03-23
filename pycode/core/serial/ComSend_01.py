#!/usr/bin/python
# -*- coding: UTF-8 -*-
from serial import Serial 
from time import sleep
import datetime

#region MCB commport.
class MCBComm:
    # Initial object        
    def __init__(self):
        self.__ser = ''
    
    # Initial MCB comport.    
    def open_MCB_comm(self,comm,baudrate):
        _comport_ok = False
        # Config serial port.        
        try:
            self.__ser = Serial(port = comm, 
                                baudrate = baudrate,
                                parity = 'N',
                                stopbits = 1,
                                bytesize =8,
                                timeout=0.1 )
            if self.__ser.isOpen():
                _comport_ok=True
        except IOError as e:
            # self.__ser.close()
            print(e)
        except Exception as e:
            print(e)
        return _comport_ok

    # write dat to smcb.
    def write_to_MCB(self,data):
        try:
            self.__ser.write(data)
            self.__ser.flush()
        except Exception as e:print(e)

    # read data from mcb. 
    def read_MCB_data(self):
        _ser_buf = []
        while (self.__ser.inWaiting()):
            _ser_buf.append(int.from_bytes(self.__ser.read(),byteorder="big",signed=False))
        return _ser_buf


    def reset_output_buffer(self):
        self.__ser.reset_output_buffer()

    # Close MCB comport.
    def close_MCB_comm(self):
        self.__ser.close()    

def main():
    _comm = MCBComm()
    _comm_flage = _comm.open_MCB_comm(comm="/dev/ttymxc2",baudrate=115200)
    if _comm_flage ==True:
        print("Start COMM!")
        while True:
            _time  = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
            print(_time)
            _comm.write_to_MCB(data=_time.encode('utf-8'))
            sleep(0.1)

if __name__ == "__main__":
    main()
        