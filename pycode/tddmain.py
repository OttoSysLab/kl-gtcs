#!/usr/bin/python
# -*- coding: UTF-8 -*-

# from presenter import gtcspresenter as gtcs
from presenter import tddpresenter as gtcs

# The main entry point for the application.
if __name__ == "__main__":
    _tdd = gtcs.GtcsTddPresenter()
    _tdd.tdd_presenter_process()
    # des加密 aes加密   