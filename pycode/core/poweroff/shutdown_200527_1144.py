import time 
from os import system as sys 

PIN_OUT_FILE_ARRAY=[
    "/dev/OUT1/value",
    "/dev/OUT2/value",
    "/dev/OUT3/value",
    "/dev/OUT4/value",
    "/dev/OUT5/value",
    "/dev/OUT6/value",
    "/dev/OUT7/value",
    "/dev/OUT8/value",
    "/dev/OUT9/value",
    "/dev/OUT10/value",
    "/dev/OUT11/value",
    "/dev/OUT12/value",
    "/dev/OUT13/value"
]

PIN_IN_FILE_ARRAY=[
    "/dev/IN1/value",
    "/dev/IN2/value",
    "/dev/IN3/value",
    "/dev/IN4/value",
    "/dev/IN5/value",
    "/dev/IN6/value",
    "/dev/IN7/value",
    "/dev/IN8/value",
    "/dev/IN9/value",
    "/dev/INT10/value",
    "/dev/IN11/value",
    "/dev/IN12/value"
]
# get file
def set_io(Pin_Num,Pin_status):
    try:
        # open file.
        f = open(PIN_OUT_FILE_ARRAY[Pin_Num],"w")
        # wirte c,d to file
        if Pin_status==True:f.write(chr(49))
        else: f.write(chr(48))
    except Exception as e :print(e)
    finally:f.close() 

def get_io(Pin_Num):
    with open(PIN_IN_FILE_ARRAY[Pin_Num],"r") as f:return f.read()
'''
sys("sudo su root")
sys("kls")
'''
sys("shutdown -h 0 ")

'''
while(True):
    _in = int(get_io(Pin_Num=1))
    if _in==0: print(_in)
    else:sys("shutdown -h 0 ")
    #else:print(_in)
    #print(int(get_io(Pin_Num=1)))
'''

