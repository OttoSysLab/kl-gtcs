#!/usr/bin/python
# -*- coding: UTF-8 -*-
from presenter import gtcspresenter as gtcs
from module import comport

# The main entry point for the application.
if __name__ == "__main__":
    _tk_start = False   
    _comm = comport.MCBComm()
    _port = _comm.select_MCB_port()
    _gtcs = gtcs.GtcsPresenter(tkstart = _tk_start,comport=_port)
    # _gtcs = gtcs.GtcsPresenter(tkstart = _tk_start,comport="/dev/ttymxc3")
    _gtcs.gtcs_presenter()
    