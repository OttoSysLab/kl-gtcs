#!/usr/bin/python
# -*- coding: UTF-8 -*-
from time import sleep,perf_counter_ns
from module import comport as com

#region RS232 presenter.
class Rs485Presenter():
    rs485 = com.MCBComm()
    def __init__(self):
        pass
#endregion