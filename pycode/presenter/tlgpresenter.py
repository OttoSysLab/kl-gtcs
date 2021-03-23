#!/usr/bin/python
# -*- coding: UTF-8 -*-
from module import telegram as tlg
from module import common  as comm
from module import comport as com
from module import monitor
from module import gtcsdata as gdt
from module import logging as log
from time import sleep,perf_counter_ns
from typing import Dict,List
from platform import system
import datetime
import os 
# from functools import singledispatch

#region Presenter for telegram.
class TlgPresenter():
    # Private 
    __MCB_poll_delay = 0
    __MCB_write_flash_delay = 0
    __MCB_write_para_delay = 0
    __MCB_comm = com.MCBComm()
    __comm = comm.CommonFunction()

    __tlgp = tlg.TcsProtocol()
    __tlgs = tlg.TlgStructure()
    __last_state = 0
    __gdt = gdt.GtcsData()
    open_comm = False

    __tool_started = False
    __last_ctrl_tlg = ""
    __log  = log.log()
    
    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass

    #region tlg communcation.
    # Initial MCB comm.
    def initial_MCB_serial(self,MCBcomm,MCBbaudrate):
        return self.__MCB_comm.open_MCB_comm(MCBcomm,MCBbaudrate)       
    
    # Set telegram poll delay time.
    def set_MCB_poll_delay(self,delay):
        """
        docstring
        """
        self.__MCB_poll_delay = delay
    
    def set_MCB_write_flash_delay(self,delay):
        """
        docstring
        """
        self.__MCB_write_flash_delay = delay
    
    def set_MCB_write_para_delay(self,delay):
        """
        docstring
        """
        self.__MCB_write_para_delay = delay

    # Set UI INI file path.
    def set_UI_INI_Path(self,Path):
        """
        docstring
        """
        self.__UI_INI_PATH = Path

    def set_MCB_INI_Path(self,Path):
        """
        docstring
        """
        self.__MCB_INI_PATH = Path

    def set_last_ctrl_tlg(self,tlg:str):self.__last_ctrl_tlg = tlg

    def enable_screwdriver(self):   # Enable screwdriver. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.initial_ctrl_flags()
        self.__tlgs.ctrl_request.set_ctrl_flag(idx=gdt.CtrlFlag.EN_TIMEOUT_200.value)
        self.__tlgs.ctrl_request.set_ctrl_flag(idx=gdt.CtrlFlag.SCREWDRIVER_EN.value)     
        # self.__tlgs.ctrl_request.set_ctrl_flag(idx=gdt.CtrlFlag.SCREWDRIVER_STA.value)
        # self.__tlgs.ctrl_request.set_ctrl_flag(idx=gdt.CtrlFlag.LOCK_MOTOR.value)              
    
    def disable_screwdriver(self):  # Disable screwdriver.
        """
        docstring
        """
        self.__tlgs.ctrl_request.initial_ctrl_flags()

    def set_SL_ctrl_flag(self,flag:int):  # Set self lenaing flags. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.set_SL_ctrl_flag(idx=flag)
    
    def clear_SL_ctrl_flag(self,flag:int):  # Set self lenaing flags. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.clear_SL_ctrl_flag(idx=flag)
    
    def set_SL_target_torque(self,torque:int):  # Set self leaning flags. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.SL_dict[gdt.CtrlTlgEnum.MAX_TORQUE.value] = torque
    
    def set_SL_max_current(self,current:int):  # Set self leaning flags. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.SL_dict[gdt.CtrlTlgEnum.MAX_CURRENT.value] = current
    
    def set_SL_target_angle(self,angle:int):  # Set self leaning flags. 
        """
        docstring
        """
        self.__tlgs.ctrl_request.SL_dict[gdt.CtrlTlgEnum.MAX_CURRENT.value] = angle

    def get_MCB_ctrl_tlg(self,direction:str)->dict:   # Get ctrl telegram form MCB.  
        """
        docstring
        """
        _dict = dict()
        if direction == gdt.Dircetion.FASTEN.value:
            _dict = self.__tlgs.ctrl_request.fasten_dict
        elif direction == gdt.Dircetion.SL.value:
            _dict = self.__tlgs.ctrl_request.SL_dict 
        else:
            _dict = self.__tlgs.ctrl_request.loosen_dict
        return _dict
    
    def get_MCB_status_tlg(self):        # Get MCB status    
        """
        docstring
        """
        _tlgs = tlg.TlgStructure()
        return _tlgs.status_response.dict
    
    def get_TMD_flags(self):
        """
        docstring
        """
        _tlgs = tlg.TlgStructure()
        _comm = comm.CommonFunction()
        _TMD_flags = _tlgs.status_response.dict.get(gdt.StatusTlgEnum.TMD_FLAGS.value)
        return _comm.convert_to_bit_list(length=8,data=_TMD_flags)
    
    def SL_MCB_polling(self):
        """
        docstring
        """
        _tlgs = tlg.TlgStructure()
        # Initial ctrl.
        _tlgs.ctrl_request.dict = _tlgs.ctrl_request.SL_dict
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)        
        # Sleep 0.01s.
        sleep(self.__MCB_poll_delay)        
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()        
        # Decode telegram.
        if len(_list)==48:
            if _tlgs.decode_response_tlg(_list)==True:
                pass
            else:
                self.__log.warning("Decode tlg error!")    
        else:
            self.__log.warning("SL_MCB_polling eho length is not 48 byte.!")
        # Retrurn MCB status dict.
        _status_dict =  _tlgs.status_response.dict
        return _status_dict

    # Polling to MCB without data.
    def poll_to_MCB(self):
        # initial tlg TlgStructure.
        _tlgs = tlg.TlgStructure()
        _err_flag = False
        _restart = False
        
        # Import _tcsP data.
        _tlgs.ctrl_request.initial_ctrl_flags()
        if _restart==False:
            for idx,val in enumerate(_tlgs.status_response.error_flg_list):
                if ((val!=1)|(idx==22)):
                    _err_flag = False
                else:
                    _err_flag = True
                    break
        if ((_err_flag==True)&(_restart == False)):
            sleep(self.__MCB_write_flash_delay)
            _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.ERROR_ACK.value)   
            _restart = True         
        else:
            # _tlgs.ctrl_request.set_ctrl_flag(tlg.CtrlFlag.EN_TIMEOUT_200.value)
            _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.SCREWDRIVER_EN.value)
            _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.SCREWDRIVER_STA.value)            
        
        # Encode requeste telegram.
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)

        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        # Sleep 0.01s.
        sleep(self.__MCB_poll_delay)
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()
        # decode tlg
        if len(_list)==48:
            if _tlgs.decode_response_tlg(_list)==True:
                pass
            else:
                self.__log.warning("Decode tlg error!")      
        else:
            self.__log.warning("poll_to_MCB eho length is not 48 byte.!")

        #_q.put(_tlgs)   
        return _tlgs
 
    # Set control flags.
    def get_MCB_polling_status(self,ctrl_dict:dict)->dict:
        _tlgs = tlg.TlgStructure()
        # Initial ctrl.
        _tlgs.ctrl_request.dict = ctrl_dict.copy()
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)        
        # Sleep 0.01s.
        sleep(self.__MCB_poll_delay)        
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()        
        # Decode telegram.
        if len(_list)==48:
            if _tlgs.decode_response_tlg(_list)==True:
                pass
            else:
                self.__log.warning("Decode tlg error!")    
        else:
            self.__log.warning("get_MCB_polling_status eho length is not 48 byte.!")
        # Retrurn MCB status dict.
        _status_dict =  _tlgs.status_response.dict
        return _status_dict   

    # Received from MCB. 
    # Initial MCB bacis parameter.
    def write_MCB_para(self,MID,SID,plist):
        _plist = plist
        _tlgs = tlg.TlgStructure()
        _len = len(plist)
        _pIdx = 0
        _tlgs.Para_wirte_request.clear_para_array()    
        for idx,val in enumerate(_plist):   
            _tlgs.Para_wirte_request.set_TLG_RW_addr(PAddr=_pIdx,
                                                    MID=MID,
                                                    SID=idx+SID,
                                                    data=val,
                                                    dlengths=2)
            _pIdx+=1
            if (_pIdx > 5)|(idx+1==_len):
                _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.WRITE_REQUEST.value)
                self.__MCB_comm.write_to_MCB(_send_tlg)
                # Sleep 0.01s.
                sleep(self.__MCB_write_para_delay)
                # Read from MCB.
                _list = self.__MCB_comm.read_MCB_data()
                _tlgs.Para_wirte_request.clear_para_array()
                _pIdx = 0
    
    # Read MCB int.
    def read_MCB_int(self,MID,SID):
        _rev = ""
        _tlgs = tlg.TlgStructure()
        _tlgs.Para_read_request.clear_para_array()
        _tlgs.Para_read_request.set_TLG_RW_addr( PAddr=0,
                                                MID=MID,
                                                SID=SID,
                                                data=0,
                                                dlengths=32)

        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.READ_REQUEST.value)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        # Sleep 0.01s.
        sleep(self.__MCB_write_para_delay)
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()
            
        if len(_list)==48:
            if _tlgs.decode_response_tlg(_list)==True:
                _rev = _tlgs.Para_response.get_int_array()
            else:
                self.__log.warning("Decode tlg error!")  
        else:
            self.__log.warning("ReadMCB_int eho length is not 48 byte.!")

        _tlgs.Para_read_request.clear_para_array()
            
        return _rev
    
    # Read MCB str.
    def read_MCB_str(self,MID,SID):
        _rev = ""
        _tlgs = tlg.TlgStructure()
        _tlgs.Para_read_request.clear_para_array()
        _tlgs.Para_read_request.set_TLG_RW_addr( PAddr=0,
                                                MID=MID,
                                                SID=SID,
                                                data=0,
                                                dlengths=32)

        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.READ_REQUEST.value)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        # Sleep 0.01s.
        sleep(self.__MCB_write_para_delay)
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()
            
        if len(_list)==48:
            if _tlgs.decode_response_tlg(_list)==True:
                _rev = _tlgs.Para_response.get_str_array()
            else:
                self.__log.warning("Decode tlg error!") 
        else:
            self.__log.warning("read_MCB_str eho length is not 48 byte.!")

        _tlgs.Para_read_request.clear_para_array()
        return _rev

    # Read parameter from MCB.
    def read_MCB_para(self,MID,SID,aList):
        _rLst = []
        _len = len(aList)
        _pIdx = 0 
        self.__tlgs.Para_read_request.clear_para_array()
        for idx,val in enumerate(aList):   
            self.__tlgs.Para_read_request.set_TLG_RW_addr( PAddr=_pIdx,
                                                    MID=MID,
                                                    SID=idx+SID,
                                                    data=val,
                                                    dlengths=2)
            _pIdx+=1
            if (_pIdx > 5)|(idx+1==_len):
                _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.READ_REQUEST.value)
                # Write TLG to MCB.
                self.__MCB_comm.write_to_MCB(_send_tlg)
                # Sleep 0.01s.
                sleep(self.__MCB_write_para_delay)
                # Read from MCB.
                _list = self.__MCB_comm.read_MCB_data()
                # Descode 
                if len(_list)==48:
                    self.__tlgs.Para_response.set_decode_type(type_num=0)
                    if self.__tlgs.decode_response_tlg(_list)==True:
                        _rLst.extend(self.__tlgs.Para_response.get_para_array())
                    else:
                        self.__log.warning("Decode tlg error!") 
                else:
                    self.__log.warning("read_MCB_para eho length is not 48 byte.!")
                self.__tlgs.Para_read_request.clear_para_array()
                _pIdx = 0
        return _rLst
    
    # Write MCB Flash
    def write_MCB_flash(self,MID_val,SID_val,data_val):
        _tlgs = tlg.TlgStructure()
        _tlgs.Para_wirte_request.clear_para_array()
        _tlgs.Para_wirte_request.set_TLG_RW_addr(PAddr=1,
                                                MID=MID_val,
                                                SID=SID_val,
                                                data=data_val,
                                                dlengths=2)
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.WRITE_REQUEST.value)
        #print(_send_tlg)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        # Sleep 0.1s.
        sleep(self.__MCB_write_flash_delay)
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()
        # print(_list)      
    
    # Read MCB flash value.
    def read_MCB_flash(self,MID_val,SID_val,data_val):
        _tlgs = tlg.TlgStructure()
        _tlgs.Para_read_request.clear_para_array()
        _tlgs.Para_read_request.set_TLG_RW_addr(PAddr=1,
                                                MID=MID_val,
                                                SID=SID_val,
                                                data=data_val,
                                                dlengths=2)
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.WRITE_REQUEST.value)
        #print(_send_tlg)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        # Sleep 0.1s.
        sleep(self.__MCB_write_flash_delay)
        # Read from MCB.
        _list = self.__MCB_comm.read_MCB_data()
        return _list
    
    # read datalogger from MCB.
    def read_MCB_datalog(self):
        # Config send tlg
        _tlgs = tlg.TlgStructure()
        _t1=perf_counter_ns()
        _cnt = 0
        _datalogger_list = []

        # Get 4096 datalogger.
        while True:
            _t2=perf_counter_ns()
            # config manual    
            _tlgs.datalog_request.search_addr = _cnt      # data loger array address. 
            _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.DL_REQUEST.value)
            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Sleep 0.01s.
            sleep(self.__MCB_poll_delay)
            # Read from MCB.
            _ser_buf = self.__MCB_comm.read_MCB_data()
            #print(_ser_buf)
        
            # decode tlg
            if len(_ser_buf)==48:
                if _tlgs.decode_response_tlg(_ser_buf)==True:pass
                else:
                    self.__log.warning("Decode datalogger error!")
            if _cnt <255 :
                _cnt+=1
            else:
                _datalogger_list = _tlgs.datalog_response.datalog_list
                break
        return _datalogger_list
    #endregion

    #region tlg paremeter.
    # Get dict.
    def get_TLGP_dict(self,MID)->dict:
        _tcsp = tlg.TcsProtocol()
        #print('get_TCSP_list _tcsp id = %d'%(id(_tcsp)))
        _dict=None
        if MID=="ID1":               # MAIN_ID = 1
            _dict = _tcsp.identification_para.dict
        elif MID=="Basic":           # MAIN_ID = 2
            _dict = _tcsp.basic_para.dict
        elif MID=="Step":            # MAIN_ID = 3XXX
            _dict = _tcsp.step_para.dict
        elif MID=="Program":         # MAIN_ID = 4XXX
            _dict = _tcsp.process_para.dict
        elif MID=="ProcStepIDList":  # MAIN_ID = StepList
            _dict = _tcsp.process_para.u16ProcStepId
        elif MID=="Sequence":        # MAIN_ID = 5
            _dict = _tcsp.program_para.dict
        elif MID=="Statistc":        # MAIN_ID = 6
            _dict = _tcsp.status_para.dict
        elif MID=="Ctrl":            # MAIN_ID = 7
            _dict = _tcsp.ctrl_para.dict
        elif MID=="Manual":       # MAIN_ID = 8
            _dict = _tcsp.manual_para.dict
        elif MID=="Status":       # MAIN_ID = 9
            _dict = _tcsp.status_para.dict
        return _dict
    
    # set_TLGP_dict
    def set_TLGP_dict(self,MID,_dict):
        _tcsp = tlg.TcsProtocol()
        # print('get_TCSP_list _tcsp id = %d'%(id(_tcsp)))
        # print(_dict)
        if MID=="ID1":        # MAIN_ID = 1
            for val in _tcsp.identification_para.key_list:
                _tcsp.identification_para.dict[val] = _dict[val]
        elif MID=="Basic":      # MAIN_ID = 2
            for val in _tcsp.basic_para.key_list:
                _tcsp.basic_para.dict[val] = _dict[val]
        elif MID=="Step":    # MAIN_ID = 3XXX
            for val in _tcsp.step_para.key_list:
                _tcsp.step_para.dict[val] = _dict[val]
        elif MID=="Program":    # MAIN_ID = 4XXX
            for val in _tcsp.process_para.key_list:
                _tcsp.process_para.dict[val] = _dict[val]
        elif MID=="ProcStepIDList":  # MAIN_ID = StepList
            for val in _tcsp.process_para.step_id_list:
                _tcsp.process_para.u16ProcStepId[val] = _dict[val]
        elif MID=="Sequence":       # MAIN_ID = 5
            for val in _tcsp.program_para.key_list:
                _tcsp.program_para.dict[val] = _dict[val]
        elif MID=="Statistc":       # MAIN_ID = 6
            for val in _tcsp.status_para.key_list:
                _tcsp.status_para.dict[val] = _dict[val]
        elif MID=="Ctrl":       # MAIN_ID = 7
            for val in _tcsp.ctrl_para.key_list:
                _tcsp.ctrl_para.dict[val] = _dict[val]
            pass
        elif MID=="Manual":       # MAIN_ID = 8
            for val in _tcsp.manual_para.key_list:
                _tcsp.manual_para.dict[val] = _dict[val]
        elif MID=="Status":       # MAIN_ID = 9
            for val in _tcsp.status_para.key_list:
                _tcsp.status_para.dict[val] = _dict[val]
        return _dict

    # Get telegram basic parameter list.
    def get_TCSP_list(self,MID):
        _tcsp = tlg.TcsProtocol()
        _list=[]
        if MID=="ID1":                      # MAIN_ID = 1
            _list = list(_tcsp.identification_para.dict.values())
        elif MID=="Basic":                  # MAIN_ID = 2
            _list = list(_tcsp.basic_para.dict.values())
        elif MID=="Step":                   # MAIN_ID = 3XXX
            _list = list(_tcsp.step_para.dict.values())[2:]
        elif MID=="Program":    # MAIN_ID = 4XXX
            _list = list(_tcsp.process_para.dict.values())[2:]
        elif MID=="ProcStepIDList":
            _list = list(_tcsp.process_para.u16ProcStepId.values())
        elif MID=="Sequence":       # MAIN_ID = 5
            _list = list(_tcsp.program_para.dict.values())
        elif MID=="Statistc":       # MAIN_ID = 6
            _list = list(_tcsp.status_para.dict.values())
        elif MID=="Ctrl":       # MAIN_ID = 7
            _list = list(_tcsp.ctrl_para.dict.values())
        elif MID=="Manual":       # MAIN_ID = 8
            _list = list(_tcsp.manual_para.dict.values())
        elif MID=="Status":       # MAIN_ID = 9
            _list = list(_tcsp.status_para.dict.values())
        return _list
    
    # set_telegram_initial_value
    def get_TCSP_dict(self,MID:str)->dict:
        _dict = dict()
        _tcsP = tlg.TcsProtocol()                                  #print('set _tcsp id = %d '%(id(_tcsP)))        
        # Initial ID2 parapmeter to telegram mode.
        if MID == "ID1":                                           # MAIN_ID = 1
            _dict = _tcsP.identification_para.dict.copy()
        elif MID == "Basic":                                           # MAIN_ID = 1
            _dict = _tcsP.basic_para.dict.copy()
        elif MID == "Step":
            _dict = _tcsP.step_para.dict.copy()
        elif MID == "Program":
            _dict = _tcsP.process_para.dict.copy()
        elif MID == "ProcStepIDList":
            _dict = _tcsP.process_para.dict.copy()
        elif MID == "Sequence":pass
        elif MID == "Statistc":pass
        elif MID == "Ctrl":pass
        elif MID == "Manual":pass
        elif MID == "Status":pass
        return _dict 

    # set_telegram_initial_value
    def set_TCSP_para(self,MID:str,plist:list):
        _tcsP = tlg.TcsProtocol()                                  #print('set _tcsp id = %d '%(id(_tcsP)))        
        # Initial ID2 parapmeter to telegram mode.
        if MID == "ID1":                                           # MAIN_ID = 1
            _key_list = _tcsP.identification_para.key_list
            for idx,val in enumerate(_key_list):
                _tcsP.identification_para.dict.update({val:plist[idx]})
        elif MID == "Basic":                                           # MAIN_ID = 1
            _key_list = _tcsP.basic_para.key_list
            for idx,val in enumerate(_key_list):
                _tcsP.basic_para.dict.update({val:plist[idx]}) 
        elif MID == "Step":
            _key_list = _tcsP.step_para.key_list[2:]
            for idx,val in enumerate(_key_list):
                _tcsP.step_para.dict.update({val:plist[idx]}) 
        elif MID == "Program":
            _key_list = _tcsP.process_para.key_list[2:]
            for idx,val in enumerate(_key_list):
                _tcsP.process_para.dict.update({val:plist[idx]})
        elif MID == "ProcStepIDList":
            _key_list = _tcsP.process_para.step_id_list
            for idx,val in enumerate(_key_list):
                _tcsP.process_para.u16ProcStepId.update({val:plist[idx]})
        elif MID == "Sequence":pass
        elif MID == "Statistc":pass
        elif MID == "Ctrl":pass
        elif MID == "Manual":pass
        elif MID == "Status":pass

    # Set process step list.
    def set_TCSP_prcoess_step_list(self,pList):
        _tcsP = tlg.TcsProtocol()
        for idx,val in enumerate(pList):
            _tcsP.process_para.u16ProcStepId[idx] = val
    
    # Set control request for telegram mode.
    def set_comm_ctrl_request(self,plist):
        # Initial tlg strucst object.
        _tlgs = tlg.TlgStructure()
        #print('_tlgs id = %d'%(id(_tlgs)))
        # Config para.
        for idx,val in enumerate(plist):
            _key_list = _tlgs.ctrl_request.key_list
            _tlgs.ctrl_request.fasten_dict.update({_key_list[idx]:val})
        
    # Set control request for telegram mode.
    def set_tlg_ctrl_request(self,_dict):
        # Initial tlg strucst object.
        _tlgs = tlg.TlgStructure()    #print('_tlgs id = %d'%(id(_tlgs)))
        # Config para.
        _tlgs.ctrl_request.fasten_dict = _dict.copy()
    #endregion

    #Check_error_flg
    def check_error_occure(self):
        _tlgs = tlg.TlgStructure()
        for idx,val in enumerate(_tlgs.status_response.error_flg_list):
            if ((val!=1)|(idx==22)):
                _tlgs.status_response.error_occure = False
            else:
                _tlgs.status_response.error_occure = True
                break
        return _tlgs.status_response.error_occure

    def cyclic_start_MCB_process(self,plist):     # cyclic start MCB process
        _tlgs = tlg.TlgStructure()
        _tempterature = plist[0]
        _delay = _temp = plist[1]
        _mrt_stop = False
     
        # control mode.
        _tlgs.ctrl_request.initial_ctrl_flags()    
        # Check error flag. 
        self.check_error_occure()

        #if (_tlgs.status_response.ack_flg == False)&(_tlgs.status_response.error_occure == True):
        if (_tlgs.status_response.error_occure == True):
            if (_tlgs.status_response.ack_cnt<2): 
                _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.ERROR_ACK.value)
                _tlgs.status_response.ack_cnt +=1
                #_tlgs.status_response.ack_flg = True
                _mrt_stop = True
            else:
                _tlgs.status_response.ack_cnt = 0
        elif((_tlgs.status_response.first_start_flg==True)&(_tlgs.status_response.restart_flg==True)):  #一般機器
            sleep(1)
            _tlgs.status_response.ack_cnt = 0
            _tlgs.status_response.restart_flg = False            
        else:
            _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.EN_TIMEOUT_200.value)
            _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.SCREWDRIVER_EN.value)
            # _tlgs.ctrl_request.set_ctrl_flag(tlg.CtrlFlag.LOCK_MOTOR.value)
            # _tlgs.ctrl_request.set_ctrl_flag(tlg.CtrlFlag.SCREWDRIVER_STA.value)
        
        _tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.FASTEN.value)
            
        # Encode send tlg.      
        _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)
        # Write TLG to MCB.
        self.__MCB_comm.write_to_MCB(_send_tlg)
        
        # Sleep 0.01s.
        # if _tlgs.status_response.error_occure==True:sleep(0.1)
        sleep(self.__MCB_poll_delay)
        
        # Read from MCB.
        _ser_buf = self.__MCB_comm.read_MCB_data()
        #print(_ser_buf)
        # decode tlg    
        if len(_ser_buf)==48:
            if _tlgs.decode_response_tlg(_ser_buf)==True:                            
                # MRT 
                if _tlgs.status_response.status_flg_list[15]==1:
                    if _tlgs.status_response.first_start_flg == False:
                        _tlgs.status_response.first_start_flg = True
                    # if _tlgs.status_response.restart_flg  ==True:
                    #     _tlgs.status_response.restart_flg = False
                else:
                    if _tlgs.status_response.first_start_flg==False:
                        pass
                    else:
                        if _tlgs.status_response.dict["u16ActStepNr"] == 3: #1
                            if _tlgs.status_response.restart_flg  ==False:
                                _tlgs.status_response.restart_flg =True
                                _mrt_stop = True
                                                
                if _tlgs.status_response.TMD_flags_list[0]==1:
                    _tlgs.status_response.ack_flag==False
                    _tlgs.status_response.ack_cnt =0

        return _mrt_stop        
    
    # Handle ini read and write event.
    def read_MCB_to_dict(self,_MID,NIDX,SIDX):
        # Read from MCB.
        try:
            _SID_list = self.get_TCSP_list(_MID)
            _list = self.read_MCB_para(MID=NIDX,SID =SIDX,aList=_SID_list)
            self.set_TCSP_para(MID=_MID,plist=_list)
        except Exception as e:
            self.__log.error(e)
        finally:
            _dict = self.get_TLGP_dict(MID = _MID)
        return _dict

    # Handle ini read and write event.
    def read_MCB_tool_dict(self):
        # Read from MCB.
        _list = []
        try:
            # 
            self.__tlgs.Para_response.set_decode_type(type_num=1)
            for idx in range(3):
                _list.append(self.read_MCB_str(MID=1,SID =idx+1))
            # 
            self.__tlgs.Para_response.set_decode_type(type_num=2)
            for idx in range(3):
                _list.append(str(self.read_MCB_int(MID=1,SID =idx+4)))                
        except Exception as e:
            self.__log.error(e)
        finally:
            self.set_TCSP_para(MID="ID1",plist=_list)
            _dict = self.get_TLGP_dict(MID = "ID1")
        return _dict    

    # Write MCB save information.
    def write_MCB_save_info(self,_dict):
        # Initial object.
        _com_dict = dict()

        _d_buf = _dict.get("Program")
        self.set_TLGP_dict(MID ="Program",_dict = _d_buf)
        _p_id = int(_d_buf["u8ProcID"])+3999
        _p_list = list(_d_buf.values())[2:]
        self.write_MCB_para(MID=_p_id,SID=2,plist=_p_list)
        # self.write_MCB_flash(7,12,_p_id)

        _d_buf = _dict.get("ProcStepIDList")
        self.set_TLGP_dict(MID ="ProcStepIDList",_dict = _d_buf)
        _p_id_list = list(_d_buf.values())
        _p_id_list[0] = _p_id_list[0]+2999
        self.write_MCB_para(MID=_p_id,SID=1000,plist=_p_id_list)
        self.write_MCB_flash(7,12,_p_id)
    
        _d_buf = _dict.get("Step")
        self.set_TLGP_dict(MID ="Step",_dict = _d_buf)
        _s_id = int(_d_buf["u8StepID"])+2999
        _s_list = list(_d_buf.values())[2:]
        self.write_MCB_para(MID=_s_id,SID=2,plist=_s_list)
        self.write_MCB_flash(7,11,_s_id)   
    
    # Read MCB start dict
    def read_MCB_start_para(self):
        # Read MCB basic parameter.(ID2)
        _d_basic = self.read_MCB_to_dict("Basic",2,1)
        _d_basic_buf  = dict(_d_basic)
        # Read MCB fasten process &step parameter.(4248,3498)
        _dFP = self.read_MCB_to_dict("Program",4248,2)       
        _dFP.update({"u8ProcID":4248})
        _d_FP_buf = dict(_dFP)
        # Read MCB fasten process list.
        _d_FPID = self.read_MCB_to_dict("ProcStepIDList",4248,1000)      # 
        _d_FPID_buf = dict(_d_FPID)
        # Read MCB fasten step parameter.
        _d_FS = self.read_MCB_to_dict("Step",3498,2)                     # 
        _d_FS.update({"u8StepID":3498})
        _d_FS_buf =dict(_d_FS)   

        # Read MCB loosen process &step parameter.(4248,3498)
        _d_LP = self.read_MCB_to_dict("Program",4249,2)       
        _d_LP.update({"u8ProcID":4249})
        _d_LP_buf  = dict(_d_LP)
        # Read MCB fasten process list.
        _d_LPID = self.read_MCB_to_dict("ProcStepIDList",4249,1000)      #
        _d_LPID_buf = dict(_d_LPID)
        # Read MCB fasten process list.
        _d_LS = self.read_MCB_to_dict("Step",3499,2)                     # 
        _d_LS.update({"u8StepID":3499})
        _d_LS_buf = dict(_d_LS)

        _MCB_STR_dict = dict()
        _MCB_STR_dict.update({"Basic":_d_basic_buf})
        _MCB_STR_dict.update({"Program_F":_d_FP_buf})
        _MCB_STR_dict.update({"ProcStepIDList_F":_d_FPID_buf})
        _MCB_STR_dict.update({"Step_F":_d_FS_buf})
        _MCB_STR_dict.update({"Program_L":_d_LP_buf})
        _MCB_STR_dict.update({"ProcStepIDList_L":_d_LPID_buf})
        _MCB_STR_dict.update({"Step_L":_d_LS_buf})
        return _MCB_STR_dict

    # Write data from TMD storage
    def write_TMD_flash(self,addr:int,value:int)->int:
        _data = 0
        try:
            # Set TMD data to telegram.
            self.__tlgs.TMD_request.set_TMD_cmd(cmd_id=gdt.TMD.WRITE_PARA_TO_STORE.value,
                                            addr  = addr,
                                            value = value)
            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.TMD_REQUEST.value)
            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Sleep 0.01s.
            sleep(self.__MCB_poll_delay)       
            # Read MCB data .
            _list = self.__MCB_comm.read_MCB_data()        
            # Decode telegram.
            if len(_list)==48:
                if self.__tlgs.decode_response_tlg(_list)==True:
                    pass
                else:
                    self.__log.warning("Decode error!")
            else:
                self.__log.warning("write_TMD_flash eho length is not 48 byte.")
        except Exception as e:
            self.__log.error("Read TMD flash .msg = %s"%(e))
        return _data
    # 
    def get_TMD_cmd(self):return 
    # Read data from TMD storage
    def read_TMD_flash(self,addr:int)->str:
        _value = 0
        try:
            # Set TMD data to telegram.
            self.__tlgs.TMD_request.set_TMD_cmd(cmd_id=gdt.TMD.READ_STORED_PARA.value,
                                            addr  = addr,
                                            value = 0)
            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.TMD_REQUEST.value)
            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Sleep 0.01s.
            sleep(self.__MCB_poll_delay)       
            # Read MCB data .
            _list = self.__MCB_comm.read_MCB_data()        
            # Decode telegram.
            if len(_list)==48:
                if self.__tlgs.decode_response_tlg(_list)==True:
                    _cmd = self.__tlgs.TMD_response.get_TMD_cmd()
                    if _cmd>0x80:
                        # _value = self.__tlgs.TMD_response.get_TMD_cmd()
                        _value = "ERROR:0x51000000"
                    else:
                        _value = self.__tlgs.TMD_response.get_TMD_value()
                else:
                    self.__log.warning("Decode error!")
            else:
                self.__log.warning("read_TMD_flash eho length is not 48 byte.")
        except Exception as e:
            self.__log.error("Read TMD flash .msg = %s"%(e))
        return _value

    # Get all data from 
    def read_TMD_all_data(self)->bool:
        # Initial obj.
        _read_OK = False
        # Set 
        try:
            # Get TMD request data from sobject.           
            self.__tlgs.TMD_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.FASTEN.value)

            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.TMD_REQUEST.value)

            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Chang MCB FSM.
        except Exception as e:self.__log.error(e)
        return _read_OK
    
    # Set TMD data. 
    def set_TMD_data(self,addr:int,data:int)->int:
        _data = 0        
        return _data

    # Read MCB basic parameter.
    def read_MCB_basic_para(self):
        # Read MCB basic parameter.(ID2)
        _d_basic = self.read_MCB_to_dict("Basic",2,1)
        return _d_basic     

    # Check control telegram status.
    def check_ctrl_tlg_status(self,tlg,idx):
        _check_ok = False
        if ((tlg>>idx)&0x0001)==True:
            _check_ok = True
        return _check_ok  

    # Clear MCB error.
    def clear_MCB_error(self): 
        # Initial obj
        _tlgs = tlg.TlgStructure()
        # clear time = 3
        for cnt in range(3):
        # while(_tlgs.status_response.status_flg_list[gdt.StatusFlagsIdx.ERROR_OCCURED.value]):
            # control mode.
            _tlgs.ctrl_request.initial_ctrl_flags()    
            # Ack error.
            if cnt <2:
                _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.ERROR_ACK.value)
            else:
                _tlgs.ctrl_request.set_ctrl_flag(gdt.CtrlFlag.SCREWDRIVER_EN.value)

            # Encode send tlg.      
            _send_tlg = _tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)
            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Sleep 0.01s.
            sleep(self.__MCB_poll_delay)            
        
            # Read from MCB.
            _ser_buf = self.__MCB_comm.read_MCB_data()
            
            # decode tlg    
            if len(_ser_buf)==48:
                pass
            else:
                self.__log.warning("clear_MCB_error eho length is not 48 byte.")
    
    # Send tool ctrl cmd.
    def __send_tool_ctrl_cmd(self,pdict:dict)->dict:
        # Initial obj.
        _dict = pdict
        try:
            _mcb_cmd  = _dict.get(gdt.PROCESS.MCB.value)
            _tool_cmd = _mcb_cmd.get("data")
            if _tool_cmd["str5"]=="0":     # Enable 
                self.enable_screwdriver()
            elif _tool_cmd["str5"]=="1":   # Disable
                self.disable_screwdriver()
            elif _tool_cmd["str5"]=="2":   # Confirm
                pass
            elif _tool_cmd["str5"]=="3":   # Repeat
                pass            
            # 
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id":gdt.TLGC.CTRL_NON.value,
                "data"  :_dict.get(gdt.PROCESS.MCB.value).get("data")
            }
        except Exception as e :self.__log.error("__send_tool_ctrl_cmd cmd,msg = %s"%(e))
        return _dict

    # Send control telegram to MCB. 
    def __send_MCB_normal_ctrl(self,pdict:dict)->dict:
        """
        docstring
        """
        _dict = dict()
        try:
            # Set MCB ctrl tlg dict.
            _TMD_flags = self.get_TMD_flags()

            # Clear error if occured.           
            if _TMD_flags[gdt.TMDStatusFlg.REVERSE_SW.value]==False:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.LOOSEN.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            else:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.FASTEN.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            
            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)

            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Chang MCB FSM.
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id":gdt.TLGC.STATUS.value,
                "data"  :self.__tlgs.status_response.dict,
            }
        except Exception as e:self.__log.error("__send_MCB_normal_ctrl err,msg = %s"%(e))
        return _dict
    
    # Send MCB advance ctrl telegram.
    def __send_MCB_advance_ctrl(self, pdict:dict)->dict:
        """
        docstring
        """
        _dict = dict()
        try:
            # Set MCB ctrl tlg dict.
            _TMD_flags = self.get_TMD_flags()

            # Clear error if occured.           
            if _TMD_flags[gdt.TMDStatusFlg.REVERSE_SW.value]==False:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.LOOSEN.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            else:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.FASTEN.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            
            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)

            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Chang MCB FSM.
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id":gdt.TLGC.STATUS.value,
                "data"  :self.__tlgs.status_response.dict,
            }
        except Exception as e:self.__log.error("__send_MCB_advance_ctrl err,msg = %s"%(e))
        return _dict
    
    # Send MCB advance ctrl telegram.
    def __send_MCB_SL_ctrl(self, pdict:dict)->dict:
        """
        docstring
        """
        _dict = dict()
        try:
            # Set MCB ctrl tlg dict.
            _TMD_flags = self.get_TMD_flags()

            # Clear error if occured.           
            if _TMD_flags[gdt.TMDStatusFlg.REVERSE_SW.value]==False:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.LOOSEN.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            else:
                self.__tlgs.ctrl_request.dict = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.SL.value)
                self.__log.debug("Ctrl tlg = %d"%(self.__tlgs.ctrl_request.dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value]))
            
            # Encode MCB ctrl tlg dict to str.
            _send_tlg = self.__tlgs.encode_request_tlg(gdt.HearderType.CTRL_TLG.value)

            # Write TLG to MCB.
            self.__MCB_comm.write_to_MCB(_send_tlg)
            # Chang MCB FSM.
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id":gdt.TLGC.STATUS.value,
                "data"  :self.__tlgs.status_response.dict,
            }
        except Exception as e:self.__log.error("__send_MCB_SL_ctrl err,msg = %s"%(e))
        return _dict

    # Receive status telegram from MCB.
    def __receive_MCB_status(self,pdict:dict)->dict:
        _dict = pdict
        try:
            _list = self.__MCB_comm.read_MCB_data()        
            # Decode telegram.
            if len(_list)==48:
                if self.__tlgs.decode_response_tlg(_list)==True:
                    pass
                else:
                    self.__log.warning("Decode error!")
            else:
                self.__log.warning("__receive_MCB_status eho length is not 48 byte.")

            # Chang MCB FSM and get MCB status dict.
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id":self.__last_ctrl_tlg,
                "data"  :self.__tlgs.status_response.dict,
            }
        except Exception as e:self.__log.error("__receive_MCB_status err,msg = %s"%(e))
        return _dict
    
    # Chang to Idle status.
    def __chang_to_polling_FSM(self,pdict:dict)->dict:
        # Inital  dictionary.
        _dict = dict()
        # Set MCB ctrl tlg dict.
        _status_flags = self.get_TMD_flags()
        if _status_flags[gdt.TMDStatusFlg.REVERSE_SW.value]==True:
            _ctrl_tlg = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.LOOSEN.value)
        else:
            _ctrl_tlg = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.FASTEN.value)
        # Chang MCB FSM.
        _dict.update({
            "cmd_id":gdt.TLGC.CTRL_NON.value,
            "data"  :_ctrl_tlg,
        })
        return _dict  
    
    # Chang to Idle status.
    def __set_SL_ctrl_tlg( self, pdict:dict)->dict:
        """
        docstring
        """
        # Inital  dictionary.
        _dict = dict()
        
        # 
        # self.__tlgs.ctrl_request.SL_dict[gdt.]
        # self.__tlgs.ctrl_request.SL_dict[gdt.]
        # self.__tlgs.ctrl_request.SL_dict[gdt.]

        # Set MCB ctrl tlg dict.
        _ctrl_tlg = self.get_MCB_ctrl_tlg(direction=gdt.Dircetion.SL.value)
        # Chang MCB FSM.
        _dict.update({
            "cmd_id":gdt.TLGC.CTRL_SL.value,
            "data"  :_ctrl_tlg,
        })
        return _dict  
    
    # Read papameter from MCB.
    def __update_MCB_basic_para(self,pdict:dict)->dict:
        """
        docstring
        """
        _dict = pdict    # Initial obj.
        # Save to MCB.
        try:     
            _mcb_cmd_dict = pdict.get(gdt.PROCESS.UI_C.value).get("data").values()
            # print(type(_mcb_cmd_dict))      
            _mcb_cmd_list = list(_mcb_cmd_dict)[4:]
            self.set_TCSP_para(MID ="Basic",plist= _mcb_cmd_list)
            _mcd_set_list = self.get_TCSP_list(MID='Basic')
            _mcb_basic_rowdata = self.__gdt.convert_MCB_basic_rawdata(plist=_mcd_set_list)
            #寫下去再讀回來
            self.write_MCB_para(MID=2,SID=1,plist=list(_mcb_basic_rowdata.values()))
            _mcb_basic_rowdata = self.read_MCB_basic_para()
            _mcb_basic_para = self.__gdt.calaulate_MCB_basic_rawdata(pdict=_mcb_basic_rowdata)
            # _mcb_basic_rowdata = self.read_MCB_basic_para()
            # _mcb_basic_para = self.__gdt.calaulate_MCB_basic_rawdata(pdict=_mcb_basic_rowdata)
            _dict[gdt.PROCESS.MCB.value] = {
                "cmd_id" : gdt.AMSCmd.ANS_W_BASIC_PARA.value,
                "data"   : _mcb_basic_para
            }
        except Exception as e:self.__log.error("Update MCB basic para err,msg =%s"%(e))
        return _dict

    # Read basic MCB para. 
    def __read_MCB_basic_para(self,pdict:dict)->dict:
        """
        docstring
        """
        _dict = dict()
        try:
            _mcb_basic_rowdata = self.read_MCB_basic_para()
            _mcb_basic_para = self.__gdt.calaulate_MCB_basic_rawdata(pdict=_mcb_basic_rowdata)
            # self.write_MCB_para(MID=2,SID=1,plist=_mcb_basic_para)
            _dict.update({
                "cmd_id" : gdt.AMSCmd.REQ_I_BASIC_PARA.value,
                "data"   : _mcb_basic_para})
        except Exception as e:
            self.__log.error("Read MCB basic para err,msg =%s"%(e))
        return _dict

    # Get MCB CMD FSM.
    def get_MCB_FSM(self,pdict:dict)->dict:  # Get MCB FSM from status.
        """
        docstring
        """
        # Initial dictionary.
        _dict = pdict
        # Tlg FSM.
        try:
            _cmd_fsm = pdict.get(gdt.PROCESS.MCB.value).get("cmd_id")            
            # log.debug("MCB_FSM = %s "%(_cmd_fsm))
            # print("MCB_FSM = %s "%(_cmd_fsm))
            data = _cmd_fsm
            {
                # CMD 
                gdt.AMSCmd.CMD_ACK.value            :lambda:_dict.update(   
                                                        self.__send_tool_ctrl_cmd(pdict = _dict)),# CMD300       
                gdt.AMSCmd.CMD_CTRL.value           :lambda:_dict.update(   
                                                        self.__send_tool_ctrl_cmd(pdict = _dict)),# CMD302     
                gdt.AMSCmd.CMD_W_BASIC.value        :lambda:_dict.update(   
                                                        self.__update_MCB_basic_para(pdict = _dict)),# CMD340
                gdt.AMSCmd.CMD_R_MCB_BASIC.value    :lambda:_dict.update(   
                                                        self.__read_MCB_basic_para(pdict = _dict)),# CMD341 
                # REQ
                gdt.AMSCmd.REQ_I_BASIC_PARA.value   :lambda:_dict.update(   
                                                        self.__chang_to_polling_FSM(pdict = _dict)),# REQ301 
                gdt.AMSCmd.REQ_SL.value             :lambda:_dict.update(   
                                                        self.__set_SL_ctrl_tlg( pdict = _dict)),# REQ301               
                # Ctrl TLG
                gdt.TLGC.CTRL_NON.value             :lambda:_dict.update(   # send normal ctrl tlg.
                                                        self.__send_MCB_normal_ctrl(pdict = _dict)),
                gdt.TLGC.CTRL_ADV.value             :lambda:_dict.update(   # send advance ctrl tlg.
                                                        self.__send_MCB_advance_ctrl(pdict = _dict)),
                gdt.TLGC.CTRL_SL.value              :lambda:_dict.update(   # send SL ctrl tlg.
                                                        self.__send_MCB_SL_ctrl(pdict = _dict)),
                # Status TLG
                gdt.TLGC.STATUS.value               :lambda:_dict.update(   # send ctrl tlg.
                                                        self.__receive_MCB_status(pdict = _dict)),
            }.get(data,lambda : self.__log.warning("Tlg FSM = %s"%(_cmd_fsm)))()
        except Exception as e:
            self.__log.error("Get MCB FSM err,msg = %s"%(e))
        return _dict         