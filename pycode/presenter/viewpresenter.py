#!/usr/bin/python
# -*- coding: UTF-8 -*-
from time import sleep,perf_counter_ns
from module import gtcsgpio as gpio
from guiview import gtcsview as view
import os 
import json

#region Gtcs GPIO Presenter.
class GtcsViewPresenter():
    __view = view.GtcsMonitor()
    def __init__(self):
        pass
#endregion