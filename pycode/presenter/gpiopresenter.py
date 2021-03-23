#!/usr/bin/python
# -*- coding: UTF-8 -*-
from time import sleep,perf_counter_ns
from module import gtcsgpio as gpio
import os 
import json

#region 
class GtcsGPIOPresenter():
    __gpio  = gpio.GtcsGPIO()
    def __init__(self):
        pass
    # 
    def turn_on_all_output(self):self.__gpio.turn_on_all_output()
    def turn_off_all_output(self):self.__gpio.turn_on_all_output()
    def get_all_intput_status(self):self.__gpio.get_all_intput_status()
#endregion
