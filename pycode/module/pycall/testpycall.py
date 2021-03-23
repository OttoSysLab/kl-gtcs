#!/usr/bin/python
# -*- coding: UTF-8 -*-
from ctypes import cdll,c_char,c_uint8,POINTER,c_wchar_p
from platform import system 
import os 

try:
    link_so = cdll.LoadLibrary
    _ckeck_sys = system() 
    if _ckeck_sys =="Windows":
        path_breach = "\module\pycall\winlibpycallclass.so"
        _str_path = os.path.abspath(".")+path_breach
        print("Fcuk windows")
    elif _ckeck_sys == "Linux":
        path_breach = "/home/kls/project/gtcs/module/pycall/tcslibpycallclass.so"
        _str_path = path_breach
        print("Fcuk linux")
    # _str_path = os.path.abspath(".")+path_breach
    if os.path.exists(_str_path):
        pycall = link_so(_str_path)
        print("Fcuk os")
    else:
        print("Fcuk pass")
        pass
except Exception as e:print(e)

#region
MCBCtrlTelegram = [0]*48
MCBCtrlTelegram[0] = 0x01
MCBCtrlTelegram[1] = 0x00
MCBCtrlTelegram[2] = 0x00
MCBCtrlTelegram[3] = 0x00
MCBCtrlTelegram[4] = 0x00
MCBCtrlTelegram[5] = 0x00
MCBCtrlTelegram[6] = 0x00
MCBCtrlTelegram[7] = 0x00
MCBCtrlTelegram[8] = 0x01
MCBCtrlTelegram[9] = 0x24
MCBCtrlTelegram[10] = 0x00
MCBCtrlTelegram[11] = 0x00
MCBCtrlTelegram[12] = 0xa0
MCBCtrlTelegram[13] = 0x0f
MCBCtrlTelegram[14] = 0x01
MCBCtrlTelegram[15] = 0x00
MCBCtrlTelegram[16] = 0xe8
MCBCtrlTelegram[17] = 0x03
MCBCtrlTelegram[18] = 0xe8
MCBCtrlTelegram[19] = 0x03
MCBCtrlTelegram[20] = 0x74
MCBCtrlTelegram[21] = 0x01
MCBCtrlTelegram[22] = 0xe8
MCBCtrlTelegram[23] = 0x03
MCBCtrlTelegram[24] = 0x00
MCBCtrlTelegram[25] = 0x00
MCBCtrlTelegram[26] = 0x00
MCBCtrlTelegram[27] = 0x00
MCBCtrlTelegram[28] = 0x08
MCBCtrlTelegram[29] = 0x07
MCBCtrlTelegram[30] = 0x00
MCBCtrlTelegram[31] = 0x00
MCBCtrlTelegram[32] = 0x00
MCBCtrlTelegram[33] = 0x00
MCBCtrlTelegram[34] = 0x00
MCBCtrlTelegram[35] = 0x00
MCBCtrlTelegram[36] = 0x00
MCBCtrlTelegram[37] = 0x00
MCBCtrlTelegram[38] = 0x00
MCBCtrlTelegram[39] = 0x00
MCBCtrlTelegram[40] = 0x00
MCBCtrlTelegram[41] = 0x00
MCBCtrlTelegram[42] = 0x00
MCBCtrlTelegram[43] = 0x00
MCBCtrlTelegram[44] = 0x46   # DEV = 70
MCBCtrlTelegram[45] = 0xcf   # DEV = 206
MCBCtrlTelegram[46] = 0xba   # DEV = 186
MCBCtrlTelegram[47] = 0xbe   # DEV = 190
#endregion

try:
    _buffer = MCBCtrlTelegram
    pycall.calculateCrc32((c_uint8 * 48)(*_buffer))
    f = pycall.getCrc32Value_all 
    f.restype=POINTER(c_uint8 * 4)
    _arr = f().contents
    # Combin tlg array.
    print([_arr[0],_arr[1],_arr[2],_arr[3]])
except Exception as e:
    print("Get err")

try:
    _com_chars = list("/dev/ttymxc3")
    # print(_com_chars)
    # _com_list = []
    # for var in _com_chars:
    #     print(type(var))
    #     _com_list.extend(chr(var))
    # print(_com_list)
    # _port_num = pycall.openMCBCom((c_char * len(_com_chars))(*_com_list))
    _port_num = pycall.openMCBCom("/dev/ttymxc3")
    print("Open Comm number = %d"%(_port_num))
    # print(pycall.checkCrc32((c_uint8 * 48)(*MCBCtrlTelegram)))
    # _com_num = pycall.getMCBCommNum()
    # print(_com_num)
    # print("ComPort number = %d"%(int(_com_num)))
except Exception as e:print(e)    
