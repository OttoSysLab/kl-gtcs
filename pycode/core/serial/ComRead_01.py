#!/usr/bin/python
# -*- coding: UTF-8 -*-
from serial import Serial 
from time import sleep

import serial  # 引用pySerial模組

def main():
    COM_PORT = 'COM3'    # 指定通訊埠名稱
    BAUD_RATES = 115200    # 設定傳輸速率
    ser = serial.Serial(port = COM_PORT, 
                        baudrate = BAUD_RATES,
                        parity = 'N',
                        stopbits = 1,
                        bytesize =8,
                        timeout=0.1 )

    try:
        while True:
            while ser.in_waiting:          # 若收到序列資料…
                data_raw = ser.readline()  # 讀取一行
                data = data_raw.decode()   # 用預設的UTF-8解碼
                # print('received data raw：', data_raw)
                # print('received data：', data)
                print(data)
            pass
 
    except KeyboardInterrupt:
        ser.close()    # 清除序列通訊物件
        print('Good Bye！')

if __name__ == "__main__":
    main()
        