#!/usr/bin/python
# -*- coding: UTF-8 -*-
#!/usr/bin/python
# -*- coding: UTF-8 -*-
from module import fsm
from module import database  
from configparser import ConfigParser
from time import sleep,perf_counter_ns
from platform import system 
from time import sleep,perf_counter_ns
from enum import Enum
from time import sleep
import datetime
import ctypes
import json
import os 

# CSV 
class CsvWriter():
    # Jay test
    def wirte_data(self,filename,_str):
        try:
            # Write to .csv. 
            with open(filename,'a+') as f:
                f.write(_str)
                f.flush()
        except Exception as e :print(e)

    def wirte_data_info(self,filename,sarray):
        try:
            # Write to .csv. 
            with open(filename,'a+') as f:
                _str = str(sarray[0])+','+str(sarray[1])+','+str(sarray[2])+','+str(sarray[3])+','+str(sarray[4])+'\n'
                f.write(_str)
                f.flush()
        except Exception as e:print(e)

# INI
class MyConfigParser(ConfigParser):
    # Initial obj
    def __init__(self,defaults=None):
        ConfigParser.__init__(self,defaults=None)
    def optionxform(self, optionstr):
        return optionstr
    
# INI Writer
class INIWriter():    
    #region About setting default.
    # Property
    CFG_NAME = ''
    # Write default value to ini file.
    def write_default_INI(self):
        config = MyConfigParser()
        # Config ini basic parameter. (ID2)
        config['Page']={
            'PageNo'              : '99',                # Page Num
            'PageCmd'             : '2',                 # Page Cmd
        }

        # Config ini basic parameter. (ID2)
        config['Basic']={  
            's16MinTemp'          : '0',                 # SID = 1
            's16MaxTemp'          : '800',               # SID = 2
            'u16MaxCurrent'       : '30000',             # SID = 3
            'u16MaxPeakCurrent'   : '2500',              # SID = 4
            'u16TorqueSensorType' : '2',                 # SID = 5
            'u16MaxDutyCycle'     : '1000',              # SID = 6
            'u16MaxTorque'        : '1862',              # SID = 7
            'u16PWMFreq'          : '2',                 # SID = 8
            'u16MaxRPM'           : '2000',              # SID = 9
            'u16MaxSlope'         : '1000',              # SID = 10
            'u16MinBusVolt'       : '180',               # SID = 11
            'u16MaxBusVolt'       : '700',               # SID = 12
            'u16StartDutyCycle'   : '20',                # SID = 13
            'u16GearBoxRatio'     : '1111',              # SID = 14
            'u32StartInp'         : '2',                 # SID = 15    
            'u32RevInp'           : '1',                 # SID = 16
            'u16RevRpm'           : '1000',              # SID = 17
            'u16RevSlope'         : '1000',              # SID = 18
            'u16RevMaxCurrent'    : '30000',             # SID = 19
            'u16RevMaxTorque'     : '1862',              # SID = 20
            'u16ErrorIdleTime'    : '200',               # SID = 21
            'u16BackLash '        : '0',                 # SID = 22
            'u16PGain'            : '2000',              # SID = 23
            'u16IGain'            : '100',               # SID = 24
            'u16Encoder'          : '1',                 # SID = 25   
        }

        config['PROCESS'] = {         
            'u8ProcID'             : '4000',             # MID = 4XXX
            'u8ProcName'           : '1',                # SID = 1
            'u16ProcPGain'         : '1000',             # SID = 2         
            'u16ProcIGai'          : '100',              # SID = 3
            'u16ProcMaxTime'       : '0',                # SID = 4
            'u16ProcMinTime'       : '65000',            # SID = 5
            'u16ProcRevFunc'       : '0',                # SID = 6
            'u16NbrStep'           : '3',                # SID = 7
        }

        config['ProcStepIDList'] = {
            'Step_1'               : '3000',                   # Step 1
            'Step_2'               : '3001',                   # Step 2
            'Step_3'               : '3002',                   # Step 3
        }

        config['STEP_1'] = {            
            'step_ID'                 : '3000',                 # MAIN_ID = 3000
            'u8StepName'              : '1',                    # SID = 1
            'u16StepRpm'              : '100',                  # SID = 2
            'u16StepSlope'            : '1000',                 # SID = 3
            'u16StepMaxCurrent'       : '30000',                # SID = 4
            'u16StepMaxTorque'        : '3724',                 # SID = 5
            'u16StepMaxRevol'         : '1000',                 # SID = 6
            'u16StepTime'             : '500',                  # SID = 7
            'u16StepAngle'            : '3600',                 # SID = 8
            'u16StepAngleWindow'      : '1800',                 # SID = 9
            'u16StepTorqueWindow'     : '37',                   # SID = 10
            'u16MinDutyCycle'         : '30',                   # SID = 11 #30
            'u16StepFlags'            : '32785',                # SID = 12
            'u16StepAngleUpperLimite' : '0',                    # SID = 13
            'u16StepAngleLowerLimit'  : '0',                    # SID = 14
            'u16StepTorqueUpperLimit' : '0',                    # SID = 15
            'u16StepTorqueLowerLimit' : '0',                    # SID = 16
            'u16StepMonitorType'      : '0',                    # SID = 17
            'u16StepDelaytime'        : '0',                    # KLS Define 
            'u16StepTorqueThreshold'  : '0',                    # KLS Define ,0
            'u16StepAngleThreshold'   : '0',                    # KLS Define ,0.000
            'u16StepTorqueJoinOffset' : '0',                    # KLS Define ,0 
            'ScrewReverseForce'       : '0',                    # KLS Define ,1-100
        }
    
        config['STEP_2'] = {             
            'step_ID'                 : '3001',                 # MAIN_ID = 3001
            'u8StepName'              : '2',                    # SID = 1
            'u16StepRpm'              : '500',                  # SID = 2
            'u16StepSlope'            : '2000',                 # SID = 3
            'u16StepMaxCurrent'       : '30000',                # SID = 4
            'u16StepMaxTorque'        : '3724',                 # SID = 5
            'u16StepMaxRevol'         : '1000',                 # SID = 6
            'u16StepTime'             : '800',                  # SID = 7
            'u16StepAngle'            : '3600',                 # SID = 8
            'u16StepAngleWindow'      : '1800',                 # SID = 9
            'u16StepTorqueWindow'     : '37',                   # SID = 10
            'u16MinDutyCycle'         : '30',                   # SID = 11 #30
            'u16StepFlags'            : '32785',                # SID = 12
            'u16StepAngleUpperLimite' : '0',                    # SID = 13
            'u16StepAngleLowerLimit'  : '0',                    # SID = 14
            'u16StepTorqueUpperLimit' : '0',                    # SID = 15
            'u16StepTorqueLowerLimit' : '0',                    # SID = 16
            'u16StepMonitorType'      : '0',                    # SID = 17
            'u16StepDelaytime'        : '0',                    # KLS Define 
            'u16StepTorqueThreshold'  : '0',                    # KLS Define ,0
            'u16StepAngleThreshold'   : '0',                    # KLS Define ,0.000
            'u16StepTorqueJoinOffset' : '0',                    # KLS Define ,0 
            'ScrewReverseForce'       : '0',                    # KLS Define ,1-100
        }

        config['STEP_3'] = {          
            'step_ID'                 : '3002',                 # MAIN_ID = 3001
            'u8StepName'              : '3',                    # SID = 1
            'u16StepRpm'              : '100',                  # SID = 2
            'u16StepSlope'            : '1000',                 # SID = 3
            'u16StepMaxCurrent'       : '30000',                # SID = 4
            'u16StepMaxTorque'        : '1862',                 # SID = 5
            'u16StepMaxRevol'         : '1000',                 # SID = 6
            'u16StepTime'             : '1000',                 # SID = 7
            'u16StepAngle'            : '3600',                 # SID = 8
            'u16StepAngleWindow'      : '1800',                 # SID = 9
            'u16StepTorqueWindow'     : '37',                   # SID = 10
            'u16MinDutyCycle'         : '30',                   # SID = 11 #30
            'u16StepFlags'            : '8197',                 # SID = 12
            'u16StepAngleUpperLimite' : '0',                    # SID = 13
            'u16StepAngleLowerLimit'  : '0',                    # SID = 14
            'u16StepTorqueUpperLimit' : '0',                    # SID = 15
            'u16StepTorqueLowerLimit' : '0',                    # SID = 16
            'u16StepMonitorType'      : '0',                    # SID = 17
            'u16StepDelaytime'        : '0',                    # KLS Define 
            'u16StepTorqueThreshold'  : '0',                    # KLS Define ,0
            'u16StepAngleThreshold'   : '0',                    # KLS Define ,0.000
            'u16StepTorqueJoinOffset' : '0',                    # KLS Define ,0 
            'ScrewReverseForce'       : '0',                    # KLS Define ,1-100
        }

        config['CONTROL_TLG']={
            'u16Ctrlflags'        : '41984',                      # 
            'u16ControlMode'      : '1',                          # ID7,SID1 
            'u16WorkProc'         : '4000',                       # ID7,SID2
            'u16CtrlProgram'      : '1',                          # 
            'u16ManRpm'           : '1000',                       # ID8,SID1
            'u16ManSlope'         : '1000',                       # ID8,SID2
            'u16ManMaxCurrent'    : '372',                        # ID8,SID4
            'u16ManMaxTorque'     : '30000',                      # ID8,SID3
            'u16ManRpmMode'       : '10',                         # 
            'u8TMDControl'        : '15',                          # 
        }

        config['CSTART_INFO']={
            'TMDTempThreshold' : '6000',                       # TMD Temperature threshold.
            'ProcessDelayMS'   : '1',                           
        }

        # Write configparser data to ini file.
        with open(self.CFG_NAME,'w',encoding='cp950') as configfile:
            config.write(configfile)   
        
    # Read_ini_process
    def read_INI_basice_para(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        #for idx,key in enumerate(config['ID2']):_lst.insert(idx,config.getint('ID2',key))
        for key in config['Basic']:_lst.append(config.getint('Basic',key))
        return _lst

    # Read_ini_process
    def read_INI_process(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for idx,key in enumerate(config['PROCESS']):
            if idx==0:_lst.append(config.get('PROCESS',key)) 
            else:_lst.append(config.getint('PROCESS',key))
        return _lst
    
    # Read_ini_step 1.
    def read_INI_step_1(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for idx,key in enumerate(config['STEP_1']):
            if idx ==0:_lst.append(config.get('STEP_1',key))
            else:_lst.append(config.getint('STEP_1',key))
        return _lst
    
    # Read_ini_step 2.
    def read_INI_step_2(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for idx,key in enumerate(config['STEP_2']):
            if idx==0:_lst.append(config.get('STEP_2',key))
            else:_lst.append(config.getint('STEP_2',key))
        return _lst

    # Read_ini_step 2.
    def read_INI_step_3(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for idx,key in enumerate(config['STEP_3']):
            if idx==0:_lst.append(config.get('STEP_3',key))
            else:_lst.append(config.getint('STEP_3',key))
        return _lst

    # Read_ini_ctrl tlg.
    def read_INI_ctrl_tlg(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for key in config['CONTROL_TLG']:
            _lst.append(config.getint('CONTROL_TLG',key))
        return _lst

    # Read_ini_cyclic start info.
    def read_INI_cstart_info(self):
        _lst = []
        config = MyConfigParser()
        config.read(self.CFG_NAME)
        for key in config['CSTART_INFO']:
            _lst.append(config.getint('CSTART_INFO',key))
        return _lst
    
    def set_INI_Path(self,INIPath):self.CFG_NAME = INIPath
    #endregion

    #region INI defalut function.
    # Inital obj value.
    def __init__(self):
        pass
    
    # Read data from INI and return list formate.
    def read_INI_list(self,iniPath):
        _ini_lst = []
        cfg = MyConfigParser()
        cfg.read(iniPath)
        _section_lst = cfg.sections()
        # for key in cfg[_section_lst[0]]:_ini_lst.append([key,cfg.get(_section_lst[0],key)])
        for sec in _section_lst:
            for key in cfg[sec]:
               _ini_lst.append([sec,key,cfg.get(sec,key)])
        return _ini_lst
        
    # Read INI to json.
    def read_INI_to_json(self,INIPath):
        _json_str = ''                          # retuur jsion string.
        # t1 = perf_counter_ns()
        cfg = MyConfigParser()                  # Initial csv obj.
        cfg.read(INIPath)                       # Read all data from INI.
        # t2 = perf_counter_ns()
        # print("Read INI time-consuming =  %f (ms)"%((t2-t1)/10000000))
        _sec_lst = cfg.sections()               # Get section list from INI.
        _cfg_dict = dict.fromkeys(_sec_lst)     # Create a dictionary include all secssion in INI.     
        _JE = json.JSONEncoder()
        try:
            for sec in _sec_lst:                              #                 
                _key_lst  = cfg[sec].keys()
                _buf_dict = dict.fromkeys(_key_lst)
                for key in _key_lst:
                    try:
                        _value = int(cfg.get(sec,key).replace('"',""))
                    except Exception as e:
                        _value = cfg.get(sec,key).replace('"',"")
                    _buf_dict.update({key:_value})
                # _cfg_dict.update({sec:json.dumps(_buf_dict)})
                _cfg_dict.update({sec:_buf_dict})
            # _json_str = json.dumps(_cfg_dict)
            _json_str = _JE.encode(_cfg_dict)
        except Exception as e:
            print(e)

        return _json_str

    # Convert json to dictionary.
    def convert_json_to_dict(self,jStr):
        _JD = json.JSONDecoder()
        _dict = _JD.decode(jStr)
        return _dict

    # Convert dictionary to json
    def convert_dict_to_json(self,_dict):
        _JE = json.JSONEncoder()
        _json = _JE.encode(_dict)
        return _json
    
    #region Write data list to INI.
    def write_json_to_INI(self,INIPath,jStr):
        # t1 = perf_counter_ns()

        _write_ok = False                       # retuur jsion string.
        _Jobj = json.JSONDecoder()              # 
        _ini_dict = _Jobj.decode(jStr)          # 
        _cfg = MyConfigParser()                 # Initial csv obj.
        _cfg.read_dict(_ini_dict)           
        # print(type(_ini_dict)) 
        # Write configparser data to ini file.
        with open(INIPath,'w',encoding='cp950') as configfile:_cfg.write(configfile)        
        # t2 = perf_counter_ns()
        # print("Write dict to INI time-consuming = %f (ms)"%((t2-t1)/1000000))   
        return _write_ok
    #endregion
    #endregion

#region Tcs database Manager.
class GtcsDBPresenter(database.SqliteManager):
    # Public attribute. 
    # Private attribute.   
    __dict_data_type = {
        "date_time"             :"datetime",    #  
        "fasten_time"           :"real",        #
        "fasten_torque"         :"real",        #
        "fasten_uint"           :"text",        #
        "fasten_angle"          :"real",        #
        "fasten_status"         :"text",        # 
        "rpm"                   :"integer",     #
        "selected_job"          :"integer",     #
        "selected_sequence"     :"integer",     # 
        "selected_program"      :"integer",     # 
        "last_screw_count"      :"integer",     # 
        "max_screw_count"       :"integer",     # 
    }
    
    # Initial object.
    def __init__(self,db_name):
        super().__init__(db_name=db_name)
        self.set_type_dict(pdict=self.__dict_data_type)

# Gtcs ubs presenter.
class GtcsUSBPresenter():
    pass

# GtcsStorgaePresenter
class GtcsStorgaePresenter():
    # Initail obj.
    def __init__(self,db_name):
        self.db = GtcsDBPresenter(db_name=db_name)
        self.db.create_table(table_name="data")
    
    def cycle_storage(self):
        _str = "'"+datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S")+"'"
        _dict = {
            "date_time"             :_str,              # 
            "fasten_time"           :"10.0",            #
            "fasten_torque"         :"1.0",             #
            "fasten_uint"           :"'N-m'",           #
            "fasten_angle"          :"0.0",             #
            "fasten_status"         :"'OK'",            # 
            "rpm"                   :"100",             #
            "selected_job"          :"1",               #
            "selected_sequence"     :"1",               # 
            "selected_program"      :"5",               # 
            "last_screw_count"      :"6",               # 
            "max_screw_count"       :"9",               #
        }

        self.db.insert_data(table_name="data",pdict=_dict)

    # Select storage thread FSM.
    def select_storage_FSM(self,_dict):
        # Initial tlg object.
        _tlgp = tlgp.TlgPresenter()
        _csv = fp.CsvWriter()
        _mt = view.GtcsMonitor()
        # Read Page information.
        _page = _dict.get("Page")
        _page_no = _page["PageNo"]
        _page_cmd = _page["PageCmd"]

        _d_mcb = None
        if _page_no==0:
            if _page_cmd==1:
                _d_mcb = _dict
            elif _page_cmd==2:
                pass
        elif _page_no==1:                                              # UI page = tcsmain.php
            if _page_cmd==1:                                           # tcsmain initial parameter check error.
                pass
            elif _page_cmd==2:                                         # tcsmain ststem ready.
                _d_mcb = _dict
                if _tlgp.check_error_occure()==True:
                    if _tlgp.alarm_flags != _tlgp.get_actural_alarm_flags():
                        _str = _mt.get_actural_Info()+"\n"
                        _csv.wirte_data(filename=_file_name,_str=_str)
                        _tlgp.alarm_flags = _tlgp.get_actural_alarm_flags()
        elif _page_no==11:  #
            if _page_cmd==1:
                _tlgp.write_MCB_save_info(_dict=_dict)
                _ProgID = _dict.get("Program")["u8ProcID"]
                _StepID = _dict.get("Step")["u8StepID"]
                _d_mcb = _tlgp.read_MCB_save_dict(PageNo=_page_no,PageCmd=_page_cmd,ProgID=_ProgID,StepId=_StepID)
        elif _page_no==12:  #
            if _page_cmd==1:pass
        elif _page_no==13:                                             #
            if _page_cmd==1:
                _tlgp.write_MCB_save_info(_dict=_dict)
                _ProgID = _dict.get("Program")["u8ProcID"]
                _StepID = _dict.get("Step")["u8StepID"]
                _d_mcb = _tlgp.read_MCB_save_dict(PageNo=_page_no,PageCmd=_page_cmd,ProgID=_ProgID,StepId=_StepID)
        elif _page_no==21:  #
            if _page_cmd == 1:pass
        elif _page_no==22:  #
            if _page_cmd == 1:pass
        elif _page_no==23:  #
            if _page_cmd == 1:pass
        elif _page_no==24:  #
                if _page_cmd == 1:pass
        elif _page_no==25:  #
                if _page_cmd == 1:pass
        return _d_mcb
    
#endregion
