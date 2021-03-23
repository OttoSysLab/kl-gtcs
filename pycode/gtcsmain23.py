#!/usr/bin/python
# -*- coding: UTF-8 -*-
from presenter import gtcspresenter as gtcs
from platform import system
from time import sleep
import os

# The main entry point for the application.
if __name__ == "__main__":
    _tk_start = True  
    _gtcs = gtcs.GtcsPresenter(tkstart = _tk_start,comport="/dev/ttymxc3")
    _gtcs.gtcs_presenter()