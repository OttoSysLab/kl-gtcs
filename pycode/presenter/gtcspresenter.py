#!/usr/bin/python
# -*- coding: UTF-8 -*-
# Present
from presenter import tlgpresenter as tlgp
from presenter import stroagepresenter as sp
from presenter import amspresenter as amsp
from presenter import dbpresenter as db
from presenter import uipresenter as ui
from presenter import rs232presenter as rs232
from presenter import rs485presenter as rs485
from presenter import gpiopresenter as gpio
# module
from module import common as comm
from module import amspotocol as ams
from module import gtcsdata as gdt
from module import logging as log
# view
from guiview import gtcsview as view
# other
#from os import system as ossys
from platform import system
import os
from time import sleep,perf_counter_ns
# from threading import Thread
# from queue import Queue as tQueue
from multiprocessing import Manager,Queue,Pool,Process
#common
import datetime
import ctypes
import socket
import select
from json import JSONEncoder,JSONDecoder
from enum import Enum

#region Gtcs Presenter lcass.
class GtcsPresenter():
    #region Private Attribute.
    __dtime = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')               # utctime
    __path = "TCS_"+__dtime+".csv"                                            # f=open(path,'w+')
    __file_name = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')+".csv"

    __check_sys = system()
    __BAUD_RATES = 115200
    __poll_delay =  0.02
    __write_para_delay  = 0.05 # 0.02
    __write_flash_delay = 0.1

    __UI_SOCKET_IP =  "127.0.0.1"  #"192.168.0.183"
    __UI_SOCKET_PORT = 9000

    __LAN_SOCKET_IP =  "127.0.0.1" # "192.168.0.207" #"192.168.0.183"
    __LAN_SOCKET_PORT = 9999

    __RS485_COM = "/dev/ttymxc1"
    __RS485_BAUDRATE = 115200
    __RS232_COM = "/dev/ttymxc2"
    __RS232_BAUDRATE = 115200
    # mcb status
    __current_mcb_status = dict()
    __last_mcb_status  = dict()
    __tool_max_troque = 5
    __last_ui_cmd = dict()
    __log = log.log()
    # __gtcs_bulletin = gdt.GtcsBulletin()
    __check_DB = False
    
    # __sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   # UI tcp socket.    
    #endregion

    #region mulitprocess.
    __p_main23_start  = False        # 
    __p_tcp_ui_start  = False        # 
    __p_tcp_lan_start = False        # 
    __p_rs232_start   = False        # 
    __p_rs485_start   = False        # 
    __p_gpio_start    = False        # 
    __p_storage_start = False        # 
    __p_view_start    = False        # 
    #endregion

    # region Initial object.
    def __init__(self,tkstart,comport:str):
        self.__main23_start = tkstart
        _check_sys = system()
        if _check_sys == "Windows":
            self.__COM_PORT = comport                      # 請自行修改序列埠名稱   # 'COM1',"COM3"
            self.__SWAP_UI_CMD = os.getcwd()+"\\resource\\"+"ini\\swapuicmd.ini"
            self.__SWAP_MCB_STATUS = os.getcwd()+"\\resource\\"+"ini\\swapmcbstate.ini"
            self.__USB_1 ="otto1.csv"
            self.__USB_2 ="otto2.csv"
            self.__TCS_DATABASE = os.getcwd()+"\\resource\\"+"db\\"+"tcs.db"
            self.__TCS_IOM = os.getcwd()+"\\resource\\"+"db\\"+"tcs_iom.db"
            self.__CMD_IOM = os.getcwd()+"/resource/db/"+"tcs_cmdiom.db"
            self.__ANS_IOM = os.getcwd()+"/resource/db/"+"tcs_ansiom.db"
        elif _check_sys == "Linux":
            self.__COM_PORT = "/dev/ttymxc3"               # 請自行修改序列埠名稱
            self.__RAM_DISK = "/mnt/ramdisk"
            self.__SWAP_UI_CMD ="/mnt/ramdisk/swapuicmd.ini"
            self.__SWAP_MCB_STATUS ="/mnt/ramdisk/swapmcbstate.ini"
            self.__USB_1 ="/media/kls/otto1/otto1.csv"
            self.__USB_2 ="/media/kls/otto2/otto2.csv"
            self.__TCS_DATABASE = "/var/www/html/database/tcs.db"
            self.__TCS_IOM = "/mnt/ramdisk/tcs_iom.db"
            self.__CMD_IOM = "/mnt/ramdisk/tcs_cmdiom.db"
            self.__ANS_IOM = "/mnt/ramdisk/tcs_ansiom.db"
        
        # Initial sub presenter
        self.__tlgp = tlgp.TlgPresenter()
        self.__tlgp.set_MCB_poll_delay(self.__poll_delay)
        self.__tlgp.set_MCB_write_flash_delay(self.__write_flash_delay)
        self.__tlgp.set_MCB_write_para_delay(self.__write_para_delay)
        self.__amsp = amsp.GtcsAMSPresenter()
        self.__comm = comm.CommonFunction()

        # Initail obj for tcs db.
        self.__gdb = db.GtcsDBPresenter()
        self.__gdb.set_tcs_db_name(self.__TCS_DATABASE)
        self.__gdb.set_tcs_db_table("basic")
        # Initial obj for tcs iom.
        self.__ui = ui.GtcsUIPresenter()
        # Mulitprocess.
        self.__turn_on_process()
    #endregion

    # Select whitch process should be turn on.
    def __turn_on_process(self):
        """
        docstring
        """
        self.__p_main23_start   = self.__main23_start
        self.__p_tcp_ui_start   = True
        # self.__p_tcp_lan_start   = True
        # self.__p_rs232_start   = True
        # self.__p_rs485_start   = True
        # self.__p_gpio_start    = True
        # self.__p_storage_start = True
        # self.__p_view_start    = False
        
    #region Gtcs mulitprocess.
    # main23
    def __init_main23(self,q):
        try:
            os.system('/home/kls/project/main23')
        except Exception as e:
            self.__log.error(msg = "Initial main23 error ,msg = %s"%(e))

    # UI tcp socket process.
    def handle_tcp_ui_process(self,pdict:dict):
        # Initial amsp obj
        _amsp  = amsp.GtcsAMSPresenter()
        _JE = JSONEncoder()
        # Initial 
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        _check_sys = system()
        if _check_sys == 'Linux':  
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        sock.bind((self.__UI_SOCKET_IP,self.__UI_SOCKET_PORT))  
        # sock.setblocking(False)      # Set socket mode to NON-Blocking.
        sock.listen(5)
        # sock.listen()
        # inputs = [sock, ]
        print("TCP ui server IP = %s ,PORT = %s" %(self.__UI_SOCKET_IP,self.__UI_SOCKET_PORT))
        print("Tcp ui server start...")

        # While loop.
        while True:  
            try:
                connection,address = sock.accept()
                connection.settimeout(5)
                # Received data and decode.
                _tcp_rev = connection.recv(1024).decode("utf-8")
                _ui_cmd_dict = _amsp.decode_ams_dict(pstr=_tcp_rev)
                # Update dictionary.
                pdict.update({gdt.PROCESS.UI_C.value:{
                    "cmd_id":_ui_cmd_dict.get("str1"),
                    "data"  :_ui_cmd_dict
                }})
                self.__log.debug(_JE.encode(pdict[gdt.PROCESS.UI_C.value]))
                # Resend Ans.
                if pdict.get(gdt.PROCESS.UI_S.value)!="":      
                    _dev_req = pdict.get(gdt.PROCESS.UI_S.value).get("data")                
                    connection.send(_dev_req.encode('utf-8'))
                    self.__log.debug(_dev_req)
            except socket.timeout:
                self.__log.error("Handle tcp ui process ip %s connect time out!"%(str(address)))
            except Exception as e:
                self.__log.error("Handle tcp ui process err msg = %s"%(e))
            connection.close()
            
    # Lan tcp socket process.
    def handle_tcp_lan_process(self,pdict:dict):
         # Initial amsp obj
        _amsp  = amsp.GtcsAMSPresenter()
        # Initial 
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind((self.__LAN_SOCKET_IP,self.__LAN_SOCKET_PORT))  
        sock.listen(5)
        print("TCP lan server ip = %s ,port = %s" %(self.__LAN_SOCKET_IP,self.__LAN_SOCKET_PORT))
        print("Tcp lan server start...")

        # While loop.
        while True:  
            connection,address = sock.accept()
            try:
                connection.settimeout(5)
                _tcp_rev = connection.recv(1024).decode("utf-8")
                _ui_cmd = _amsp.decode_ams_dict(pstr=_tcp_rev)
            except socket.timeout:
                # log.error("Handle tcp ui process ip %s connect time out!"%(address))
                print("Handle tcp ui process ip %s connect time out!"%(address))
            except Exception as e:
                # log.error("Handle tcp ui process err msg = %s"%(e))
                print("Handle tcp ui process err msg = %s"%(e))
            connection.close()
            sleep(0.01)
    #endregion

    #region gctsdriver.py
    # Update initial dict.
    def __update_init_dict(self,pdict:dict)->dict:
        _dict = dict()
        # Main FSM.
        _dict.update({gdt.PROCESS.M_Ready.value:"Stop"})
        _dict.update({gdt.PROCESS.M_FSM.value  :"StartSYS"})
        # MCB.
        _dict.update({gdt.PROCESS.MCB.value:{
            "cmd_id":gdt.AMSCmd.CMD_R_MCB_BASIC.value,
            "data":""
        }})
        # UI cmd.
        _dict.update({gdt.PROCESS.UI_C.value:{"cmd_id":"","data":""}})
        # UI status.
        _dict.update({gdt.PROCESS.UI_S.value:{"cmd_id":"","data":""}})
        # DB.
        _dict.update({gdt.PROCESS.DB.value:{
            "cmd_id":gdt.AMSCmd.CMD_R_DB_BASIC.value,
            "data"  :""
        }})
        # # SD.
        # _dict.update({gdt.PROCESS.SD.value:{"cmd_id":"","data":""}})
        # # TKINTER VIEW.
        # _dict.update({gdt.PROCESS.VIEW.value:{"cmd_id":"","data":""}})
        return _dict

    # Initial gtcs system.
    def __start_system(self,pdict:dict)->dict:
        # Open ttymxc3 comport.
        while(self.__tlgp.open_comm==False):  # Chekc commprt connect state.
            self.__tlgp.open_comm = self.__tlgp.initial_MCB_serial( MCBcomm=self.__COM_PORT,
                                                        MCBbaudrate=self.__BAUD_RATES)
            sleep(0.1)
        # Get first polling status.
        self.__log.debug("Open MCB com = %s ,port = %s"%(self.__COM_PORT, self.__BAUD_RATES))

        # Get first tlg stautus.
        self.__tlgp.disable_screwdriver()
        _ctrl_tlg = self.__tlgp.get_MCB_ctrl_tlg(direction = gdt.Dircetion.FASTEN.value)
        _mcb_status = self.__tlgp.get_MCB_polling_status(ctrl_dict=_ctrl_tlg)
        self.__amsp.set_ams_last_flags(flags_value=_mcb_status[gdt.StatusTlgEnum.STATUS_FLAGS.value])
        self.__amsp.set_tool_max_torque(torque=self.__tool_max_troque)

        # Jump m_FSM to Check Initial.
        pdict[gdt.PROCESS.M_FSM.value] = gdt.MSTATUS.CHECK.value
        return pdict

    # Initial gtcs system.
    def __check_basic_para(self,pdict:dict)->dict:
        _dict = dict()
        # Disable MCB.
        self.__tlgp.disable_screwdriver()

        # Read basic parameter from database.
        _db_basic_dict = self.__gdb.get_DB_FSM(pdict = pdict)
        _db_basic_data = _db_basic_dict.get("data")
        _dict.update({gdt.PROCESS.DB.value: _db_basic_dict})

        # Read basic parameter from database.
        _mcb_basic_dict = self.__tlgp.get_MCB_FSM(pdict = pdict)
        _mcb_basic_data = _mcb_basic_dict.get("data")
        _dict[gdt.PROCESS.MCB.value] = _mcb_basic_dict

        # Compare basic parameter betweem MCB and database.
        if self.__check_DB == False:    # Do not check DB.
            # Enable screbdriver.
            self.__tlgp.enable_screwdriver()
            self.__tlgp.set_last_ctrl_tlg(tlg = gdt.TLGC.CTRL_NON.value)
            self.__ui.set_tool_init_status(status=True)                  # 
            _dict[gdt.PROCESS.M_FSM.value] = gdt.MSTATUS.READY.value     # 
        else:
            if self.__comm.compare_dict(dict1= _db_basic_data,dict2= _mcb_basic_data)==False:
                # Disable screwdriver.
                self.__tlgp.disable_screwdriver()               
                # Set REQ_301
                _req301_dict = self.__amsp.convert_ams_dict(
                                        key="REQ301",
                                        pdict = _mcb_basic_dict.get("data"))
                _req301_ams_str = self.__amsp.encode_ams_cmd(pdict=_req301_dict)

                _ui_status = ({
                    "cmd_id":gdt.AMSCmd.REQ_I_BASIC_PARA.value,
                    "data"  :_req301_ams_str
                })
                self.__ui.set_tool_init_status(status=False)                 # 
                _dict[gdt.PROCESS.UI_S.value]  = _ui_status                  # 
                _dict[gdt.PROCESS.M_FSM.value] = gdt.MSTATUS.SETTING.value   # Jump to initial FSM.
            else:
                # Enable screbdriver.
                self.__tlgp.enable_screwdriver()
                self.__tlgp.set_last_ctrl_tlg(tlg = gdt.TLGC.CTRL_NON.value)
                self.__ui.set_tool_init_status(status=True)                  # 
                _dict[gdt.PROCESS.M_FSM.value] = gdt.MSTATUS.READY.value     # 
        return _dict
    
    #region Main FSM.
    # Running gtcs system.
    def __system_ready(self,pdict:dict)->dict: # DATA300
        # Initial.
        _dict = dict()
        _comm = comm.CommonFunction()
        try:            
            # Get TMD start signal.
            _current_tmd_flags = _comm.convert_to_bit_list(
                                    length = 4,
                                    data   = self.__current_mcb_status[gdt.StatusTlgEnum.TMD_FLAGS.value])
            _tmd_start = _current_tmd_flags[gdt.TMDStatusFlg.LERVER_SW.value]    

            # Get MCB start signal.
            _current_MCB_flags = _comm.convert_to_bit_list(
                                    length = 16,
                                    data   = self.__current_mcb_status[gdt.StatusTlgEnum.STATUS_FLAGS.value])
            _mcb_start = _current_MCB_flags[gdt.StatusFlagsIdx.TOOL_START.value]
            _mcb_error = _current_MCB_flags[gdt.StatusFlagsIdx.ERROR_OCCURED.value]

            # Get UI cmd .
            _ui_cmd = pdict[gdt.PROCESS.UI_C.value] 

            # M_FSM jump to next status.
            if _mcb_error ==True:                                        # If mcb error occued.
                _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.ALARM.value   
                self.__log.debug("Next M_FSM = MCB_ERR")
            elif _ui_cmd["cmd_id"]==gdt.AMSCmd.REQ_IDLE.value:           # REQ300
                _dict[gdt.PROCESS.UI_S.value] = self.__ui.get_UI_status_FSM(pdict=pdict)     
                _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.READY.value   
                self.__tlgp.set_last_ctrl_tlg(gdt.TLGC.CTRL_NON.value)
                self.__log.debug("Next M_FSM = READY_REQ300")
                self.__SL_ON = False
            elif _ui_cmd["cmd_id"]==gdt.AMSCmd.REQ_SL.value:             # REQ302
                # Set Ctrl tlg.
                _ams_dict = _ui_cmd.get("data")
                _input_trq = int(int(_ams_dict["str5"])/5*1862)
                self.__tlgp.set_SL_target_torque(torque=_input_trq)
                self.__tlgp.set_SL_max_current(current=(int(_ams_dict["str6"])*1000))

                if _ams_dict["str8"]=="1":
                    self.__tlgp.set_SL_ctrl_flag(flag=8)

                if _ams_dict["str9"]=="1":
                    self.__tlgp.set_SL_ctrl_flag(flag=4)

                if _ams_dict["str10"]=="1":
                    self.__tlgp.set_SL_ctrl_flag(flag=2)

                if _ams_dict["str11"]=="1":
                    self.__tlgp.set_SL_ctrl_flag(flag=0)

                _dict[gdt.PROCESS.UI_S.value] = self.__ui.get_UI_status_FSM(pdict=pdict)
                _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.READY.value   
                self.__tlgp.set_last_ctrl_tlg(gdt.TLGC.CTRL_SL.value)
                self.__log.debug("Next M_FSM = READY_REQ302")
            elif self.__comm.compare_dict(dict1= _ui_cmd["data"],dict2= self.__last_ui_cmd)==False:
                self.__last_ui_cmd = _ui_cmd["data"]
                _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.SETTING.value
                self.__log.debug("Next M_FSM = SETTING")             
            else:
                _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.READY.value  #
                self.__tlgp.set_last_ctrl_tlg(gdt.TLGC.CTRL_NON.value) 
                self.__log.debug("Next M_FSM = READY_NO_REQ")
            
            # System idle.
            if ((_tmd_start == False)&(_mcb_start ==False)):              
                self.__last_mcb_flags = _current_MCB_flags
        except Exception as e:
            self.__log.error("__system_ready err,msg = %s"%(e))
        finally:
            return _dict

    # System. 
    def __system_alarm(self,pdict:dict)->dict:
        # Initial obj.
        _dict = dict()
        _comm = comm.CommonFunction()
        try:
            # Get TMD start signal.
            _current_tmd_flags = _comm.convert_to_bit_list(
                                        length = 4,
                                        data=self.__current_mcb_status[gdt.StatusTlgEnum.TMD_FLAGS.value])
            _tmd_start = _current_tmd_flags[gdt.TMDStatusFlg.LERVER_SW.value]    

            if _tmd_start ==True:
                self.__tlgp.clear_MCB_error()
        except Exception as e:self.__log.error("Alarm error,msg = %s"%(e))
        finally:  
            _dict[gdt.PROCESS.M_FSM.value] = gdt.MSTATUS.READY.value
        return _dict

    # Settimg gtcs system.
    def __system_setting(self,pdict:dict)->dict:
        # Initial obj.
        _dict = pdict
        # Command FSM.
        try:
            # Get UI FSM.
            _ui_cmd = _dict.get(gdt.PROCESS.UI_C.value)
            _cmd_fsm  = _ui_cmd.get("cmd_id")
            _cmd_data = _ui_cmd.get("data")
            print("SETTING FSM = %s"%(_cmd_fsm))
            
            if (_cmd_fsm != "")&(_cmd_fsm !="REQ300"):     # Setting
                # Get MCB FSM.
                _dict[gdt.PROCESS.MCB.value] = {
                    "cmd_id": _cmd_fsm,
                    "data"  : _cmd_data
                }                 
                _dict.update(self.__tlgp.get_MCB_FSM(pdict=_dict))
                # Get DB FSM.
                _dict[gdt.PROCESS.DB.value] = {
                    "cmd_id": _cmd_fsm,
                    "data":_dict.get(gdt.PROCESS.MCB.value).get("data")
                }
                _dict.update(self.__gdb.get_DB_FSM(pdict=_dict))  
                # Get UI FSM.
                _dict.update(self.__ui.get_UI_status_FSM(pdict=_dict))   
            else:
                # Stay in M_FSM = setting.  
                self.__log.error("No received data from UI.=%s")  
        except Exception as e:
            self.__log.error("Setting system error,msg=%s"%(e))
        finally:
            # Jump to Ready FSM.
            _dict[gdt.PROCESS.MCB.value] = {"cmd_id": gdt.TLGC.CTRL_NON.value,"data":""}
            _dict[gdt.PROCESS.M_FSM.value]= gdt.MSTATUS.READY.value  # 
        return _dict    
    
    # Check system FSM.
    def __get_system_FSM(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = pdict
        # Main FSM.
        main_fms = pdict.get("M_FSM")
        self.__log.debug("M_FSM = %s "%(main_fms))
        # print("M_FSM = %s "%(main_fms))   
        try:
            data = main_fms
            {
                # System common status.
                gdt.MSTATUS.READY.value   :lambda:_dict.update(self.__system_ready(pdict = _dict)),
                gdt.MSTATUS.ALARM.value   :lambda:_dict.update(self.__system_alarm(pdict = _dict)),
                gdt.MSTATUS.SETTING.value :lambda:_dict.update(self.__system_setting(pdict = _dict)),
                # Initial.
                gdt.MSTATUS.CHECK.value   :lambda:_dict.update(self.__check_basic_para(pdict = _dict)),
                gdt.MSTATUS.START.value   :lambda:_dict.update(self.__start_system(pdict = _dict))
            }.get(data,lambda : self.__log.warning("Main FSM = %s ,Cmd error!"%(data)))()
        except Exception as e:
            print("M_FSM error,FSM = %s,err_msg=%s"%(main_fms,e))
            self.__log.error("M_FSM error,FSM = %s,err_msg=%s"%(main_fms,e))
        finally:
            _dict.update({"M_Status":"Running"})
        return _dict
    #endregion

    #region presenter process
    def gtcs_presenter(self):
        #region Initial Process.
        #Initial main23.
        _q = Queue()
        _p_main23  = Process(target= self.__init_main23,args=(_q,))
        _check_sys = system()
        if _check_sys == 'Linux':  
            if self.__p_main23_start ==True: _p_main23.start()  # Initial main23.  
            sleep(1)

        # Creat dictionary.
        _mgr = Manager()
        _dict_main = _mgr.dict()
        _dict_main.update(self.__update_init_dict(pdict=_dict_main))      
        # Start system.
        _dict_main.update(self.__start_system(pdict=_dict_main))        
        # Check system basic parameter betweem MCB and tcs database.
        _dict_main.update(self.__check_basic_para(pdict=_dict_main))   #

        # Initial process.
        _p_tcp_ui_server = Process(target=self.handle_tcp_ui_process,args=(_dict_main,)) 
        __p_tcp_lan_start = Process(target=self.handle_tcp_lan_process,args=(_dict_main,))

        # Start process. 
        if self.__p_tcp_ui_start == True:_p_tcp_ui_server.start() 
        if self.__p_tcp_lan_start  == True:__p_tcp_lan_start.start()         
        
        # Initial timer counter.
        t20  = perf_counter_ns()
        #endregion

        # region Main FSM
        # Main process while loop.
        while True:
            # Inital elpased timer.
            t0 = perf_counter_ns()
            elpased_t20  = t0 - t20
            # TDMA.
            if elpased_t20>=20000000:     # TDMA = 20MS.
                t20 = t0                  # Reset t20.
                # Storage current MCB status.
                try:
                    if _dict_main.get(gdt.PROCESS.MCB.value).get("cmd_id") == "status":
                        self.__current_mcb_status = _dict_main.get(gdt.PROCESS.MCB.value).get("data")
                        # Check system FSM.
                        _dict_main.update(self.__get_system_FSM(pdict=_dict_main))
                    # Get MCB FSM status.
                    _dict_main.update(self.__tlgp.get_MCB_FSM(pdict=_dict_main)) 
                except Exception as e:print(e)             
        #endregion        

        #region Finish Process.
        # Join process.
        _p_main23.join()
        _p_tcp_ui_server.join()
        __p_tcp_lan_start.join()
        #endregion
    #endregion
#endregion
