#!/usr/bin/python
# -*- coding: UTF-8 -*-

from module import common
from module import crcchecker as crc
import numpy as np
from enum import Enum

class TcsTestDriver:
    # Inital attribute.
    _test_arr = np.zeros((48,),dtype=np.ubyte)
    _debug_mode = False
    
    # Initial object
    def __init__(self,debug_mode:bool=None) -> None:
        self.initial_test_arr(self._test_arr)
        self._debug_mode = debug_mode

    # Initial test array.
    def initial_test_arr(self,parr:np.array)->bool:
        """
        docstring
        """
        #region test array.
        parr[0] = 1
        parr[1] = 0
        parr[2] = 0
        parr[3] = 0
        parr[4] = 0
        parr[5] = 0
        parr[6] = 0
        parr[7] = 0
        parr[8] = 1
        parr[9] = 36
        parr[10] = 0
        parr[11] = 0
        parr[12] = 160
        parr[13] = 15
        parr[14] = 1
        parr[15] = 0
        parr[16] = 232
        parr[17] = 3
        parr[18] = 208
        parr[19] = 7
        parr[20] = 116
        parr[21] = 1
        parr[22] = 232
        parr[23] = 3
        parr[24] = 0
        parr[25] = 0
        parr[26] = 0
        parr[27] = 0
        parr[28] = 0
        parr[29] = 0
        parr[30] = 0
        parr[31] = 0
        parr[32] = 0
        parr[33] = 0
        parr[34] = 0
        parr[35] = 0
        parr[36] = 0
        parr[37] = 0
        parr[38] = 0
        parr[39] = 0
        parr[40] = 0
        parr[41] = 0
        parr[42] = 0
        parr[43] = 0
        parr[44] = 0           # 149 
        parr[45] = 0           # 223
        parr[46] = 0           # 23
        parr[47] = 0           # 226
        #endregion
        return True

    # Run tcs test driver main process.
    def run(self):
        """
        Run tcs test driver main process.
        """
        # Get CRC 32 check sum.
        _crc = crc.CrcChecker()
        _result = _crc.get_crc32_array(self._test_arr)
        self._test_arr[44] = _result[0]
        self._test_arr[45] = _result[1]
        self._test_arr[46] = _result[2]
        self._test_arr[47] = _result[3]

        # Display test parray 
        if self._debug_mode==True:
            for idx,var in enumerate(self._test_arr):
                print("Index = %d , value = %d"%(idx,var))
