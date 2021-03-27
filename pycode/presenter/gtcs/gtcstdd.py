#!/usr/bin/python
# -*- coding: UTF-8 -*-

from module import common
from module import crcchecker as crc
import numpy as np
from enum import Enum

class TcsTestDriver:
    # Inital attribute.
    _test_arr = np.zeros((48,),dtype=np.byte)
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

        # debug
        if self._debug_mode == True:
            for idx,var in enumerate(self._test_arr):
                print("Index = %d , value = %d"%(idx,var))
