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
import os 

# Gtcs IOM Presenter
class GtcsDBPresenter:    
    # Initial private attribute.
    __gtcs_db = gdt.GtcsData() 
    __tcs_db_name = ""
    __tcs_bacis_table = ""
    __db = db.SqliteManager()
    __log = log.log()
    
    # Initial IOM obj
    def __init__(self):pass

    # Set tcs database name.
    def set_tcs_db_name(self,name):self.__tcs_db_name = name
    def set_tcs_db_table(self,table):self.__tcs_bacis_table = table
        
    # Create TCS_data.db
    def create_database(self,table:dict)->bool:
        # Initial sql command.
        _sql_cmd =(
            '''
            create table if not exists %s
            (
                data_time datetime,
                device_type int,
                device_sn int,
                tool_sn int,
                selected_job int,
                selected_sequence int,
                selected_step int,
                direction int,
                fasten_uint int,
                count_dircetion int,
                last_screw_count int,
                max_screw_count int,
                fasten_time real,
                fasten_torque real,
                fasten_angle real,
                max_torque real,
                revolutions real,
            ) 
            '''
            %(
                self.__tcs_bacis_table,
            )
        )
        #   
        try:
            self.__db.read_sql(db_name = self.__tcs_db_name,cmd=_sql_cmd)
        except Exception as e:
            print(e)

    # Get basic parameter from database . 
    def __get_DB_basic_data(self)->dict:
        _data_list = []
        _sql_cmd =(
            '''
            select 
            *
            from %s
            where rowid = 1 
            '''
            %(
                self.__tcs_bacis_table,
            )
        )
        _data_list = self.__db.read_sql(db_name = self.__tcs_db_name,cmd=_sql_cmd)
        return _data_list
    
    # Convert_DB_basic_data
    def __convert_DB_basic_data(self,plist)->dict:
        # Initial dictionary.
        _dict = dict()
        # Get data keys.
        _keys = self.__gtcs_db.get_data_keys()
        for idx,val in enumerate(_keys):
            _dict.update({val:plist[idx]})
        return _dict 
    
    # Check basice parameter.
    def __update_DB_basic_data(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = pdict
        # Read basic info from tcs.db
        # Create sql string.
        _keys_list = list(pdict.get(gdt.PROCESS.MCB.value).get("data").keys())
        _values_list = list(pdict.get(gdt.PROCESS.MCB.value).get("data").values())

        # Deside write to db item.
        _sql_value = ""
        for idx,val in enumerate(_keys_list):
            _sql_value += str(val)+"="+"'"+str(_values_list[idx])+"'"+","
        
        _sql_value = _sql_value[:-1]

        # Create sql command.
        _sql_cmd =(
            '''
            update %s 
            set 
            %s   
            where rowid = 1 
            '''
            %(                
                self.__tcs_bacis_table,
                _sql_value,
            )
        )
        # Update CMD IOM database.
        _update_status = self.__db.operating_status_cmd(db_name = self.__tcs_db_name,cmd=_sql_cmd)   
        if _update_status=="":
            _dict[gdt.PROCESS.DB.value] = {
                "cmd_di":gdt.AMSCmd.ANS_W_BASIC_PARA.value,
                "data"  :pdict.get("data")
            }
        else:
            _dict[gdt.PROCESS.DB.value] = {
                "cmd_di":pdict.get("cmd_id"),
                "data"  :pdict.get("data")
            }                 
        # Set response dictionary. 
        return _dict
    
    # Check basice parameter.
    def __read_DB_basic_data(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = dict()
        # Read basic info from tcs.db
        _db_basic = self.__convert_DB_basic_data(self.__get_DB_basic_data()[0])
        # Compare two basic info dictionary betweem MCB telegram and db. 
        # Set response dictionary. 
        _dict.update({
            "cmd_id": gdt.AMSCmd.REQ_I_BASIC_PARA.value,
            "data"  :_db_basic
        })
        return _dict      
    
    # Storage DATA300 to db.
    def __save_data300(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = dict()
        # Read basic info from tcs.db
        _db_basic = self.__convert_DB_basic_data(self.__get_DB_basic_data()[0])
        # Compare two basic info dictionary betweem MCB telegram and db. 
        # Set response dictionary. 
        _dict.update({
            "cmd_id": gdt.AMSCmd.REQ_I_BASIC_PARA.value,
            "data"  :_db_basic
        })
        return _dict
    
    # Get database fsm dictionary.
    def get_DB_FSM(self,pdict:dict)->dict:
        # Initial dictionary.
        _dict = pdict
        # Select FSm.
        try:
            _cmd_fsm = pdict.get(gdt.PROCESS.DB.value).get("cmd_id")
            self.__log.debug("DB_FSM = %s"%(_cmd_fsm))
            data = _cmd_fsm
            {   
                # CMD
                gdt.AMSCmd.CMD_ACK.value        :lambda:_dict.update(pdict),                      # CMD300
                gdt.AMSCmd.CMD_CTRL.value       :lambda:_dict.update(pdict),                      # CMD302
                gdt.AMSCmd.CMD_W_BASIC.value    :lambda:_dict.update(
                                                        self.__update_DB_basic_data(pdict=pdict)),# CMD340
                gdt.AMSCmd.CMD_R_DB_BASIC.value :lambda:_dict.update(
                                                        self.__read_DB_basic_data(pdict=pdict)),  # CMD343
                # ANS
                # DATA
                gdt.AMSCmd.DATA_RT_STATUS.value   :lambda:_dict.update(pdict),                    # DATA300
                # REQ 
                gdt.AMSCmd.REQ_I_BASIC_PARA.value :lambda:_dict.update(
                                                        self.__save_data300(pdict=pdict)),             # REQ301
            }.get(data,lambda : self.log.debug("Cmd error,DB FSM  = %s"%(data)))()
        except Exception as e:
            print("Get DB FSM dicte error,msg=%s"%(e))
            self.__log.error("Get DB FSM dicte error,msg=%s"%(e))
        finally:
            return _dict
    