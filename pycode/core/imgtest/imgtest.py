from platform import system
from serial import Serial 
import datetime
import csv
import os
import time 

# 宣告全域變數
_USB_PATH = ""
_DELAY_TIME = 5
# _FILE_PATH = "/media/kls/otto1/imgtest.csv"
_FILE_PATH = "/home/kls/project/imgTest/imgtest.csv"

#main
def main():
    _check_sys = system()
    _time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    if _check_sys == "Windows":
        # 寫目前的時間到USB       
        with open(os.getcwd()+"\\imgtest.csv", 'a+') as f:f.write(_time+','+'\n')
    elif _check_sys == "Linux":
        # 寫目前的時間到USB       
        # with open(_FILE_PATH, 'a+') as f:f.write(_time+','+'\n')
        with open(_FILE_PATH, 'a+') as f:
            f.write(_time+","+'\n')
            f.flush()
        time.sleep(_DELAY_TIME)
        _ser = Serial("/dev/ttymxc3",115200,timeout=0.5) #使用ttymxc3連線序列口
        _ser.write('c'.encode('utf-8'))
        _ser.flush()       

if __name__ == "__main__":
    main()



