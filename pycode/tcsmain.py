#!/usr/bin/python
# -*- coding: UTF-8 -*-

import numpy
from presenter.gtcs import gtcspresenter
from presenter.gtcs import gtcstdd

# Tcs main process.
if __name__ == "__main__":
    # _tcs = gtcspresenter.GtcsPresenter()
    _tcs = gtcstdd.TcsTestDriver(debug_mode=True)
    _tcs.run() 