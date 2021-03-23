#!/usr/bin/python
# -*- coding: UTF-8 -*-
from module import telegram as tlg
from module import logging as log
from datetime import datetime as dt
import os 

#region MCB commport.
class Monitor:
    # Initial object        
    #def __init__(self, *args, **kwargs):super().__init__(*args, **kwargs)
    def __init__(self):
        pass 

    # Monitor tlg information. 
    def dispaly_status_info(self):
        _tlgs = tlg.TlgStructure()
        # print('_tlgs id = %d'%(id(_tlgs)))
        # os.system("clear")
        print("---------------------------------------------------------------------")
        print("u16Ctrlflags = %d"%(_tlgs.ctrl_request.fasten_dict["u16Ctrlflags"]))
        print("u16ControlMode = %d"%(_tlgs.ctrl_request.fasten_dict["u16ControlMode"]))
        print("u16Statusflags = %d"%(_tlgs.status_response.dict["u16Statusflags"]))
        print("status flag list = ",end='')
        print(_tlgs.status_response.status_flg_list)
        # print("u16ActError = %d"%(_tlgs.status_response.dict["u16ActError"]))
        print("error flag list = ",end="")
        print(_tlgs.status_response.error_flg_list)
        print("u16ActProcNr = %d"%(_tlgs.status_response.dict["u16ActProcNr"]))
        print("u16ActStepNr = %d"%(_tlgs.status_response.dict["u16ActStepNr"]))
        _act_current = (_tlgs.status_response.dict["u16ActCurr"]/1000)
        print("u16ActCurr = %6.3f (A)"%(_act_current))
        print("u16ActTorque = %d"%(_tlgs.status_response.dict["u16ActTorque"]))
        print("u16ActRPM = %d"%(_tlgs.status_response.dict["u16ActRPM"]))
        _max_current = (_tlgs.status_response.dict["u16MaxCurrent"]/1000)
        print("u16MaxCurrent = %6.3f (A)"%(_max_current))
        print("u16MaxTorque = %d"%(_tlgs.status_response.dict["u16MaxTorque"]))
        # #
        print("u32Angle = %.1f"%(_tlgs.status_response.dict["u32Angle"]))
        print("u32Revolutions = %.2f"%(_tlgs.status_response.dict["u32Revolutions"]))
        print("u16TMDFlags = %d"%(_tlgs.status_response.dict["u16TMDFlags"]))
        print("TMD Flag list = ",end='')
        print(_tlgs.status_response.TMD_flags_list)
        print("s16Debug = %d"%(_tlgs.status_response.dict["s16Debug"]))
        print("s32Debug = %d"%(_tlgs.status_response.dict["s32Debug"]))  

    # Moniting SL tlg information. 
    def dispaly_SL_info(self):
        _tlgs = tlg.TlgStructure()
        # print('_tlgs id = %d'%(id(_tlgs)))
        # os.system("clear")
        print("---------------------------------------------------------------------")
        print("u16Ctrlflags = %d"%(_tlgs.ctrl_request.fasten_dict["u16Ctrlflags"]))
        print("u16ControlMode = %d"%(_tlgs.ctrl_request.fasten_dict["u16ControlMode"]))
        print("u16Statusflags = %d"%(_tlgs.status_response.dict["u16Statusflags"]))
        print("status flag list = ",end='')
        print(_tlgs.status_response.status_flg_list)
        # print("u16ActError = %d"%(_tlgs.status_response.dict["u16ActError"]))
        print("error flag list = ",end="")
        print(_tlgs.status_response.error_flg_list)
        print("u16ActProcNr = %d"%(_tlgs.status_response.dict["u16ActProcNr"]))
        print("u16ActStepNr = %d"%(_tlgs.status_response.dict["u16ActStepNr"]))
        _act_current = (_tlgs.status_response.dict["u16ActCurr"]/1000)
        print("u16ActCurr = %6.3f (A)"%(_act_current))
        print("u16ActTorque = %d"%(_tlgs.status_response.dict["u16ActTorque"]))
        print("u16ActRPM = %d"%(_tlgs.status_response.dict["u16ActRPM"]))
        _max_current = (_tlgs.status_response.dict["u16MaxCurrent"]/1000)
        print("u16MaxCurrent = %6.3f (A)"%(_max_current))
        print("u16MaxTorque = %d"%(_tlgs.status_response.dict["u16MaxTorque"]))
        # #
        print("u32Angle = %.1f"%(_tlgs.status_response.dict["u32Angle"]))
        print("u32Revolutions = %.2f"%(_tlgs.status_response.dict["u32Revolutions"]))
        print("u16TMDFlags = %d"%(_tlgs.status_response.dict["u16TMDFlags"]))
        print("TMD Flag list = ",end='')
        print(_tlgs.status_response.TMD_flags_list)
        print("s16Debug = %d"%(_tlgs.status_response.dict["s16Debug"]))
        print("s32Debug = %d"%(_tlgs.status_response.dict["s32Debug"]))  

    def get_tcsmain_tdd_Info(self):
        _tlgs = tlg.TlgStructure()
        _dt = dt.now().strftime('%H:%M:%S')

        _err_str = ''
        if _tlgs.status_response.error_flg_list[0]==1:_err_str += str(0)+','
        if _tlgs.status_response.error_flg_list[1]==1:_err_str += str(1)+','
        if _tlgs.status_response.error_flg_list[2]==1:_err_str += str(2)+','
        if _tlgs.status_response.error_flg_list[3]==1:_err_str += str(3)+','
        if _tlgs.status_response.error_flg_list[4]==1:_err_str += str(4)+','
        if _tlgs.status_response.error_flg_list[5]==1:_err_str += str(5)+','
        if _tlgs.status_response.error_flg_list[6]==1:_err_str += str(6)+','
        if _tlgs.status_response.error_flg_list[7]==1:_err_str += str(7)+','
        if _tlgs.status_response.error_flg_list[8]==1:_err_str += str(8)+','
        if _tlgs.status_response.error_flg_list[9]==1:_err_str += str(9)+','
        if _tlgs.status_response.error_flg_list[10]==1:_err_str += str(10)+','
        if _tlgs.status_response.error_flg_list[11]==1:_err_str += str(11)+','
        if _tlgs.status_response.error_flg_list[12]==1:_err_str += str(12)+','
        if _tlgs.status_response.error_flg_list[13]==1:_err_str += str(13)+','
        if _tlgs.status_response.error_flg_list[14]==1:_err_str += str(14)+','
        if _tlgs.status_response.error_flg_list[15]==1:_err_str += str(15)+','
        if _tlgs.status_response.error_flg_list[16]==1:_err_str += str(16)+','
        if _tlgs.status_response.error_flg_list[17]==1:_err_str += str(17)+','
        if _tlgs.status_response.error_flg_list[18]==1:_err_str += str(18)+','
        if _tlgs.status_response.error_flg_list[19]==1:_err_str += str(19)+','
        if _tlgs.status_response.error_flg_list[20]==1:_err_str += str(20)+','
        if _tlgs.status_response.error_flg_list[21]==1:_err_str += str(21)+','
        # if _tlgs.status_response.error_flg_list[22]==1:_err_str += str(22)+','

        _lst = [_dt,                                                  # 21152
                _tlgs.status_response.dict["s16Debug"],                       # 20025
                _tlgs.status_response.dict["u16MaxCurrent"],             # 20023
                _tlgs.status_response.dict["u16MaxTorque"],
                #_tlgs.status_response.error_flg_list,
                _err_str,
        ]
        
        return _lst
    
    def get_actural_alarm_list(self):
        _tlgs = tlg.TlgStructure()
        _err_str = ''
        if _tlgs.status_response.error_flg_list[0]==1:_err_str += str(0)+','
        if _tlgs.status_response.error_flg_list[1]==1:_err_str += str(1)+','
        if _tlgs.status_response.error_flg_list[2]==1:_err_str += str(2)+','
        if _tlgs.status_response.error_flg_list[3]==1:_err_str += str(3)+','
        if _tlgs.status_response.error_flg_list[4]==1:_err_str += str(4)+','
        if _tlgs.status_response.error_flg_list[5]==1:_err_str += str(5)+','
        if _tlgs.status_response.error_flg_list[6]==1:_err_str += str(6)+','
        if _tlgs.status_response.error_flg_list[7]==1:_err_str += str(7)+','
        if _tlgs.status_response.error_flg_list[8]==1:_err_str += str(8)+','
        if _tlgs.status_response.error_flg_list[9]==1:_err_str += str(9)+','
        if _tlgs.status_response.error_flg_list[10]==1:_err_str += str(10)+','
        if _tlgs.status_response.error_flg_list[11]==1:_err_str += str(11)+','
        if _tlgs.status_response.error_flg_list[12]==1:_err_str += str(12)+','
        if _tlgs.status_response.error_flg_list[13]==1:_err_str += str(13)+','
        if _tlgs.status_response.error_flg_list[14]==1:_err_str += str(14)+','
        if _tlgs.status_response.error_flg_list[15]==1:_err_str += str(15)+','
        if _tlgs.status_response.error_flg_list[16]==1:_err_str += str(16)+','
        if _tlgs.status_response.error_flg_list[17]==1:_err_str += str(17)+','
        if _tlgs.status_response.error_flg_list[18]==1:_err_str += str(18)+','
        if _tlgs.status_response.error_flg_list[19]==1:_err_str += str(19)+','
        if _tlgs.status_response.error_flg_list[20]==1:_err_str += str(20)+','
        if _tlgs.status_response.error_flg_list[21]==1:_err_str += str(21)+','
        return _err_str

    def get_actural_Info(self):
        _tlgs = tlg.TlgStructure()
        _dt = dt.now().strftime('%H:%M:%S')
        _err_str = self.get_actural_alarm_list()

        _str = (_dt+","+_err_str)
        
        return _str
        
    def display_dict_info(self,_dict):
        _key_list = list(_dict.keys())
        _value_list = list(_dict.values())
        for idx,val in enumerate(_key_list):
            print(str(idx)+"."+val,end="=")
            print(_value_list[idx])
#endregion   
