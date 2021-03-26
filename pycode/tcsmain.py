#!/usr/bin/python
# -*- coding: UTF-8 -*-

import numpy as np
from enum import Enum



_arr = np.zeros((48,),dtype=np.byte)

for idx,var in enumerate(_arr):
    print("Index = %d , value = %d"%(idx,var))

