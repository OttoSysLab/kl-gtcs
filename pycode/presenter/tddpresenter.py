#!/usr/bin/python
# -*- coding: UTF-8 -*-
# Present
from presenter import tlgpresenter as tlgp
from presenter import stroagepresenter as sp
from presenter import amspresenter as amsp
from presenter import dbpresenter as gdb
# Module
from module import common as comm
from module import gtcsgpio as gpio
from module import gtcsdata as gdt
from guiview import gtcsview as view
#from os import system as ossys
from platform import system
import os
from time import sleep,perf_counter_ns
# Process & Threading
from threading import Thread
from queue import Queue as tQueue
from multiprocessing import Manager,Queue,Pool,Process
#common
import datetime
import ctypes
import socket
from json import JSONEncoder,JSONDecoder
# import requests
import tkinter as tk

_file_name = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')+".csv"

#region
class GtcsTddPresenter():
    #Initial value.
    __tdd_test = True                        # Set tdd test flag.
    __BAUD_RATES = 115200                    # 
    __poll_delay = 0.1                       # 
    __write_para_delay = 0.02                # 
    __write_flash_delay = 0.05               # 

    __dtime = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')             #utctime
    __path = os.getcwd()+"/logging/"+"TCS_"+__dtime+".csv"                       #f=open(path,'w+')
    __title= ("time,Motor Temp,Max Current,Max Torque,Error code,Polling cycle = "+str(__poll_delay)+"\n")

    __check_sys = system()
    __MEMORY_IOM = ""
    __TEST_IOM = ""

    def __init__(self):
        if self.__check_sys == "Windows":
            self.__COM_PORT = "COM8"                      # 請自行修改序列埠名稱   # 'COM1',"COM3"
            self.__MEMORY_IOM = "tcs_memory.db"           # MEMORY_IOM = "tcs_memory00.db"
            self.__ui  = "swapuicmd.ini"
            self.__mcb = "swapmcbstate.ini"
            self.SWAP_UI_CMD = os.getcwd()+"/resource/ini/"+self.__ui
            self.SWAP_MCB_STATUS = os.getcwd()+"/resource/ini/"+self.__mcb
            self.__TEST_IOM = os.getcwd()+"/resource/db/"+"tcs_test.db"
            self.__TEST_CMD_IOM = os.getcwd()+"/resource/db/"+"tcs_test01.db"
            self.__TEST_ANS_IOM = os.getcwd()+"/resource/db/"+"tcs_test02.db"
            self.__TCS_DATABASE = os.getcwd()+"\\resource\\"+"db\\"+"tcs.db"
            self.__TCS_IOM = os.getcwd()+"\\resource\\"+"db\\"+"tcs_iom.db"
            self.__CMD_IOM = os.getcwd()+"/resource/db/"+"tcs_cmdiom.db"
            self.__ANS_IOM = os.getcwd()+"/resource/db/"+"tcs_ansiom.db"

        elif self.__check_sys == "Linux":
            self.__COM_PORT = "/dev/ttymxc3"               # 請自行修改序列埠名稱
            self.__RAM_DISK = "/mnt/ramdisk"
            self.__MEMORY_IOM ="/mnt/ramdisk/tcs_memory.db" #MEMORY_IOM ="/mnt/ramdisk/tcs_memory00.db"
            self.SWAP_UI_CMD ="/mnt/ramdisk/swapuicmd.ini"
            self.SWAP_MCB_STATUS ="/mnt/ramdisk/swapmcbstates.ini"
            self.__TEST_IOM = "/mnt/ramdisk/tcs_test.db"
            self.__TEST_CMD_IOM = os.getcwd()+"/resource/db/"+"tcs_test01.db"
            self.__TEST_ANS_IOM = os.getcwd()+"/resource/db/"+"tcs_test02.db"
            self.__TCS_DATABASE = "/var/www/html/database/tcs.db"
            self.__TCS_IOM = "/mnt/ramdisk/tcs_iom.db"
            self.__CMD_IOM = "/mnt/ramdisk/tcs_cmdiom.db"
            self.__ANS_IOM = "/mnt/ramdisk/tcs_ansiom.db"

        self.__log_file_name = os.getcwd()+"/logging/"+"Iom_err_"
        self.__log_cmd_iom = os.getcwd()+"/logging/"+"cmd_iom_err_"
        self.__log_ans_iom = os.getcwd()+"/logging/"+"ans_iom_err_"
        self.__csv_title = "time,operating,error_msg \n"
        #region not used
        # print(os.getcwd())
        # print(os.path.abspath(os.path.dirname(__file__)))
        # _ini_buf = "config.ini"
        #endregion

    #region cyclic_start
    # Telegram start
    def Telegram_start(self):
        # Creat TlgPresenter,INIWriter obj.
        _sp = sp.INIWriter()
        _monitor = view.GtcsMonitor()      #print('_tlgp id = %d '%(id(_tlgp)))

        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Config send telegram.
        _cfg = "config.ini"
        _ini_ui = os.getcwd()+"/resource/ini/"+_cfg
        _sp.set_INI_Path(_ini_ui)
        _basic_info = _sp.read_INI_basice_para()
        _basic_info[14] = 2
        _basic_info[15] = 1
        _tlgp.set_TCSP_para(MID ="Basic",plist =_basic_info)
        _ID2_lst = _tlgp.get_TCSP_list(MID='Basic')
        _tlgp.write_MCB_para(MID=2,SID=1,plist=_ID2_lst)

        # Config poll process.
        while True:
            _tlgp.set_comm_ctrl_request(_sp.read_INI_ctrl_tlg())
            _tlgp.poll_to_MCB()
            _monitor.dispaly_status_info()

    # Manual start
    def manual_start(self):
        # Creat TlgPresenter,INIWriter obj.
        _tlgp = tlgp.TlgPresenter()
        _sp = sp.INIWriter()
        _monitor = view.GtcsMonitor()     #print('_tlgp id = %d '%(id(_tlgp)))

        # Config MCB sraial port and delay time.
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Config send telegram.
        _cfg = "config.ini"
        _ini_ui = os.getcwd()+"/resource/ini/"+_cfg
        _sp.set_INI_Path(_ini_ui)
        _basic_info = _sp.read_INI_basice_para()
        _basic_info[14] = 1
        _basic_info[15] = 0
        _tlgp.set_TCSP_para(MID="Basic",plist=_basic_info)
        _ID2_lst=_tlgp.get_TCSP_list(MID='Basic')
        _tlgp.write_MCB_para(MID=2,SID=1,plist=_ID2_lst)

        # Config poll process.
        while True:
            _tlgp.set_comm_ctrl_request(_sp.read_INI_ctrl_tlg())
            _tlgp.poll_to_MCB()
            _monitor.dispaly_status_info()

    # process_start
    def process_start(self):
        # Creat TlgPresenter,INIWriter obj.
        _tlgp = tlgp.TlgPresenter()
        _sp = sp.INIWriter()
        _monitor = view.GtcsMonitor()
        _csv = sp.CsvWriter()
        _csv.wirte_data(filename=self.__path,_str = self.__title)

        # Config MCB sraial port and delay time.
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Config comm ctrl request.
        _cfg = "config.ini"
        _ini_ui = os.getcwd()+"/resource/ini/"+_cfg
        _sp.set_INI_Path(_ini_ui)
        _CSTR_Tlg  = _sp.read_INI_ctrl_tlg()      # read ctr telsgram settimg from ini.
        _CSTR_Tlg[1] = 1                          # Set ctrl telegram to process mode. 
        _CSTR_INFO = _sp.read_INI_cstart_info()
        _idx = 0
        while True:
            _tlgp.set_comm_ctrl_request(plist=_CSTR_Tlg)
            _mrt_stop = _tlgp.cyclic_start_MCB_process(plist=_CSTR_INFO)
            if _mrt_stop == True:
                _lst = _monitor.get_tcsmain_tdd_Info()
                _csv.wirte_data_info(filename=self.__path,sarray=_lst)
            if _idx<9:
                _idx+=1
            else:
                _monitor.dispaly_status_info()
                _idx = 0

    # Self leaning test. 
    def test_self_leaning(self):
        """
        docstring
        """
        # Creat TlgPresenter,INIWriter obj.
        _tlgp = tlgp.TlgPresenter()
        _monitor = view.GtcsMonitor()
        
        # Config MCB sraial port and delay time.
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)
        
        while True:
            _tlgp.SL_MCB_polling()    
            _monitor.dispaly_SL_info()

    #cyclic start
    def cyclic_start(self):
        print("")
        print("Protocol item :")
        print("1.Telegram")
        print("2.Manual")
        print("3.Cycle Start")
        print("4.Test Self leaning")
        print("")
        print("Select Protocol TDD item =",end=" ")
        _p = int(input())
        if _p ==1:self.Telegram_start()
        elif _p==2:self.manual_start()
        elif _p==3:self.process_start()
        elif _p==4:self.test_self_leaning()
    #endregion
     
    # Jay Demand INI.
    def Jay_Demand_INI(self):
        _sp = sp.INIWriter()
        print("")
        print("INI item :")
        print("1.Write default INI.")
        print("2.Read INI process.")
        print("3.Read INI step1.")
        print("4.Read INI step2.")
        print("Select tdd item = ",end="")
        _p = int(input())
        _lst = []
        if _p == 1:_sp.write_default_INI()
        elif _p==2:_lst=_sp.read_INI_process()
        elif _p==3:_lst=_sp.read_INI_step_1()
        elif _p==4:_lst=_sp.read_INI_step_2()
        print(_lst)

    # Jay's demand process.
    def Jay_Demand_Process(self):
        print("")
        print("Jay' Tdd Demand item:")
        print("1.Initial INI file.")
        print("2.Jay's First Demand Process.")
        print("3.Fedtec TDD Process.")
        print("4.Test INI R/W elapsed time.")
        print("Select tdd item = ",end="")
        _p = int(input())
        if _p == 1:
            self.Jay_Demand_INI()
        elif _p==2:pass
        elif _p==3:pass
        elif _p==4:
            t1 = perf_counter_ns()
            _sp = sp.INIWriter()
            _sp.read_INI_process()
            _sp.read_INI_step_1()
            _sp.read_INI_step_2()
            t2 = perf_counter_ns()
            print("Test INI R/W elapsed time =%f (ms)"%((t2-t1)/10000000))
    
    #
    def handle_iom_process(self,pdict:dict)->dict:
        _iom = iom.GtcsIOMPresenter()
        while True:
            t1 = perf_counter_ns()

            _date_time = datetime.datetime.now().strftime('%Y%m%d_%H:%M:%S')
            pdict.update({"date_time":_date_time})

            _read_iom = _iom.read_test_IOM( db_name = self.__TEST_IOM,
                                        table   = "iom",
                                        symbol  = "CMD",
                                        id      = 300)            
            pdict.update({"read_iom":_read_iom}) # Update read dict.

            if _read_iom!="":
                _err_msg = _date_time+","+"R"+","+_read_iom+"\n"
                with open(self.__log_file_name,'a+') as f:
                    f.write(_err_msg)
                    f.flush()

            _update_iom = _iom.update_test_IOM( db_name = self.__TEST_IOM,
                                                table   = "iom",
                                                symbol  = "DATA",
                                                id      = 300)

            pdict.update({"update_iom":_update_iom})

            if _update_iom != "" :
                _err_msg = datetime.datetime.now().strftime('%Y%m%d_%H:%M:%S')+","+"W"+","+_update_iom+"\n"
                with open(self.__log_file_name,'a+') as f:
                    f.write(_err_msg)
                    f.flush()

            t2 = perf_counter_ns()

            # if ((_read_iom=="")&(_update_iom == "")):
            #     print("R/W IOM OK! "+_date_time)                
            # print("Test DB R/W elapsed time =%f (ms)"%((t2-t1)/1000000))
            pdict.update({"read_iom" :_read_iom,
                        "update_iom":_update_iom,
                        "date_time":_date_time,
                        "pre_time":t2-t1})

            sleep(0.1)

    # one IOM process.
    def one_IOM_Verification(self):
        # _iom = db.GtcsIOMPresenter(db_Path=self.__TEST_IOM,table = "iom",cycle=0.1)
        # _iom = gdb.GtcsDBPresenter()
        # Select porcessitem.
        print("Start verification one db swap lockd!")
        self.__log_file_name += datetime.datetime.now().strftime('%Y%m%d_%H%M%S')+".csv"
        with open(self.__log_file_name,'a+') as f:
            f.write(self.__csv_title)
        _date_time = datetime.datetime.now().strftime('%Y%m%d_%H:%M:%S')

        # Initial mgr dict.
        _mgr  = Manager()
        _dict_main = _mgr.dict()
        _dict_main.update({"read_iom" :"",
                        "update_iom":"",
                        "date_time":_date_time,
                        "pre_time":0})

        _p_iom = Process(target=self.handle_iom_process, args=(_dict_main,))           # Intial MCB process.
        _p_iom.start()

        while True:
            if ((_dict_main.get("read_iom")=="")&(_dict_main.get("update_iom") == "")):
                print("R/W IOM OK! "+_dict_main.get("date_time"))
                print("Test DB R/W elapsed time =%f (ms)"%((_dict_main.get("pre_time"))/1000000))
            else:
                print("read_iom = %s" %(_dict_main.get("read_iom")))
                print("update_iom = %s" %(_dict_main.get("update_iom")))
            sleep(0.2)
        _p_iom.join()        

    # one IOM process.
    def two_IOM_Verification(self):
        t1 = perf_counter_ns()
        _iom_cmd = iom.GtcsIOMPresenter()
        _iom_ans = iom.GtcsIOMPresenter()
        # Select porcessitem.
        print("Start verification one db swap lockd!")
        self.__log_cmd_iom += datetime.datetime.now().strftime('%Y%m%d_%H%M%S')+".csv"
        self.__log_ans_iom += datetime.datetime.now().strftime('%Y%m%d_%H%M%S')+".csv"
        # Creat an s table.
        _iom_ans.create_test_ansiom_table(db_name =self.__TEST_ANS_IOM ,table = "iomans")
        # Get cm iom length.
        _len = _iom_cmd.get_table_length(db_name = self.__TEST_CMD_IOM,table = "iomcmd")
        # print(_len)
        for idx in range(_len+1):
            if idx ==0:
                print(idx,end = " ")
                print(_iom_ans.insert_test_IOM( db_name = self.__TEST_ANS_IOM,
                                                table   = "iomans",
                                                symbol  = "REQ",
                                                id      = 300,
                                                state   = 0,
                                                data    = ""))
            elif idx ==1:
                print(idx,end = " ")
                print(_iom_ans.insert_test_IOM( db_name = self.__TEST_ANS_IOM,
                                                table   = "iomans",
                                                symbol  = "REQ",
                                                id      = 301,
                                                state   = 0,
                                                data    = ""))
            elif idx ==2:
                print(idx,end = " ")
                print(_iom_ans.insert_test_IOM( db_name = self.__TEST_ANS_IOM,
                                                table   = "iomans",
                                                symbol  = "DATA",
                                                id      = 300,
                                                state   = 0,
                                                data    = ""))                                 
            elif idx <_len:
                print(idx,end = " ")
                print(_iom_ans.insert_test_IOM( db_name = self.__TEST_ANS_IOM,
                                                table   = "iomans",
                                                symbol  = "ANS",
                                                id      = 300+idx-2,
                                                state   = 0,
                                                data    = ""))
        t2 = perf_counter_ns()           
        print("Test DB R/W elapsed time =%f (ms)"%((t2-t1)/10000000))
        # with open(self.__log_cmd_iom,'a+') as f:f.write(self.__csv_title)
        # with open(self.__log_ans_iom,'a+') as f:f.write(self.__csv_title)

        # while True:
        #     t1 = perf_counter_ns()
        #     _date_time = datetime.datetime.now().strftime('%Y%m%d_%H:%M:%S')

        #     # CMD
        #     _read_iom = _iom_cmd.read_IOM(symbol= "CMD",id = 300)
        #     if _read_iom!="":
        #         _err_msg = _date_time+","+"R"+","+_read_iom+"\n"
        #         with open(self.__log_cmd_iom,'a+') as f:
        #             f.write(_err_msg)
        #             f.flush()
        #         print("Read CMD_IOM OK! "+_date_time)

        #     # ANS
        #     _update_iom = _iom_ans.update_IOM(symbol = "ANS",id = 300)
        #     if _update_iom != "" :
        #         _err_msg = _date_time+","+"W"+","+_update_iom+"\n"
        #         with open(self.__log_ans_iom,'a+') as f:
        #             f.write(_err_msg)
        #             f.flush()
        #         print("Write AMS_IOM OK! "+_date_time)

        #     t2 = perf_counter_ns()                
        #     print("Test DB R/W elapsed time =%f (ms)"%((t2-t1)/10000000))
        #     sleep(0.1)

    # Read mcb info
    def read_MCB_info(self):
        t1 = perf_counter_ns()
        # Initial path mane.
        _ui  = "swapuicmd.ini"
        _mcb = "swapmcbstate.ini"
        _ini_ui = os.getcwd()+"/resource/"+_ui
        _ini_mcb = os.getcwd()+"/resource/"+_mcb

        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # R/W INI.
        _ini = sp.INIWriter()
        _d_mcb = _tlgp.read_MCB_start_para()
        # Write data to INI file.
        _json = _ini.convert_dict_to_json(_d_mcb)
        _ini.write_json_to_INI(_ini_mcb,_json)

        t2 = perf_counter_ns()
        print("R/W INI perf counter = %f (ms)"%((t2-t1)/1000000))

    # region polling_socket_threading
    # MCB polling.
    def polling_MCB_Thread(self,_q):
        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Config send telegram.
        _sp = sp.INIWriter()
        _basic_info = _sp.read_INI_basice_para()
        _basic_info[14] = 1
        _basic_info[15] = 0
        _tlgp.set_TCSP_para(MID="ID2",plist=_basic_info)
        _ID2_lst=_tlgp.get_TCSP_list(MID='ID2')
        _tlgp.write_MCB_para(MID=2,SID=1,plist=_ID2_lst)

        # Config poll process.
        _tlgp.set_comm_ctrl_request(_sp.read_INI_ctrl_tlg())
        while True:
            _obj = _tlgp.poll_to_MCB()
            _q.put(_obj)

    # check_json
    def check_json(self,myjson):
        json_object = None
        _JD = JSONDecoder()
        try:
            # json_object = json.loads(myjson)
            json_object = _JD.decode(myjson)
        except ValueError as e:
            print(e)
        finally:
            return json_object

    # Handle sokcet event.
    def handle_socket_server_event(self,_q):
        #import socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(('127.0.0.1', 9000))
        sock.listen(5)
        _tcsp = None
        _JD = JSONDecoder()
        _JE = JSONEncoder()
        while True:
            connection,address = sock.accept()
            _tcsp = _q.get()
            try:
                connection.settimeout(5)
                buf = connection.recv(1024)
                # _s = json.loads(buf.decode("utf-8"))
                _s = _JD.decode(buf.decode("utf-8"))
                if ((int(_s["PageNum"])==1)&(int(_s["PageCmd"])==2)):
                    #_tcsp = tlg.TlgStructure()
                    _data = {  "u16ActStatus"    : str(_tcsp.status_response.u16ActStatus),           # [0,0]
                            "u16ActError"     : str(_tcsp.status_response.u16ActError),           # [9,1]
                            "u16ActProNr"     : str(_tcsp.status_response.u16ActProNr),           # [9,3]
                            "u16ActStepID"    : str(_tcsp.status_response.u16ActStepID),          # [9,4]
                            "u16ActCurr"      : str(_tcsp.status_response.u16ActCurr),            # [9,8]
                            "u16ActTorque"    : str(_tcsp.status_response.u16ActTorque),          # [9,8]
                            "u16ActRPM"       : str(_tcsp.status_response.u16ActRPM),             # [9,8]
                            "u16MaxCurrent"   : str(_tcsp.status_response.u16MaxCurrent),         # [9,12]
                            "u16MaxTorque"    : str(_tcsp.status_response.u16MaxTorque),          # [9,13]
                            "u32Angle"        : str(_tcsp.status_response.u32Angle),              # [9,13]
                            "u32Revolutions"  : str(_tcsp.status_response.u32Revolutions),        # [9,13]
                            "u8TMDStatus"     : str(_tcsp.status_response.u8TMDStatus),           # [9,13]
                            "s16Debug"        : str(_tcsp.status_response.s16Debug),              # [9,13]
                    }
                    _s_dump = _JE.encode(_data)
                connection.send(_s_dump.encode('utf-8'))
            except socket.timeout:
                pass
                #print('time out')
            sleep(0.001)
        connection.close()

    # Handle sokcet event.
    def handle_socket_client_event(self,_q):
        #import socket
        _cnt =0
        while(True):
            try:
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sock.connect(('127.0.0.1', 9000))
                sock.send('Client snd data to server....'.encode('utf-8'))
                str = sock.recv(1024)
                print(str.decode("utf-8"))
                _cnt+=1
                print("connect cnt = %d"%(_cnt))
                sock.close()
            except Exception as e:
                print(e)
                _cnt = 0
            sleep(0.5)

    # Post http requets to apache server.
    def http_post_to_UI(self,_q):
        #import socket
        server_url = "http://localhost/kls/gtcs/tcshttpserver.php"
        headers = {'Content-Type': 'application/json'}
        #headers = {'Content-Type': 'text/html'}
        _cnt =0
        _JD = JSONDecoder()
        _JE = JSONEncoder()
        # _t1 = datetime.datetime.now().strftime()
        while True:
            _tcsp = _q.get()
            _data_1 = {  "u16ActStatus"    : str(_tcsp.status_response.u16ActStatus),         # [0,0]
                    "u16ActError"     : str(_tcsp.status_response.u16ActError),           # [9,1]
                    "u16ActProNr"     : str(_tcsp.status_response.u16ActProNr),           # [9,3]
                    "u16ActStepID"    : str(_tcsp.status_response.u16ActStepID),          # [9,4]
                    "u16ActCurr"      : str(_tcsp.status_response.u16ActCurr),            # [9,8]
                    "u16ActTorque"    : str(_tcsp.status_response.u16ActTorque),          # [9,8]
                    "u16ActRPM"       : str(_tcsp.status_response.u16ActRPM),             # [9,8]
                    "u16MaxCurrent"   : str(_tcsp.status_response.u16MaxCurrent),         # [9,12]
                    "u16MaxTorque"    : str(_tcsp.status_response.u16MaxTorque),          # [9,13]
                    "u32Angle"        : str(_tcsp.status_response.u32Angle),              # [9,13]
                    "u32Revolutions"  : str(_tcsp.status_response.u32Revolutions),        # [9,13]
                    "u8TMDStatus"     : str(_tcsp.status_response.u8TMDStatus),           # [9,13]
                    "s16Debug"        : str(_tcsp.status_response.s16Debug),              # [9,13]
            }
            _packet_dump = _JE.encode(_data_1)        # First Json package.
            _data_2 = {
                "CmdSide":"MCB",                      # Post form MCB side.
                "Packet":_packet_dump
            }
            _post_dump = _JE.encode(_data_2)          # Second Json package.

            try:
                # timeout except = 0.1
                # timeout except = 0.11
                _post_response = requests.post(url=server_url,headers=headers,data=_post_dump,timeout=0.5)
                if _post_response.status_code == requests.codes.ok:
                    _json_obj = self.check_json(_post_response.text)
                    if _json_obj!=None:    # Check responst cmd formate.
                        print(_json_obj)
                    else:
                        #print("Post response is not json formate.")
                        _cnt +=1
                        print('lose packate CNT = %d'%(_cnt))
            except Exception as e:
                print(e)
            sleep(0.1)

    # Socket thread polling.
    def polling_socket_threading(self):
        _q_tlg = tQueue()                                                   # 使用 queue 接收 function 的回傳值
        _t_mcb = Thread(target=self.polling_MCB_Thread, args=(_q_tlg,))          # 特別注意 這邊的傳入參數只有一個的話，後面要有逗號
        _t_socket = Thread(target=self.handle_socket_server_event, args=(_q_tlg,))      # 特別注意 這邊的傳入參數只有一個的話，後面要有逗號
        #_t_socket = Thread(target=handle_socket_client_event, args=(_q_tlg,))      # 特別注意 這邊的傳入參數只有一個的話，後面要有逗號
        #_t_socket = Thread(target=http_post_to_UI, args=(_q_tlg,))         # 特別注意 這邊的傳入參數只有一個的話，後面要有逗號

        _t_mcb.start()
        #sleep(1)
        _t_socket.start()
        _t_mcb.join()
        _t_socket.join()

    #Polling_socket_thread
    # Copy Database to memory.
    def initial_UI_Package(self):
        # Initial database.
        _check_sys = system()
        if _check_sys == 'Windows':
            pass
        elif _check_sys == 'Linux':
            os.system('cp /var/www/html/database/tcs_memory.db /mnt/ramdisk/tcs_memory.db')
            os.system('chmod 777 -R /mnt/ramdisk/tcs_memory.db')
            os.system('cp /var/www/html/database/tcs_memory00.db /mnt/ramdisk/tcs_memory00.db')
            os.system('chmod 777 -R /mnt/ramdisk/tcs_memory00.db')
            os.system('cp /var/www/html/database/tcs_exchange.db /mnt/ramdisk/tcs_exchange.db')
            os.system('chmod 777 -R /mnt/ramdisk/tcs_exchange.db')
            os.system('cp /var/www/html/database/tcs_exchange2.db /mnt/ramdisk/tcs_exchange2.db')
            os.system('chmod 777 -R /mnt/ramdisk/tcs_exchange2.db')
            os.system('cp /var/www/html/database/tcs_data.db /mnt/ramdisk/tcs_data.db')
            os.system('chmod 777 -R /mnt/ramdisk/tcs_data.db')
            os.system('/home/kls/project/main23')

    # MCB polling.
    def polling_MCB_Process(self,_dtlg,_dcmd):
        # Creat TlgPresenter,INIWriter obj.
        _tlgp = tlgp.TlgPresenter()
        _sp = sp.INIWriter()

        # Config MCB sraial port and delay time.
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Config send telegram.
        _basic_info = _sp.read_INI_basice_para()
        _basic_info[14] = 1
        _basic_info[15] = 0
        _tlgp.set_TCSP_para(MID="ID2",plist=_basic_info)
        _ID2_lst=_tlgp.get_TCSP_list(MID='ID2')
        _tlgp.write_MCB_para(MID=2,SID=1,plist=_ID2_lst)

        # Config poll process.
        _tlgp.set_comm_ctrl_request(_sp.read_INI_ctrl_tlg())
        while True:
            _obj = _tlgp.poll_to_MCB()
            _dtlg["u16ActStatus"]    = _obj.status_response.dict["u16ActStatus"]      # [0,0]
            _dtlg["u16ActError"]     = _obj.status_response.dict["u16ActError"]       # [9,1]
            _dtlg["u16ActProNr"]     = _obj.status_response.dict["u16ActProNr"]       # [9,3]
            _dtlg["u16ActStepID"]    = _obj.status_response.dict["u16ActStepID"]      # [9,4]
            _dtlg["u16ActCurr"]      = _obj.status_response.dict["u16ActCurr"]        # [9,8]
            _dtlg["u16ActTorque"]    = _obj.status_response.dict["u16ActTorque"]      # [9,8]
            _dtlg["u16ActRPM"]       = _obj.status_response.dict["u16ActRPM"]         # [9,8]
            _dtlg["u16MaxCurrent"]   = _obj.status_response.dict["u16MaxCurrent"]     # [9,12]
            _dtlg["u16MaxTorque"]    = _obj.status_response.dict["u16MaxTorque"]      # [9,13]
            _dtlg["u32Angle"]        = _obj.status_response.dict["u32Angle"]          # [9,13]
            _dtlg["u32Revolutions"]  = _obj.status_response.dict["u32Revolutions"]    # [9,13]
            _dtlg["u8TMDStatus"]     = _obj.status_response.dict["u8TMDStatus"]       # [9,13]
            _dtlg["s16Debug"]        = _obj.status_response.dict["s16Debug"]          # [9,13]
            _dtlg["s32Debug"]        = _obj.status_response.dict["s32Debug"]          # [9,13]

    # Handle sokcet event.
    def handle_Process_event(self,_dtlg,_dcmd):
        #import socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(('127.0.0.1', 9999))
        sock.listen(5)
        _tcsp = None
        _JD = JSONDecoder()
        _JE = JSONEncoder()
        while True:
            connection,address = sock.accept()
            #_tcsp = _q.get()
            try:
                connection.settimeout(5)
                buf = connection.recv(1024)
                _s = _JD.decode(buf.decode("utf-8"))
                if((int(_s["PageNum"])==1)&(int(_s["PageCmd"])==2)):
                    _data = [{  
                        "u16ActStatus"    : _dtlg["u16ActStatus"],               # [0,0]
                        "u16ActError"     : _dtlg["u16ActError"],                # [9,1]
                        "u16ActProNr"     : _dtlg["u16ActProNr"],                # [9,3]
                        "u16ActStepID"    : _dtlg["u16ActStepID"],               # [9,4]
                        "u16ActCurr"      : _dtlg["u16ActCurr"],                 # [9,8]
                        "u16ActTorque"    : _dtlg["u16ActTorque"],               # [9,8]
                        "u16ActRPM"       : _dtlg["u16ActRPM"],                  # [9,8]
                        "u16MaxCurrent"   : _dtlg["u16MaxCurrent"],              # [9,12]
                        "u16MaxTorque"    : _dtlg["u16MaxTorque"],               # [9,13]
                        "u32Angle"        : _dtlg["u32Angle"],                   # [9,13]
                        "u32Revolutions"  : _dtlg["u32Revolutions"],             # [9,13]
                        "u8TMDStatus"     : _dtlg["u8TMDStatus"],                # [9,13]
                        "s16Debug"        : _dtlg["s16Debug"],                   # [9,13]
                    }]
                _s_dump = _JE.encode(_data)
                _REQ_s = _s_dump.replace('[','').replace(']','')
                connection.send(_REQ_s.encode('utf-8'))
            except socket.timeout:
                pass
            sleep(0.001)
        connection.close()

    #  Socket process polling.
    def polling_socket_process(self):
        mgr = Manager()
        _d_tlg = mgr.dict()
        _d_cmd = mgr.dict()
        #_p_os =  Process(target=initial_UI_Package)
        _p_mcb = Process(target=self.polling_MCB_Process, args=(_d_tlg,_d_cmd))    # 特別注意 這邊的傳入參數只有一個的話，後面要有逗號
        _p_socket = Process(target=self.handle_Process_event, args=(_d_tlg,_d_cmd))# 特別注意 這邊的傳入參數只有一個的話，後面要有逗號

        # Start Process.
        #_p_os.start()
        _p_mcb.start()
        sleep(1)
        _p_socket.start()
        # Join Process.
        #_p_os.join()
        _p_mcb.join()
        _p_socket.join()

    #Polling_ini_thread
    # Read_INI_save_info
    def read_INI_save_info(self):
        t1 = perf_counter_ns()                     # Set t1 prerf counter.
        # Initial tlg object.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Set file path.
        _ini_ui = os.getcwd()+"/resource/ini/"+"config.ini"
        _ini_mcb = self.SWAP_MCB_STATUS

        # Read frome INI.
        _ini = sp.INIWriter()
        _read_data = _ini.read_INI_to_json(_ini_ui)
        _dict = _ini.convert_json_to_dict(_read_data)

        # Get page number and command.
        _page = _dict.get("Page")
        if _page["PageNo"]==1:
            if _page["PageCmd"]==1:
                _d = _tlgp.read_MCB_start_para()

        # Write data to INI file.
        _json = _ini.convert_dict_to_json(_d)
        _ini.write_json_to_INI(_ini_mcb,_json)
        t2 = perf_counter_ns()
        print("R/W INI perf counter = %f (ms)"%((t2-t1)/1000000))
    #endregion

    # Auto read test 
    
    # Read TMD info.
    def read_TMD_info(self):
        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)
        
        # Set read addr.
        print("Read TMD addr = ",end="")
        _addr = int(input())
        # Read data from TMD flash.
        _value  = _tlgp.read_TMD_flash(addr=_addr)
        print("Read TMD addr = %d,value = %d"%(_addr,_value))    
        return _value 

    # Write data to TMD.
    def write_TMD_info(self):
        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # Set TMD write flash addr & value
        print("Write TMD add = ",end="")
        _addr = int(input())
        print("Write TMD value = ",end="")
        _value = int(input())

        # Read data from TMD flash.
        _tlgp.write_TMD_flash(addr=_addr,value=_value)
        print("Write TMD addr = %d,value = %d"%(_addr,_value))           
    
    # TMD  flash test.
    def TMD_flash_test(self):
        print("Test TMD Read / Write process.")
        print("1.Read data from TMD.")
        print("2.Write data to TMD.")
        print("Please select test process:",end="")
        _select = int(input())
        if _select == 1:
            self.read_TMD_info()
        else:
            self.write_TMD_info()

    # TMD test process.
    def TMD_flash_autotest(self):
        # Config MCB sraial port and delay time.
        _tlgp = tlgp.TlgPresenter()
        _tlgp.initial_MCB_serial(MCBcomm=self.__COM_PORT,MCBbaudrate=self.__BAUD_RATES)
        _tlgp.set_MCB_poll_delay(self.__poll_delay)
        _tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        _tlgp.set_MCB_write_para_delay(self.__write_para_delay)

        # 寫目前的測試address到檔案
        _PATH = os.getcwd()+"/log/TMD_test.csv"
        _addr = 0
        _addr_max = 4000
        _value = " "  
        with open(_PATH, 'a+') as f:f.write("Time"+','+"Address"+','+"Value"+'\n')   

        for idx in range(_addr_max):
            _time  = datetime.datetime.now().strftime('%Y-%m-%d_%H:%M:%S')
            _addr  = idx      
            _value = _tlgp.read_TMD_flash(addr=idx)
            print(_value)
            with open(_PATH, 'a+') as f:
                f.write(str(_time)+','+''+str(_addr)+','+str(_value)+'\n')     
            sleep(0.1)
        
    #Presenter process.
    def tdd_presenter_process(self):
        # #region About test.
        if self.__tdd_test ==True:
            print("TDD item :")
            print("1.Protocol Verification")
            print("2.One IOM Verification")
            print("3.Two IOM data swap process")
            print("4.Jay's Demand Process")
            print("5.Read INI setting.")
            print("6.Read MCB information.")
            print("7.Write MCB infromation.")
            print("8.TMD test flash.")
            print("9.TMD flash autotest.")
            print("Select TDD item = ",end="")
            _p = int(input())
            if _p == 1:self.cyclic_start()
            elif _p==2:self.one_IOM_Verification()
            elif _p==3:self.two_IOM_Verification()
            elif _p==4:self.Jay_Demand_Process()
            elif _p==5:self.read_INI_save_info()
            elif _p==6:self.read_MCB_info()
            elif _p==7:self.read_MCB_info()
            elif _p==8:self.TMD_flash_test()
            elif _p==9:self.TMD_flash_autotest()
        else:
            self.polling_socket_process()                  # 200528 # 202602
            self.polling_socket_threading()                # 200529
#endregion
