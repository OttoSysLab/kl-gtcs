import time 
import datetime
import multiprocessing

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

def Process_flashing_light(q_pin,q_time):
    #t_now = time.perf_counter()
    _q_pin = q_pin.get()
    _q_time = q_time.get()
    while(True):
        #t_now = time.perf_counter()
        set_io(1,True)
        set_io(2,True)
        set_io(3,True)
        set_io(4,True)
        time.sleep(_q_time)
        set_io(1,False)
        set_io(2,False)
        set_io(3,False)
        set_io(4,False)
        time.sleep(_q_time)
        #print('Sub thread pre_time = %s (s)'%(time.perf_counter()-t_now))

def main():
    _flash_pin = 0x01
    _flash_time = 0.1
    _q_flash_pin = multiprocessing.Queue()
    _q_flash_pin.put(_flash_pin,False)
    _q_flash_time = multiprocessing.Queue()
    _q_flash_time.put(_flash_time,False)
    _p_flash = multiprocessing.Process(target=Process_flashing_light,args=(_q_flash_pin,_q_flash_time,))
    _p_flash.start()

    while(True):
        #t_now = time.perf_counter_ns()
        t_now = time.perf_counter()
        
        time.sleep(10)
        #print('through time = %s (ms)'%(time.perf_counter_ns()-t_now))
        print('Main thread pre_time = %s (s)'%(time.perf_counter()-t_now))
        

if __name__ == "__main__":
    main() 
