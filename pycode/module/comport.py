#!/usr/bin/python
# -*- coding: UTF-8 -*-
from module import logging as log
from serial import Serial,SerialException
from time import sleep
from sys import platform 
from glob import glob

#region MCB commport.
class MCBComm:
    # Initial object
    def __init__(self):
        self.__ser = ''
        self.__log = log.log()

    # Initial MCB comport.
    def open_MCB_comm(self,comm,baudrate):
        """
        docstring
        """
        _comport_ok = False
        # Config serial port.
        try:
            self.__ser = Serial(port = comm,
                                baudrate = baudrate,
                                parity = 'N',
                                stopbits = 1,
                                bytesize =8,
                                timeout=0.1 )
            if self.__ser.isOpen():
                _comport_ok=True
        except IOError as e:
            self.__log.error(msg=str(e))
            print(e)
        except Exception as e:
            self.__log.error(msg=str(e))
            print(e)
        return _comport_ok

    # write dat to smcb.
    def write_to_MCB(self,data):
        """
        docstring
        """
        try:
            self.__ser.write(data)
            self.__ser.flush()
        except Exception as e:
            print(e)

    # read data from mcb.
    def read_MCB_data(self):
        """
        docstring
        """
        _ser_buf = []
        while (self.__ser.inWaiting()):
            _ser_buf.append(int.from_bytes(self.__ser.read(),byteorder="big",signed=False))
        return _ser_buf

    def reset_output_buffer(self):
        """
        docstring
        """
        self.__ser.reset_output_buffer()

    # Close MCB comport.
    def close_MCB_comm(self):
        """
        docstring
        """
        self.__ser.close()
    # Get serial port form OS.
    def __get_ports(self):
        """ Lists serial port names
            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif platform.startswith('linux') or platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob('/dev/tty[A-Za-z]*')
        elif platform.startswith('darwin'):
            ports = glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')
 
        result = []
        for port in ports:
            try:
                s = Serial(port)
                s.close()
                result.append(port)
            except (OSError, SerialException):
                pass
        return result 

    # Get ports form OS and selected one to set serial.
    def select_MCB_port(self)->str:
        """
        docstring
        """
        _ports = self.__get_ports()                 # Get ports form OS.
        print("Serial Ports list = ")
        for idx,val in enumerate(_ports):
            print("%d.%s"%(idx,val))
        _idx = input("Pls select MCBComm = ")       # Select port form serial ports list.
        _port = _ports[int(_idx)]            
        print("MCBComm = %s "%(_port))      
        return _port     
#endregion


