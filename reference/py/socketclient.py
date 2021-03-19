import json
import socket
from datetime import datetime
from time import sleep,perf_counter_ns
import os

DEBUG_MODE = True

if DEBUG_MODE == True:
    # IP =  "192.168.0.38"  # i.Max8 :38.
    IP =  "192.168.0.54"  # i.Max8 :38.
    # IP =  "192.168.0.202"  # OTTO :207 , Eric = 202.
    # IP =  "192.168.0.207"  # OTTO :207 , Eric = 202.
else:
    IP =  "127.0.0.1"
PORT = 9000

#region CMD
# CMD301, 條碼/手動切換工作
_CMD_301 = {
    "str1"   :"CMD301",                                   # Header+DATA
    "str2"   :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   :"0119",                                     # check sum ,4 chars
    "str4"   :"001",                                      # Command_sn
    "str5"   :"1",                                        # Job ID, nnornal - 0;
}
# CMD302, 工具控制命令
_CMD_302 = {          # {CMDstr1,str2,str3,str4}                                             
    "str1"   :"CMD302",                                   # Header+DATA
    "str2"   :datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   :"0119",                                     # check sum ,4 chars
    "str4"   :"001",                                      # Command_sn
    "str5"   :"0",                                        # Tool control = enable
    # "str5"   :"1",                                        # Tool control = disabke
}
# CMD310, 設定Normal Torque
_CMD_310 = {                                               # Nornal Troque
    "str1"   : "CMD310",                                   # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   : "0119",                                     # check sum ,4 chars
    "str4"   : "001",                                      # Command_sn
    "str5"   : "0",                                        # Direction
    "str6"   : "0",                                        # PRM
    "str7"   : "0",                                         # Target orque
    "str8"   : "0",                                         # Torque threshold
    "str9"   : "0",                                         # Joint offset
    "str10"  : "0",                                         # Monitoring mode
    "str11"  : "0",                                         # Torque window
    "str12"  : "0",                                         # High torque
    "str13"  : "0",                                         # Low torque
    "str14"  : "0",                                         # High angle
    "str15"  : "0",                                         # Low angle
}
# CMD311, 設定Normal Angle
_CMD_311 = {                                               # Nornal angle
    "str1"   : "CMD311",                                   # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   : "0119",                                     # check sum ,4 chars
    "str4"   : "001",                                      # Command_sn
    "str5"   : "0",                                        # Direction
    "str6"   : "0",                                        # PRM
    "str7"   : "0",                                         # Target orque
    "str8"   : "0",                                         # Torque threshold
    "str9"   : "0",                                         # Monitoring mode
    "str10"  : "0",                                         # Torque window
    "str11"  : "0",                                         # High torque
    "str12"  : "0",                                         # Low torque
    "str13"  : "0",                                         # High angle
    "str14"  : "0",                                         # Low angle
}
# CMD312, 設定Normal Unscrew
_CMD_312 = {                                               # Nornal Unscrew
    "str1"   : "CMD312",                                   # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   : "0119",                                     # check sum ,4 chars
    "str4"   : "001",                                      # Command_sn
    "str5"   : "0",                                        # PRM
    "str6"   : "0",                                        # Force
    "str7"   : "0",                                         # Dircetion
}
# CMD313, 設定Normal Unscrew
_CMD_313 = {                                               # Nornal Unscrew
    "str1"   : "CMD313",                                   # Header+DATA
    "str2"   : datetime.now().strftime("%Y%m%d %H:%M:%S"), # yyyyMMdd HH:mm:ss
    "str3"   : "0119",                                     # check sum ,4 chars
    "str4"   : "001",                                      # Command_sn
    "str5"   : "0",                                        # PRM 
    "str6"   : "0",                                        # Force 
    "str7"   : "0",                                        # Dircetion
}
# CMD340, 設定Initial, {CMDstr1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16,str17}			
_CMD_340 = {
    "str1"  : "CMD340",                                   # Header+DATA
    "str2"  : "yyyyMMdd HH:mm:ss",                        # yyyyMMdd HH:mm:ss
    "str3"  : "0",                                        # check sum ,4 chars        
    "str4"  : "0",                                        # Command_sn
    # DATA
    "str5"  : "0.0",                                      # Min temperature 
    "str6"  : "80.0",                                     # Max temperature
    "str7"  : "30000",                                    # Max current
    "str8"  : "2500",                                     # Max peak current
    "str9"  : "1",                                        # torque sensor type,TCS300=0,TCS500=1,TCS700=2,TCC_3NM=3,TCC_LT=4
    "str10" : "100.0",                                    # Max duty cycle
    "str11" : "5.0",                                      # Max torque 
    "str12" : "2",                                        # PWM frequency 
    "str13" : "1200",                                     # Max rpm
    "str14" : "2000",                                     # Max slope
    "str15" : "10.0",                                     # Min bus voltage
    "str16" : "80.0",                                     # Max bus voltage
    "str17" : "3.0",                                      # Start duty cycle
    "str18" : "11.11",                                    # Gear box ratio 13Nm=11.11,5Nm=16.00 ,7Mn = 27.04
    "str19" : "2",                                        # Start input source
    "str20" : "1",                                        # Reverse ipnut source
    "str21" : "1000",                                     # Reverse rpm
    "str22" : "2000",                                     # Reverse slope
    "str23" : "15000",                                    # Reverse max current
    "str24" : "5.0",                                      # Reverse max torque
    "str25" : "200",                                      # Error idle time
    "str26" : "0",                                        # Bachlash 
    "str27" : "200",                                      # Proportional gain
    "str28" : "200",                                      # Integral gain
    "str29" : "1",                                        # Encoder
    # New
    "str30" : "0.0",                                      # Min Torque
    "str31" : "10",                                       # Min RPM
    "str32" : "10",                                       # Reverse Min RPM
    "str33" : "16908288",                                 # DmsSWVer
    "str34" : "65536",                                    # DmsCoreID
    "str35" : "444",                                      # DmsSerNr
    "str36" : "0",                                        # Led
    "str37" : "60",                                       # Lever Sensitivity
    "str38" : "30",                                       # Push Sensitivity
    "str39" : "01.63",                                    # MotSWVer 
    # "str40" : "\n\r",                                   # CR,LF
}
#endregion

#region REQ
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
#endregion

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
#endregion

# Test socket client.
def test_socket_client():
    '''
    FUCK!!!
    '''
    # 載入傳送的字串
    _ams_dict = dict()  
    
    # AMS command.
    _ams_dict = _CMD_301       # Seitch Jobs
    # _ams_dict = _CMD_302
    # _ams_dict = _CMD_310
    # _ams_dict = _CMD_311
    # _ams_dict = _CMD_312
    # _ams_dict = _CMD_313
    # _ams_dict = _CMD_340       # Initial ID2. 

    # AMS REQ.
    _ams_dict = _REQ_300
    # _ams_dict = _REQ_302   

    # Convert dictionary to  ams string.
    _ams_str = encode_ams_cmd(pdict =_ams_dict)
    
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
            if _ams_dict['str1']!='REQ300':
                break
        except Exception as e:
            print(e)
        finally:
            sock.close()
        sleep(0.1)
    sock.close()

# Main 
if __name__ == '__main__':
    # test_package_cmd()
    test_socket_client()
