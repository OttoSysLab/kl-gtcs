#!/usr/bin/python
# -*- coding: UTF-8 -*-

from presenter import gtcspresenter as gtcs

# The main entry point for the application.
if __name__ == "__main__":
    _tk_start  = True
    _gtcs = gtcs.GtcsPresenter(tkstart = _tk_start,main_cycle=0.05)
    _gtcs.gtcs_presenter()
