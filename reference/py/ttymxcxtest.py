#!/usr/bin/python
# -*- coding: UTF-8 -*-
from serial import Serial 
from time import sleep

import serial  # 引用pySerial模組

# 
def main():
    _BAUD_RATES = 115200         # 設定傳輸速率
    # Initial ttymxc1 
    _COM_1 = '/dev/ttymxc1'     
    _ser_1 = serial.Serial(port = _COM_1, 
                        baudrate = _BAUD_RATES,
                        parity = 'N',
                        stopbits = 1,
                        bytesize =8,
                        timeout=0.1 )
    # Initial ttymxc2           
    _COM_2 = '/dev/ttymxc2'     
    _ser_2 = serial.Serial(port = _COM_2, 
                        baudrate = _BAUD_RATES,
                        parity = 'N',
                        stopbits = 1,
                        bytesize =8,
                        timeout=0.1 )
    # Initial ttymxc2           
    _COM_3 = '/dev/ttymxc3'     
    _ser_3 = serial.Serial(port = _COM_3, 
                        baudrate = _BAUD_RATES,
                        parity = 'N',
                        stopbits = 1,
                        bytesize =8,
                        timeout=0.1 )

    _ser_list = [_ser_1,_ser_2,_ser_3]
    _msg_list = ["ttymxc1\n","ttymxc2\n","ttymxc3\n"]
    _idx = 0
    while True:
        if _idx <3:
            print("Test Com = %s"%(_msg_list[_idx]))
            _ser_list[_idx].write(_msg_list[_idx].encode('utf-8'))
            _idx+=1
        else:
            _idx = 0
        sleep(0.01)

if __name__ == "__main__":
    main()
    