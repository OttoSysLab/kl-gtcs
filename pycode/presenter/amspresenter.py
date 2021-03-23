#!/usr/bin/python
# -*- coding: UTF-8 -*-
try:
    # Gtcs module package
    from presenter import tlgpresenter as tlgp
    from module import amspotocol as ams
    from module import common as comm
    from module import gtcsdata as gdt
    from module import logging as log 
    # General package
    from json import JSONDecodeError,JSONDecoder,JSONEncoder
    from datetime import datetime
    from enum import Enum
    from functools import singledispatch
    from platform import system
    import os 
except Exception as e:print(e)

# AMS Preseneter
class GtcsAMSPresenter():
    # Gtcs ams protocol object.
    __ams = ams.AmsGtcsProtocol()
    __last_mcb_flags = [0]*16
    __last_mcb_status = "Idle"
    __first_start_flag = False 
    __restart_flag = False
    __tool_max_torque = 0
    __log = log.log()

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object value.
    def __init__(self): pass

    # Set Max torque
    def set_tool_max_torque(self,torque:float):
        self.__tool_max_torque = torque
    
    # Get tool max torque.
    def get_tool_max_torque(self):
        return self.__tool_max_torque

    # Set mcb flags
    def set_ams_last_flags(self,flags_value:int):
        _comm  = comm.CommonFunction()
        self.__last_mcb_flags = _comm.convert_to_bit_list(length = 16,data = flags_value)
    
    # Get first start status
    def get_first_start_status(self)->bool:
        return self.__first_start_flag

    #region Baisc function.        

    # Set AMS dictionary obj value.
    def set_ams_opposite_dict(self,pdict:dict,key:str):
        self.__ams.set_opposite_dict(pdict = pdict,okey=key)

    # Get AMS dictionary obj value.
    def get_ams_dict(self,key:str):
        return self.__ams.get_sub_dict(key=key)

    # convert_to_ams_dict
    def convert_to_ams_dict(self,key:str,pstr:str)->dict:
        _dict = dict()
        _start_idx = pstr.index("{")
        _stop_idx = pstr.index("}")
        _substr = pstr[(_start_idx+1):_stop_idx]
        _substr_list = _substr.split(",")
        _keys = self.__ams.get_sub_keys(key=key)
        for idx,val in enumerate(_keys):
            _dict.update({val:_substr_list[idx]})
        return _dict

    # convert_ams_dict
    def convert_ams_dict(self,key:str,pdict:dict)->dict:
        _value_list = list(pdict.values())
        _dict = self.__ams.get_sub_dict(key=key)
        _keys = list(_dict.keys())
        for idx,val in enumerate(_keys):
            if idx > 4:
                _dict.update({val:_value_list[idx-4]})
        return _dict
    
    # decode_ams_list
    def decode_ams_list(self,pstr:str)->list:
        _dict = dict()
        _start_idx = pstr.index("{")
        _stop_idx = pstr.index("}")
        _substr = pstr[(_start_idx+1):_stop_idx]
        _substr_list = _substr.split(",")
        return _substr_list

    # Decode AMS cmd.
    def decode_ams_dict(self,pstr:str)->dict:
        _start_idx = pstr.index("{")
        _stop_idx = pstr.index("}")
        _substr = pstr[(_start_idx+1):_stop_idx]
        _substr_list = _substr.split(",")
        _keys = self.__ams.get_sub_dict_keys(key=_substr_list[0])
        _dict = dict()
        for idx,val in enumerate(_keys):
            _dict.update({val:_substr_list[idx]})
        return _dict

    # convert_to_ams_str
    def convert_to_ams_str(self,input_type:str,cmd_id:str,pdict:dict)->str:
        # initial decode list.
        _value_list = list(pdict.values()) # Initial ams string.
        _check_list = []                   # Initial ams string.
        _ams_str = ""                      # Initial ams string.

        if input_type == "DB":    
            _check_list = _value_list            
            # 組data 字串
            _check_str  = ","+str(self.__ams.get_command_sn()).zfill(3)
            for val in _check_list:_check_str += ","+str(val)        
            # Calculate checku sum
            _check_sum = self.__ams.calculate_check_sum(pstr=_check_str)
            # Package command to string.
            _ams_str = "{"
            _ams_str += cmd_id+","
            _ams_str += datetime.now().strftime("%Y%m%d %H:%M:%S") +","
            _ams_str += _check_sum
            _ams_str += _check_str
            _ams_str += "}"        
        return _ams_str

    # Encode AMS protocol command string.
    def encode_ams_cmd(self,pdict:dict)->str:
        _value_list = list(pdict.values())
        _check_list = _value_list[3:]        
        # 組data 字串
        _check_str  = ""
        for val in _check_list:
            _check_str += ","+str(val)        
        # Calculate checku sum.
        _check_sum = self.__ams.calculate_check_sum(pstr=_check_str)
        # Package command to string.
        _value_list[1] = datetime.now().strftime("%Y%m%d %H:%M:%S.%f")[:21]
        _value_list[2] = _check_sum
        _str = "{"
        _len = len(_value_list)
        for idx,val in enumerate(_value_list):
            if idx<(_len-1):
                _str+=str(val)+","
            else:
                _str+=str(val)
        _str += "}"
        return _str
    #endregion
    
    # Get job serial number.
    def __get_job_id(self)->int:
        _job_id = 0
        return _job_id
    
    # Get sequence serial number.
    def __get_seq_id(self)->int:
        _seq_id = 0
        return _seq_id
    
    # Get sequence serial number.
    def __get_prog_id(self)->int:
        _prog_id = 0
        return _prog_id
    
    # Get sequence serial number.
    def __get_step_id(self)->int:
        _step_id = 0
        return _step_id
    
    # Get last count.
    def __get_last_count(self)->int:
        _cnt = 0
        return _cnt
    
    # Get total count.
    def __get_max_count(self)->int:
        _cnt = 0
        return _cnt
    
    # Get counting method.
    def __get_counting_method(self)->int:
        _type = 0
        return _type
    
    # Get input GPIO status.
    def __get_input_gpio_status(self)->int:
        _input_status = 0
        return _input_status
    
    # Get output GPIO status.
    def __get_output_gpio_status(self)->int:
        _output_status = 0
        return _output_status

    # Check MCB runtime status.
    def __check_MCB_RT_status(self,pdict:dict)->str:
        _op_status =  self.__last_mcb_status
        _comm = comm.CommonFunction()
        try:    
            # Get.
            _status_flags = _comm.convert_to_bit_list(
                                            length=16,
                                            data=pdict[gdt.StatusTlgEnum.STATUS_FLAGS.value])
            _tmd_flags    = _comm.convert_to_bit_list(
                                            length=8,
                                            data=pdict[gdt.StatusTlgEnum.TMD_FLAGS.value])
            
            # Get flags status from list.  
            _mcb_STA      = _status_flags[gdt.StatusFlagsIdx.TOOL_START.value]
            _mcb_REV      = _status_flags[gdt.StatusFlagsIdx.TOOL_REVERSE.value] 
            _mcb_NG       = _status_flags[gdt.StatusFlagsIdx.ERROR_OCCURED.value]
            _proc_status  = _status_flags[gdt.StatusFlagsIdx.PROC_STATUS.value]
            _tmd_start = _tmd_flags[gdt.TMDStatusFlg.LERVER_SW.value]            
            _last_proc_status = self.__last_mcb_flags[gdt.StatusFlagsIdx.PROC_STATUS.value]

            # Decide TOOL status.
            if _mcb_STA == True:                                # Tool running.
                if _mcb_REV == True:
                    _op_status = gdt.DATA300Status.REV.value
                else:
                    _op_status = gdt.DATA300Status.RUN.value
                # Check first start.
                if self.__first_start_flag == False:
                    self.__first_start_flag = True
                if self.__restart_flag == True:
                    self.__restart_flag = False
                    self.__last_mcb_flags = _status_flags               # Remain mcb flags status. 
            elif _mcb_NG == True:
                _op_status = gdt.DATA300Status.NG.value      # Tool NG stop.
            elif _proc_status != _last_proc_status:
                _op_status = gdt.DATA300Status.OK.value      # Tool OK stop.
                self.__restart_flag = True
            elif _proc_status == _last_proc_status:
                if _tmd_start == False:                           # check NG status.
                    _op_status = gdt.DATA300Status.NG.value      # Tool stop.   
            else:
                pass
        except Exception as e:
            self.__log.error(e)
            print(e)
        finally:
            self.__last_mcb_status = _op_status
            return _op_status
 
    #region Covert torque rawdata to ams value.
    # Covert torque rawdata to Kgf_cm unit value.
    def __convert_rawdata_to_kgf_m(self,rawdata:int)->float:
        _torque = 0
        return _torque

    # Covert torque rawdata to Nm unit value.
    def __convert_rawdata_to_Nm(self,rawdata:int)->float:
        _torque = round((rawdata/1862)*self.__tool_max_torque,2)
        return _torque
    
    # Covert torque rawdata to Ncm unit value.
    def __convert_rawdata_to_kgf_cm(self,rawdata:int)->float:
        _torque = 0
        return _torque

    # Covert torque rawdata to In_lb unit value.
    def __convert_rawdata_to_lbf_ln(self,rawdata:int)->float:
        _torque = 0
        return _torque

    # Covert torque rawdata to ams value.
    def __convert_to_ams_torque(self,unit:str,rawdata:int)->float:
        _torque = 0
        try:
            _torque = {
                comm.UINT.kgf_m.value  :lambda value:self.__convert_rawdata_to_kgf_m(rawdata=value),
                comm.UINT.N_m.value    :lambda value:self.__convert_rawdata_to_Nm(rawdata=value),
                comm.UINT.kgf_cm.value :lambda value:self.__convert_rawdata_to_kgf_cm(rawdata=value),
                comm.UINT.lbf_in.value :lambda value:self.__convert_rawdata_to_lbf_ln(rawdata=value),
            }[unit](rawdata)
        except Exception as e:
            self.__log.error(e)
        finally:
            return _torque
    #endregion

    # Check tool running dircetion.
    def __check_tool_running_dircetion(self,flags:list)->int:
        _dicection = 0
        if flags[gdt.StatusFlagsIdx.REVERVED.value]==0: # CW
            _dicection = 0
        else:                                          # CCW
            _dicection = 1
        return _dicection
    
    # Get torque unit.
    def __get_torque_unit(self)->int:
        _unit = 1     # Nm
        return _unit
    
    # Check mcb error message.
    def __check_mcb_err_msg(self,errlist:list)->str:
        _gdt = gdt.GtcsData()
        # Initial error string.
        _err_str = ""
        for idx ,val in enumerate(errlist):
            if (val ==True)&(idx!=22):
                _err_str = _gdt.get_MCB_err_msg(idx=idx)
        return  _err_str   

    # Check tool total count.
    def __get_tool_total_CNT(self)->int:
        _cnt = 0
        return  _cnt
    
    # Get fasten timer.
    def __get_fasten_timer(self,flags:list)->float:
        _timer = 0
        return _timer 
    
    # Get Tool REQ300 information. 
    def get_heart_beat(self,pdict:dict)->dict:
        # Initial comm object.
        _comm = comm.CommonFunction()
        # Get.
        _status_flags = _comm.convert_to_bit_list(
                                        length=16,
                                        data=pdict[gdt.StatusTlgEnum.STATUS_FLAGS.value])
        _tmd_flags    = _comm.convert_to_bit_list(
                                        length=8,
                                        data=pdict[gdt.StatusTlgEnum.TMD_FLAGS.value])
        # Get flags status from list.  
        _mcb_STA      = _status_flags[gdt.StatusFlagsIdx.TOOL_START.value] 
        _tmd_start = _tmd_flags[gdt.TMDStatusFlg.LERVER_SW.value]      
              
        # Check if system start.
        if _tmd_start ==True:self.__first_start_flag =True
        
        # Get REQ300 value.
        _ams_obj = self.__ams.get_sub_dict(key=gdt.AMSCmd.REQ_IDLE.value)
        return _ams_obj

    # Calculate MCB dict to ams dict.
    def calculate_RT_MCB_status(self, pdict:dict)->dict:
        """
        docstring
        """
        # Initial common & gtcs function obj.
        _comm = comm.CommonFunction()
        _gdt = gdt.GtcsData()     
        # Get AMS object status from bulletin board.
        _ams_obj    = self.__ams.get_sub_dict(key=gdt.AMSCmd.DATA_RT_STATUS.value)
        # Gonvert MCB flags to list.
        _mcb_flags  = _comm.convert_to_bit_list(  length = 16,
                                                data = pdict[gdt.StatusTlgEnum.STATUS_FLAGS.value])           
        # print(_mcb_flags)
        # Convert MCB error flags to list.
        _err_list   = _comm.convert_to_bit_list(  length = 32,
                                                data = pdict[gdt.StatusTlgEnum.ACT_ERR.value])
        # Get MCB data from rawdata dictionary.
        _dev_type   = _ams_obj.get(gdt.DATA300.DEV_TYPE.value)              # str5
        _tool_sn    = _ams_obj.get(gdt.DATA300.TOOL_SN.value)               # str6
        _device_sn  = _ams_obj.get(gdt.DATA300.DEV_SN.value)                # str7
        _job_id     = self.__get_job_id()                                   # str8
        _seq_id     = self.__get_seq_id()                                   # str9
        _prog_id    = self.__get_prog_id()                                  # str10
        _step_id    = self.__get_step_id()                                  # str11
        _direction  = self.__check_tool_running_dircetion(flags=_mcb_flags) # str12 
        _unit       = self.__get_torque_unit()                              # str13
        _inc_dnc    = self.__get_counting_method()                          # str14
        _last_CNT   = self.__get_last_count()                               # str15
        _max_CNT    = self.__get_max_count()                                # str16
        _fasten_time= self.__get_fasten_timer(flags=_mcb_flags)             # str17
        _act_torque = self.__convert_to_ams_torque(                         # str18
                                        unit   = comm.UINT.N_m.value,
                                        rawdata= pdict[gdt.StatusTlgEnum.ACT_TORQUE.value])
        _act_angle  = _gdt.convert_to_angle(                                # str19
                                        rawdata  = pdict[gdt.StatusTlgEnum.ACT_ANGLE.value],
                                        gear     = gdt.GearBoxRatio.TCS_NT_7.value, 
                                        direction=_direction)

        _max_torque = self.__convert_to_ams_torque(                         # str20
                                        unit   = comm.UINT.N_m.value,
                                        rawdata = pdict[gdt.StatusTlgEnum.MAX_TORQUE.value])
        _revalution = _gdt.convert_to_revolutions(                   # str21
                                        rawdata  = pdict[gdt.StatusTlgEnum.ACT_REVOLUTION.value],
                                        gear     = gdt.GearBoxRatio.TCS_NT_7.value,
                                        direction=_direction)
        
        _mcb_status = self.__check_MCB_RT_status(pdict = pdict)# str22      
        _gpio_in    = self.__get_input_gpio_status()                        # str23
        _gpio_out   = self.__get_output_gpio_status()                       # str24
        _mcb_err    = self.__check_mcb_err_msg(                             # str25
                                        errlist = _comm.convert_to_bit_list(
                                        length = 32,
                                        data   = pdict[gdt.StatusTlgEnum.ACT_ERR.value])) 
        _tool_CNT   = self.__get_tool_total_CNT()                           # str26
        _act_rpm    = pdict[gdt.StatusTlgEnum.ACT_RPM.value]                # str27

        # Update dictionary.
        _ams_obj[gdt.DATA300.DEV_TYPE.value]  = str(_dev_type)      # str5
        _ams_obj[gdt.DATA300.TOOL_SN.value]   = str(_tool_sn)       # str6   
        _ams_obj[gdt.DATA300.DEV_SN.value]    = str(_device_sn)     # str7
        _ams_obj[gdt.DATA300.JOB_ID.value]    = str(_job_id)        # str8
        _ams_obj[gdt.DATA300.SEQ_ID.value]    = str(_seq_id)        # str9
        _ams_obj[gdt.DATA300.PRG_ID.value]    = str(_prog_id)       # str10
        _ams_obj[gdt.DATA300.STEP_ID.value]   = str(_step_id)       # str11
        _ams_obj[gdt.DATA300.DIRECTION.value] = str(_direction)     # str12
        _ams_obj[gdt.DATA300.UNIT.value]      = str(_unit)          # str13
        _ams_obj[gdt.DATA300.INC_DEC.value]   = str(_inc_dnc)       # str14
        _ams_obj[gdt.DATA300.LAST_CNT.value]  = str(_inc_dnc)       # str15
        _ams_obj[gdt.DATA300.MAX_CNT.value]   = str(_inc_dnc)       # str16
        _ams_obj[gdt.DATA300.FASTEN_TR.value] = str(_fasten_time)   # str17          
        _ams_obj[gdt.DATA300.TORQUE.value]    = str(_act_torque)    # str18
        _ams_obj[gdt.DATA300.ANGLE.value]     = str(-round(_act_angle,1))     # str19
        _ams_obj[gdt.DATA300.MAX_TRQ.value]   = str(_max_torque)              # str20
        _ams_obj[gdt.DATA300.REVOLUTION.value]= str(round(_revalution,4))     # str21
        _ams_obj[gdt.DATA300.STATUS.value]    = str(_mcb_status)    # str22
        _ams_obj[gdt.DATA300.GPIO_IN.value]   = str(_gpio_in)       # str23
        _ams_obj[gdt.DATA300.GPIO_OUT.value]  = str(_gpio_out)      # str24
        _ams_obj[gdt.DATA300.ERR_MSG.value]   = str(_mcb_err)       # str25
        _ams_obj[gdt.DATA300.TOOL_CNT.value]  = str(_tool_CNT)      # str26
        _ams_obj[gdt.DATA300.TOOL_RPM.value]  = str(_act_rpm)       # str27
        # Update AMS protocol object.
        self.__ams.set_sub_dict(key=gdt.AMSCmd.DATA_RT_STATUS.value,pdict=_ams_obj)
        return _ams_obj 

    # Get MCB SL dict to ams dict.
    def calculate_ADV_MCB_status(self, pdict:dict)->dict:
        """
        docstring
        """
        # Initial common & gtcs function obj.
        _comm = comm.CommonFunction()
        _gdt = gdt.GtcsData()     
        # Initial common & gtcs function obj.
        _comm = comm.CommonFunction()
        _gdt = gdt.GtcsData()     
        # Get data form tlg.
        _status = ""
        
        # Ansign AMS object status from bulletin.
        _ams_obj  = self.__ams.get_sub_dict(key=gdt.AMSCmd.DATA_SL_STATUS.value) 
        # Update dictionary. 
        _ams_obj[gdt.DATA302.HEADER.value] = str("")     # Header+DATA ,{DATA302
        _ams_obj[gdt.DATA302.TIME.value] = str("")       # yyyyMMdd HH:mm:ss
        _ams_obj[gdt.DATA302.CHECH_SUM.value] = str("")  # Checksum
        _ams_obj[gdt.DATA302.CMD_SN.value] = str("")     # Command_sn     
        _ams_obj[gdt.DATA302.STATUS.value] = str("")     # status
        # Update AMS protocol object.
        self.__ams.set_sub_dict(key=gdt.AMSCmd.DATA_SL_STATUS.value,pdict=_ams_obj)
        return _ams_obj
    
    # Get MCB SL dict to ams dict.
    def calculate_SL_MCB_status(self, pdict:dict)->dict:
        """
        docstring
        """     
        # Initial common & gtcs function obj.
        _comm = comm.CommonFunction()
        _gdt = gdt.GtcsData()     
        # Get data form tlg.
        _status = pdict[gdt.StatusTlgEnum.ACT_STEP.value]
        
        # Ansign AMS object status from bulletin.
        _ams_obj  = self.__ams.get_sub_dict(key=gdt.AMSCmd.DATA_SL_STATUS.value) 
        # Update dictionary.   
        _ams_obj[gdt.DATA302.STATUS.value] = str(_status)     # status
        # Update AMS protocol object.
        self.__ams.set_sub_dict(key=gdt.AMSCmd.DATA_SL_STATUS.value,pdict=_ams_obj)
        return _ams_obj

    #endregion