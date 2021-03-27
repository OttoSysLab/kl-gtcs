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
                                timeout=10 )
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
    
    def read_comm_data(self):
        _ser_buf = []
        while (self.__ser.inWaiting()):
            _ser_buf.append(self.__ser.read().decode())
        return _ser_buf

    def reset_output_buffer(self):
        self.__ser.reset_output_buffer()

    # Close MCB comport.
    def close_MCB_comm(self):
        self.__ser.close()    

    def calculate_check_sum(self,pstr):
        _value = 0
        for val in pstr:
            _value += ord(val)
        return str(hex(_value))[2:].zfill(4)


def main():
    _comm = MCBComm()
    _comm_flage = _comm.open_MCB_comm(comm="/dev/ttymxc1",baudrate=115200)
    #_comm_flage = _comm.open_MCB_comm(comm="COM9",baudrate=115200)
    
    _DATA_300 = {
        "str1"             :"DATA300",                                  # Header+DATA
        "str2"             :"",                                         # yyyyMMdd HH:mm:ss
        "str3"             :"",                                         # check sum ,4 chars
        "str4"             :"001",                                      # Command_sn
        "str5"             :"6",                                        # Device type
        "str6"             :"0123456789______________________",         # Tool SN
        "str7"             :"N333456789______________________",         # Device SN
        "str8"             :"01",                                       # Job ID
        "str9"             :"01",                                       # Sequence ID
        "str10"            :"001",                                      # Program ID
        "str11"            :"001",                                      # Step ID
        "str12"            :"0",                                        # Direction
        "str13"            :"4",                                        # Torque unit
        "str14"            :"01",                                       # INC/DEC
        "str15"            :"01",                                       # Last_screw_count
        "str16"            :"01",                                       # Max_screw_count
        "str17"            :"0000.0000",                                # Fastening time
        "str18"            :"0000.0000",                                # Torque
        "str19"            :"36000.0",                                  # Angle
        "str20"            :"0000.0000",                                # Max Torque
        "str21"            :"0000.0000",                                # Revolutions
        "str22"            :"Sequence.OK",                                       # Status
        "str23"            :"000000000",                                # Inputio
        "str24"            :"0000000000",                               # Outputio
        "str25"            :"0000000000000000000000",                   # Error Masseage
        "str26"            :"0000000001",                               # Tool Count
        "str27"            :"00001",                                    # RPM
        #"str28"            :"\n",
        #"str29"            :"\r",
    }

    if _comm_flage ==True:
        print("Start COMM!")
        _cmdsn = 0
        while True:
            if _cmdsn<255:
                _cmdsn +=1
            else:
                _cmdsn = 1
            _DATA_300["str2"] = datetime.datetime.now().strftime("%Y%m%d %H:%M:%S")
            _DATA_300["str4"] = str(_cmdsn).zfill(3)
            _DATA_300_list = list(_DATA_300.values())[3:]
            _str_1 = ""
            for val in _DATA_300_list:
                _str_1 += ","+str(val)
            _DATA_300["str3"] = _comm.calculate_check_sum(pstr=_str_1).upper()
            _str_2 =""
            _DATA_300_list = list(_DATA_300.values())
            for idx,val in enumerate(_DATA_300_list):
                if idx ==0:
                    _str_2+=val
                else:
                    _str_2+=","+val
            _str_send = "{"+_str_2+"}"+"\n"+"\r"
            print(_DATA_300["str3"])
            _comm.write_to_MCB(data=_str_send.encode('utf-8'))
            _str_read = _comm.read_comm_data()
            print(_str_read)
            sleep(1)

if __name__ == "__main__":
    main()
        