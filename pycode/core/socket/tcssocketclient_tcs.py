#!/usr/bin/python
# -*- coding: UTF-8 -*-
from serial import Serial 
from time import sleep
import datetime
import socket

#region MCB commport.
class AmsProtocol:
    # Initial object.        
    def __init__(self):
        pass
    
    def calculate_check_sum(self,pstr):
        _value = 0
        for val in pstr:
            _value += ord(val)
        return str(hex(_value))[2:].zfill(4)

def main():    
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
        "str22"            :"Sequence.OK",                              # Status
        "str23"            :"000000000",                                # Inputio
        "str24"            :"0000000000",                               # Outputio
        "str25"            :"0000000000000000000000",                   # Error Masseage
        "str26"            :"0000000001",                               # Tool Count
        "str27"            :"00001",                                    # RPM
        #"str28"            :"\n",
        #"str29"            :"\r",
    }
    _ams = AmsProtocol()
    # IP =   "127.0.0.1" 
    IP =   "192.168.0.237"
    PORT = 9002
    _cmdsn = 1
    # sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock.settimeout(5)
    # sock.connect((IP, PORT))
    while True:
        print("Start TCP Socket Client!")
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(5)
            sock.connect((IP, PORT))
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
            _DATA_300["str3"] = _ams.calculate_check_sum(pstr=_str_1).upper()
            _str_2 =""
            _DATA_300_list = list(_DATA_300.values())
            for idx,val in enumerate(_DATA_300_list):
                if idx ==0:
                    _str_2+=val
                else:
                    _str_2+=","+val
            _str_send = "{"+_str_2+"}"+"\n"+"\r"

            print("Checksumn = %s"%(_DATA_300["str3"]))
            # print(_str_send)
            sock.send(_str_send.encode('utf-8'))
            # print(type(_str_send))
            # print(type(_str_send.encode('utf-8')))
            _str_recv = sock.recv(1024)
            print(type(_str_recv))
            print(_str_recv.decode("utf-8"))
        except Exception as e :
            print(e)
        finally:
            sock.close()
        sleep(0.1)
    sock.close()

if __name__ == "__main__":
    main()