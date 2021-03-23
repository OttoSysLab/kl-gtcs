#!/usr/bin/python
# -*- coding: UTF-8 -*-    
#####################################################################
# 
# 
#  
# ###################################################################
try:

    import sqlite3 
    from enum import Enum 
    from sqlite3 import Error 
except Exception as e :print(e)

import os
from time import sleep 
from datetime import datetime as dt

def main():
    _ans_iom = "/mnt/ramdisk/tcs_ansiom.db"
    _cmd_iom = "/mnt/ramdisk/tcs_cmdiom.db"
    while True:
        _check_ans_iom = os.path.isfile(_ans_iom)
        _check_cmd_iom = os.path.isfile(_cmd_iom)
        if (_check_ans_iom==True)&(_check_cmd_iom==True):
            os.system('php /var/www/html/tcs10/gateway/gtcs_gateway.php')
            break
        sleep(0.1)

if __name__ == "__main__":
    main()

