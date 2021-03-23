#!/usr/bin/python
# -*- coding: UTF-8 -*-
from serial import Serial 
from time import sleep
from json import JSONDecodeError,JSONDecoder,JSONEncoder
from enum import Enum

class UINT(Enum):
    kgf_m  = 0
    N_m    = 1
    kgf_cm = 2
    lbf_in = 3

#region MCB commport.
class CommonFunction:
    # Initial object        
    def __init__(self):
        self.__ser = ''

    # check_json
    def check_json(self,myjson):
        _JD = JSONDecoder()
        json_object = ''
        try:
            json_object = _JD.decode(myjson)
        except ValueError as e:
            print(e)
        finally:
            return json_object
    
    # compare dict.
    def compare_dict(self,dict1,dict2):
        _JE = JSONEncoder()
        _j1 = _JE.encode(dict1)
        _j2 = _JE.encode(dict2)

        _cmp_ok = False
        if(_j1==_j2):
            _cmp_ok = True
        return _cmp_ok

    # Compare list.
    def compare_bitwise(self,x, y):
        set_x = frozenset(x)
        set_y = frozenset(y)
        return set_x & set_y
    
    # Compare list.
    def compare_listcomp(self,x,y):
        return [i for i, j in zip(x, y) if i == j]

    # Convert to bit list.
    def convert_to_bit_list(self,length:int,data:int)->list:
        _list = [None]*length
        for idx,val in enumerate(_list): 
            if (data & (1<<idx)):_list[idx] = 1
            else:_list[idx] = 0
        return _list
        
    # Convert to list
    def convert_to_list(self,num,len):
        _list = []
        for idx in range(len):
            _list.append(num%256)
            num>>=8
        return _list
        
    # Convert array to int value.
    def convert_to_value(self,carray):
        _value = 0  
        _rev_lst = list(reversed(carray))
        _max_idx = len(_rev_lst)-1      
        for idx,val in enumerate(_rev_lst):
            _value += val
            if idx < _max_idx:_value <<=8 
        return _value 
#endregion  