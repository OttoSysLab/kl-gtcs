import json
import socket
from datetime import datetime
from time import sleep,perf_counter_ns
import os

# CMD302, 工具控制命令
_CMD_302 = {          # {CMDstr1,str2,str3,str4}                                             
    "str1"   :"CMD302",                                   # Header+DATA
    "str2"   :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   :"0119",                                     # check sum ,4 chars
    "str4"   :"001",                                      # Command_sn
    "str5"   :"0",                                        # Tool control 
}
# CMD340, 設定Initial
# {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}				
_CMD_340 = {                                                        
    "str1"   : "CMD340",                                    # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"),  # yyyyMMdd HH:mm:ss
    "str3"   : 0,                                           # check sum ,4 chars
    "str4"   : 0,                                           # Command_sn
    "str5"   : 80.0,                                        # Min temperature z
    "str6"   : 0,                                           # Max temperature
    "str7"   : "0",                                         # Max current
    "str8"   : "0",                                         # Max peak current
    "str9"   : "0",                                         # torque sensor type
    "str11"  : "0",                                         # Max torque
    "str12"  : "0",                                         # PWM frequency 
    "str13"  : "0",                                         # Max rpm
    "str14"  : "0",                                         # Max slope
    "str15"  : "0",                                         # Min bus voltage
    "str16"  : "0",                                         # Max bus voltage
    "str17"  : "0",                                         # Start duty cycle
    "str18"  : "0",                                         # Gear box ratio
    "str19"  : "0",                                         # Start input source
    "str20"  : "0",                                         # Reverse ipnut source
    "str21"  : "0",                                         # Reverse rpm
    "str22"  : "0",                                         # Reverse slope
    "str23"  : "0",                                         # Reverse max current
    "str24"  : "0",                                         # Reverse max torque
    "str25"  : "0",                                         # Error idle time
    "str26"  : "0",                                         # Bachlash 
    "str27"  : "0",                                         # Proportional gain
    "str28"  : "0",                                         # Integral gain
    "str29"  : "0",                                         # Encoder
}
# REQ 300.
_REQ_300 = {
    "str1"   : "REQ300",                                   # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   : "0",                                        # check sum ,4 chars
    "str4"   : "0",                                        # Command_sn
}
# REQ 302.
_REQ_302 = {                                                       
    "str1"              :"REQ302",                                   # Header+DATA
    "str2"              :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"              :"1245",                                     # check sum ,4 chars        
    "str4"              :"225",                                      # Command_sn
    "str5"              :"3",                                        # Target Torque 
    "str6"              :"10",                                       # Max Current
    "str7"              :"0",                                        # Angle Selflearning
    "str8"              :"0",                                        # SI Angle Monitoring
    "str9"              :"0",                                        # SI Angle Positioning
    "str10"             :"0",                                        # Sl Wait Start
    "str11"             :"1",                                        # SI Turn Back 
}
# Calcualate check sum.
def calculate_check_sum(pstr:str)->str: #
    _value = 0
    for val in pstr:
        _value += ord(val)
    return str(hex(_value))[2:].zfill(4).upper() # 我懶得寫註解,請先讀完python再來看這行

# Encode AMS protocol command string.
def encode_ams_cmd(pdict:dict)->str:
    _value_list = list(pdict.values())
    _check_list = _value_list[3:]
    # 組data 字串
    _check_str  = ""
    for val in _check_list:
        _check_str += ","+str(val)
    # Calculate checku sum.
    _check_sum = calculate_check_sum(pstr=_check_str)
    # Package command to string.
    _value_list[1] = datetime.now().strftime("%Y%m%d %H:%M:%S.%f")[:21]
    _value_list[2] = _check_sum
    _str = "{"
    _len = len(_value_list)
    for idx,val in enumerate(_value_list):
        if idx<(_len-1):
            _str+=str(val)+","
        else:
            _str+=str(val)
    _str += "}"
    return _str

# Test socket client.
def test_socket_client():
    # IP =  "127.0.0.1"
    IP =  "192.168.0.207"
    PORT = 9000
    # 載入傳送的字串
    # _ams_str = encode_ams_cmd(_CMD_302)
    # _ams_str = _CMD_340
    _ams_str = encode_ams_cmd(pdict = _REQ_300)
    # _ams_str = encode_ams_cmd(pdict = _REQ_302)
    
    # 寫目前的測試address到檔案
    # _PATH = "socket_log.csv"  
    # with open(_PATH, 'a+') as f:f.write("Time"+','+"data"+","+'\n')    
    # _connected = False

    while(True):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((IP, PORT))
            t1 = perf_counter_ns()
            sock.send(_ams_str.encode('utf-8'))
            _sock_recv = sock.recv(1024).decode('utf-8')
            t2 = perf_counter_ns()
            pre_time = (t2-t1)/1000000
            print(_sock_recv)
            print("Pre time = %s (ms)"%(str(pre_time)))          
        except Exception as e:
            print(e)
        finally:
            sock.close()
        sleep(0.1)
    sock.close()

if __name__ == '__main__':
    test_socket_client()
