from platform import system
from serial import Serial 
import datetime
import csv
import os 
from functools import singledispatch
from time import sleep,process_time_ns

# 宣告全域變數
_time = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
_LOG_PATH = "/media/kls/OTTO1/memtest_"+_time+".csv"
_DELAY_TIME = 5

def memory_stat(): 
    mem = {} 
    f = open("/proc/meminfo") 
    lines = f.readlines() 
    f.close() 
    for line in lines: 
        if len(line) < 2: continue 
        name = line.split(':')[0] 
        var = line.split(':')[1].split()[0] 
        mem[name] = int(var) * 1024.0 
    mem['MemUsed'] = mem['MemTotal'] - mem['MemFree'] - mem['Buffers'] - mem['Cached'] 
    # return mem
    return (mem['MemUsed']/1024/1024)

def cpu_stat(): 
    cpu = [] 
    cpuinfo = {} 
    f = open("/proc/cpuinfo") 
    lines = f.readlines() 
    f.close() 
    for line in lines: 
        if line == '/n': 
            cpu.append(cpuinfo) 
            cpuinfo = {} 
        if len(line) < 2: continue 
        name = line.split(':')[0].rstrip() 
        var = line.split(':')[1] 
        cpuinfo[name] = var 
    return cpu  

#main
def get_sys_status():
    _time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    # 寫目前的時間到USB      
    _mem = memory_stat()
    _cpu = cpu_stat()
    print("memory status = ",end="")
    print(_mem)
    # print("cpu status = ",end ="")
    # print(_cpu)
    with open(_LOG_PATH, 'a+') as f:
        f.write(_time+','+str(_mem)+'\n')
        f.flush()       

def main():
    with open(_LOG_PATH, 'a+') as f:
        f.write(  "Time,"+'MEM(MB)'+'\n')
        f.flush()
    while True:
        get_sys_status()
        sleep(1)        


if __name__ == "__main__":
    main()



