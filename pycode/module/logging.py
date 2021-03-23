#!/usr/bin/python
# -*- coding: UTF-8 -*-
from platform import system
from datetime import datetime
from enum import Enum
import os

class log():
    # Initial path.
    # Initial logging obj.
    _check_sys = system()
    _time = datetime.now().strftime("%Y%m%d_%H%M%S")
    if _check_sys == "Windows":
        __LOG_FILENAME = os.getcwd()+"/logging/gtcsdriver_"+_time+".csv"
    elif _check_sys == "Linux":
        # __LOG_FILENAME = "/home/kls/project/gtcs/logging/gtcsdriver"+_time+".csv"
        __LOG_FILENAME = "/media/kls/OTTO1/gtcsdriver_"+_time+".csv"
    
    # Define logging level.
    __NOTSET    = 0            # 0
    __DEBUG     = 10           # 10
    __INFO      = 20           # 20
    __WARNING   = 30           # 30
    __ERROR     = 40           # 40
    __CRITICAL  = 50           # 50

    __start_log = True 

    # Initial logging leverl.
    __logging_lever = 0

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass

    # Set logging level.
    def set_logging_level(self,level:int):self.__logging_lever = level
    
    # Create 
    def check_logging_file(self,Path:str):
        _check_ok = False
        try:
            if self.__start_log == True:
                if os.path.isfile(Path) == False:
                    # Write title.
                    with open(Path, 'a+') as f:
                        f.write("date,"+"time,"+"s,"+"level,"+"\n")
                        f.flush()
            _check_ok = True
        except Exception as e :print(e)
        return _check_ok

    # logging debug.
    def debug(self,msg:str):    
        try:
            if (self.check_logging_file(Path=self.__LOG_FILENAME) == True):
                if (self.__logging_lever <= self.__DEBUG)&(self.__start_log == True): 
                    _time = datetime.now().strftime('%Y-%m-%d,%H:%M,%S.%f')
                    _str  = _time+",BEGUE,"+msg+"\n"
                    # Write title.
                    # with open(self.__LOG_FILENAME, 'a+') as f:
                    #     f.write(_str)
                    #     f.flush()
                    print(_str)
        except Exception as e:print(e)
    
    # logging debug. 
    def info(self,msg:str):     
        try:
            if (self.check_logging_file(Path=self.__LOG_FILENAME) == True):
                if (self.__logging_lever <= self.__INFO)&(self.__start_log == True):
                    _time = datetime.now().strftime('%Y-%m-%d,%H:%M,%S.%f')
                    _str  = _time+",INFO,"+msg+"\n"
                    # Write title.
                    with open(self.__LOG_FILENAME, 'a+') as f:
                        f.write(_str)
                        f.flush()
                    print(_str)
        except Exception as e:print(e)

    # logging warning. 
    def warning(self,msg:str):                   
        try:
            if (self.check_logging_file(Path=self.__LOG_FILENAME) == True):
                if (self.__logging_lever <= self.__WARNING)&(self.__start_log == True):
                    _time = datetime.now().strftime('%Y-%m-%d,%H:%M,%S.%f')
                    _str  = _time+",WARNING,"+msg+"\n"
                    # Write title.
                    with open(self.__LOG_FILENAME, 'a+') as f:
                        f.write(_str)
                        f.flush()
                print(_str)
        except Exception as e:print(e)

    # logging error.
    def error(self,msg:str):     
        try:
            if (self.check_logging_file(Path=self.__LOG_FILENAME) == True):
                if (self.__logging_lever <= self.__ERROR)&(self.__start_log == True):
                    _time = datetime.now().strftime('%Y-%m-%d,%H:%M,%S.%f')
                    _str  = _time+",ERROR,"+msg+"\n"
                    # Write title.
                    with open(self.__LOG_FILENAME, 'a+') as f:
                        f.write(_str)
                        f.flush()
                print(_str)
        except Exception as e:print(e)

    # logging error. 
    def critical(self,msg:str):      
        try:
            if (self.check_logging_file(Path=self.__LOG_FILENAME) == True):
                if (self.__logging_lever <= self.__CRITICAL)&(self.__start_log == True):
                    _time = datetime.now().strftime('%Y-%m-%d,%H:%M,%S.%f')
                    _str  = _time+",CRITICAL,"+msg+"\n"
                    # Write title.
                    with open(self.__LOG_FILENAME, 'a+') as f:
                        f.write(_str)
                        f.flush()
                    print(_str)
        except Exception as e:print(e)
        