import time 

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

while(True):
    set_io(1,True)
    time.sleep(1)
    set_io(1,False)
    time.sleep(1)