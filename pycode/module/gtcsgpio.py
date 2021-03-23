#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
from time import sleep,perf_counter_ns

class GtcsGPIO:
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
        "/dev/OUT11/value",  # enable 24V
        "/dev/OUT12/value",  # enable 5V
        "/dev/OUT13/value"   # buzzer
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
    
    # Get IO status.
    def __get_input_status(self,Pin_Num):
        with open(self.PIN_IN_FILE_ARRAY[Pin_Num],"r") as f:
            return f.read()

    # Set file
    def __set_output_status(self,Pin_Num,Pin_status):
        try:
            # open file.
            f = open(self.PIN_OUT_FILE_ARRAY[Pin_Num],"w")
            # wirte c,d to file
            if Pin_status==True:
                f.write(chr(49))
            else:
                f.write(chr(48))
        except Exception as e :print(e)
        finally:
            f.close() 

    # Flasging  light.
    def turn_on_all_output(self):
        # pid=os.getpid()
        # os.system("sudo chrt -f -p 99 '%s'" %(pid))
        # os.system("sudo taskset -cp 1 '%s'" %(pid))

        for idx in range(10):
            self.__set_output_status(Pin_Num=idx,Pin_status=True)

        # _list = []
        # _value = 0
        # for idx,val in enumerate(_list):
        #     _list.append(int(idx))
        #     _value += sum(_list)
        # return _value

    def turn_off_all_output(self):
        for idx in range(10):
            self.__set_output_status(Pin_Num=idx,Pin_status=False)
            
        # _list = []
        # _value = 0
        # for idx,val in enumerate(_list):
        #     _list.append(int(idx))
        #     _value += sum(_list)
        # return _value

    # Output
    def get_all_intput_status(self):
        # pid=os.getpid()
        # os.system("sudo chrt -f -p 99 '%s'" %(pid))
        # os.system("sudo taskset -cp 1 '%s'" %(pid))
        # #t_now = time.perf_counter()
        # _q_pin = q_pin.get()
        # _q_time = q_time.get()
        _lst = []
        for idx in range(9):        
            _lst.append(self.__get_input_status(Pin_Num=idx))
        return _lst
        
        # _list = []
        # _value =0
        # for idx,val in enumerate(_list):
        #     _list.append(int(idx))
        #     _value += sum(_list)
        # return _value
