from platform import system
from serial import Serial 
import datetime
import csv
import os 
import psutil
from functools import singledispatch
from time import sleep,process_time_ns

# 宣告全域變數
_USB_PATH = ""
_DELAY_TIME = 5

@singledispatch
def get_cpu_state(obj):
    return (obj,type(obj),"obj")

@get_cpu_state.register(int)
def _(interval):
    return psutil.cpu_percent(interval)

@get_cpu_state.register(str)
def _(retStr):
    _interval = int(retStr)
    return ("CPU : " + str(psutil.cpu_percent(_interval)) + "%")

@singledispatch
def get_memory_state(obj):
    return (obj,type(obj),"obj")

@get_memory_state.register(int)
def _(_int)->int:
    phymem = psutil.virtual_memory()
    return (phymem.used/1024/1024)

@get_memory_state.register(str)
def _(_str)->str: 
    phymem = psutil.virtual_memory()
    line = "Memory: %5s%% %6s/%s"%(
        phymem.percent,
        str(int(phymem.used/1024/1024))+"M",
        str(int(phymem.total/1024/1024))+"M"
        )
    return line 

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
    return mem 

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
    _cpu_list = [0]*4
    for idx in range(4): _cpu_list[idx] = str(get_cpu_state(idx))    # Read cpu load.
    _mem = str(get_memory_state(1))                                  # Read memory.
    _check_sys = system()
    if _check_sys == "Windows":       
        with open(os.getcwd()+"\\memtest.csv", 'a+') as f:
            f.write(_time+','+_cpu_list[0]+','+_cpu_list[1]+','+_cpu_list[2]+','+_cpu_list[3]+','+_mem +'\n')
            f.flush()
    elif _check_sys == "Linux":

        # 寫目前的時間到USB      
        with open("/media/kls/OTTO1/memtest.csv", 'a+') as f:
            f.write(_time+','+_cpu_list[0]+','+_cpu_list[1]+','+_cpu_list[2]+','+_cpu_list[3]+','+_mem +'\n')
            f.flush()       

def main():
    with open(os.getcwd()+"\\memtest.csv", 'a+') as f:
        f.write(  "Time,"+'CPU_1(%),'+'CPU_2(%),'+'CPU_3(%),'+'CPU_4(%),'+'MEM(MB)'+'\n')
        f.flush()
    while True:
        get_sys_status()
        sleep(1)        


if __name__ == "__main__":
    main()



