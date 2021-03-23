#!/usr/bin/python
# -*- coding: UTF-8 -*-
from presenter import amspresenter as ams
from module import database as db
from module import gtcsdata as gdt
from module import common as comm
from module import logging as log

from time import sleep
import datetime
import time 
from platform import system
import socket,sys,errno,os

# Gtcs IOM Presenter
class GtcsUIPresenter:    
    # Initial private attribute.
    __gtcs_db = gdt.GtcsData() 
    __db = db.SqliteManager()
    __amsp = ams.GtcsAMSPresenter()      
    __tool_init_OK = True   
    __log = log.log()
    
    # Initial IOM obj
    def __init__(self):pass 
    
    # Set tool init status. 
    def set_tool_init_status(self,status:bool):self.__tool_init_OK = status

    # Set UI cmd to dictionary.
    def __set_UI_cmd(self,pdict:dict)->dict:
        # Initial obj.
        _dict = dict()
        return _dict

    #  Set ctrl command to MCB.
    def __set_tool_ctrl_cmd(self,pdict:dict)->dict:
        # Initial obj.
        _dict = pdict
        try:
            _dict = _dict.get(gdt.PROCESS.MCB.value).get("data")
            if _dict != '':
                self.__amsp.set_ams_opposite_dict(pdict=_dict,key=gdt.AMSCmd.ANS_CTRL.value)
                _ams_dict = self.__amsp.get_ams_dict(key = gdt.AMSCmd.ANS_CTRL.value)
                _ams_str = self.__amsp.encode_ams_cmd(pdict = _ams_dict)
                _mcb_ans ={
                    "cmd_id":gdt.AMSCmd.ANS_CTRL.value,
                    "data"  :_ams_str
                }
                _dict[gdt.PROCESS.UI_S.value] = _mcb_ans
        except Exception as e:
            self.__log.error("set_tool_ctrl_cmd err,msg = %s"%(e))
        return _dict
    
    # Response.
    def __echo_ANS_W_BASIC(self,pdict:dict)->dict:
        # Initial obj.
        _dict = pdict
        # Get ANS 340.
        try:
            _ui_data = pdict.get(gdt.PROCESS.UI_C.value).get("data")
            self.__amsp.set_ams_opposite_dict(pdict=_ui_data,key=gdt.AMSCmd.ANS_W_BASIC_PARA.value)
            _ams_dict = self.__amsp.get_ams_dict(key=gdt.AMSCmd.ANS_W_BASIC_PARA.value)
            _ams_str = self.__amsp.encode_ams_cmd(pdict=_ams_dict)
            _dict[gdt.PROCESS.UI_S.value] = {
                "cmd_id":gdt.AMSCmd.ANS_W_BASIC_PARA.value,
                "data"  :_ams_str
            }
        except Exception as e:
            self.__log.error("Echo ANS W BASIC err,msg = %s"%(e))
        
        return _dict

    # Get system running ams cmd.
    def __get_system_status(self,pdict:dict)->dict:
        """
        docstring
        """
        _ui_cmd = dict()                  # Initial obj.
        _comm = comm.CommonFunction()
        try:
            if self.__tool_init_OK ==True:
                _mcb_status = pdict.get(gdt.PROCESS.MCB.value).get("data")
                _amsp_dict = self.__amsp.calculate_RT_MCB_status(pdict=_mcb_status)
                _amsp_str  = self.__amsp.encode_ams_cmd(pdict=_amsp_dict)
                # Inital IOM cmd and data.
                _ui_cmd = ({
                    "cmd_id": gdt.AMSCmd.DATA_RT_STATUS.value,
                    "data"  : _amsp_str
                })
            else:                
                _ui_cmd = ({
                    "cmd_id": gdt.AMSCmd.REQ_I_BASIC_PARA.value,
                    "data"  : _amsp_str
                })                
        except Exception as e:self.__log.error("Get system running error msg = %s"%(e))
        return _ui_cmd
    
    # Get self leaning status
    def __get_SL_status(self,pdict:dict)->dict:
        """
        docstring
        """
        _ui_cmd = dict()
        try:
            if self.__tool_init_OK == True:
                _mcb_status = pdict.get(gdt.PROCESS.MCB.value).get("data")
                _amsp_dict = self.__amsp.calculate_SL_MCB_status(pdict=_mcb_status)
                _amsp_str  = self.__amsp.encode_ams_cmd(pdict=_amsp_dict)
                # Inital IOM cmd and data.
                _ui_cmd = ({
                    "cmd_id": gdt.AMSCmd.DATA_SL_STATUS.value,
                    "data"  : _amsp_str
                })   
        except Exception as e:
            self.__log.error("Get SL status err,msg=%s "%(e))
        return _ui_cmd

    # Get UI fsm dictionary.
    def get_UI_status_FSM(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = pdict
        # Select FSM.
        try:
            _cmd_fsm = pdict.get(gdt.PROCESS.UI_C.value).get("cmd_id")
            # print("Time = %s ,IOM_FSM = %s "%(datetime.datetime.now().strftime("%Y%m%d %H:%M:%S.%f")[:21],
            #                                 _cmd_fsm,))
            data = _cmd_fsm
            {   
                # CMD 
                gdt.AMSCmd.CMD_CTRL.value       :lambda:_dict.update(    
                                                        self.__set_tool_ctrl_cmd(pdict=pdict)),# CMD302

                gdt.AMSCmd.CMD_W_BASIC.value    :lambda:_dict.update(    
                                                        self.__echo_ANS_W_BASIC(pdict=pdict)), # CMD340
                # REQ
                gdt.AMSCmd.REQ_IDLE.value       :lambda:_dict.update(    
                                                        self.__get_system_status(pdict=pdict)),# REQ300
                gdt.AMSCmd.REQ_SL.value         :lambda:_dict.update(    
                                                        self.__get_SL_status(pdict=pdict)),    # REQ302
                
            }.get(data,lambda : self.__log.warning("Cmd err, UI FSM = %s"%(data)))()
        except Exception as e:
            self.__log.error("Get UI FSM dicte error,msg=%s"%(e))
        finally:
            return _dict
    