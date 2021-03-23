#!/usr/bin/python
# -*- coding: UTF-8 -*-
from enum import Enum
from ctypes import cdll,c_uint8,POINTER
from platform import system 
from functools import singledispatch
from module import common as comm
from module import gtcsdata as gdt
import os 

try:
    link_so = cdll.LoadLibrary
    _ckeck_sys = system() 
    if _ckeck_sys =="Windows":
        path_breach = "\module\pycall\winlibpycallclass.so"
        _str_path = os.path.abspath(".")+path_breach
    elif _ckeck_sys == "Linux":
        path_breach = "/home/kls/project/gtcs/module/pycall/tcslibpycallclass.so"
        _str_path = path_breach
    # _str_path = os.path.abspath(".")+path_breach
    if os.path.exists(_str_path):
        pycall = link_so(_str_path)
    else:
        pass
except Exception as e:
    print(e)

#region basic type
#region TcsProrocoal
class TcsProtocol():
    # MAIN_ID = 1
    class IdentificationPara():
        # Public Enum
        class MainID(Enum):MAIN_ID = 1
        #Initial object.
        def __init__(self):        
            # property
            self.dict={
                "u8MotSWVer" : '',   # SID = 1,Software Version Motorcontrolle.
                "u8MotCoreID" : '',   # SID = 2,Unique Core ID from Motor- STM32 Conrtroller.
                "u8MotSerNr" : '',    # SID = 3,Serial Number of the Motor Controller.
                "u8DmsSWVer" : '',    # SID = 4,Software Version DMS- Controller.
                "u8DmsCorel" : '',    # SID = 5,Unique Core ID from DMS- STM32.
                "u8DmsSerNr" : '',    # SID = 6,Serial Number of the DMS- Controller.
            }
            self.key_list = list(self.dict.keys())
     
    # MAIN_ID = 2  
    class BasicPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 2
        #Initial object.
        def __init__(self):
            # public property.
            self.dict  = {
                "s16MinTemp" : 0,         # SID = 1,Minimal Temperature of the motor and the motorcontroller. 
                                          # Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
                "s16MaxTemp" : 0,         # SID = 2,Maximal Temperature of the motor and the motorcontroller. 
                                          # Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
                "u16MaxCurrent" : 0,      # SID = 3,This is the absolute maximum motor current value. 
                                          # There are also maximum current values for the specific process steps 
                                          # (see….), but this parameter is the limit. Unit is [mA].
                "u16MaxPeakCurrent" : 0,  # SID = 4,This value controls the threshold of the peak current comparator.
                                          # The peak current detection is built in for the protection of the device 
                                          # when a latch up situation occurs. Unit is [A].
                "u16TorqueSensorType" : 0,# SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
                "u16MaxDutyCycle" : 0,    # SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]
                "u16MaxTorque" : 0,       # SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
                "u16PWMFreq" : 0,         # SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
                "u16MaxRPM" : 0,          # SID = 9,This is the absolute maximum motor rpm value. 
                                          # There are also rpm values for the specific process steps (see….) , 
                                          # but this parameter is the limit. Unit is [rpm].
                "u16MaxSlope" : 0,        # SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                          # There are also rpm slope values for the specific process steps (see….) , 
                                          # but this parameter is the limit. Unit is [rpm/s].
                "u16MinBusVolt" : 0,      # SID = 11,Minimal Bus Voltage of the Power Supply. 
                                          # Underneath this Voltage the tool doesn’t work. 
                                          # Unit is [0,1V] (600 = 60V).
                "u16MaxBusVolt" : 0,      # SID = 12,Maximal Bus Voltage of the Power Supply. 
                                          # Above this Voltage the tool doesn’t work. Unit is [0,1V].
                "u16StartDutyCycle" : 0,  # SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].
                                          # (20 = 10%)
                "u16GearBoxRatio" : 0,    # SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
                "u32StartInp" : 0,        # SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
                "u32RevInp" : 0,          # SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
                "u16RevRpm" : 0,          # SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
                "u16RevSlope" : 0,        # SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
                "u16RevMaxCurrent" : 0,   # SID = 19,"Maximum Current of reverse drive. Unit is [mA]."
                "u16RevMaxTorque" : 0,    # SID = 20,"Maximum Torque of reverse drive. 
                                          # Value is 0- 1862 (max TMD Raw Value)"
                "u16ErrorIdleTime" : 0,   # SID = 21,"Idle time of the motor controller after a Error condition.
                                          # Unit is [ms]."
                "u16BackLash" : 0,        # SID = 22,"Backlash of the gearbox. The value depends if a Encoder or 
                                          # Hallsensors are used for angle Positioning. Unit is [Increments]."
                "u16PGain" : 0,           # SID = 23,Proportional Gain for the RPM Regulator.
                "u16IGain" : 0,           # SID = 24,Integral Gain for the RPM Regulator.
                "u16Encoder" : 0,         # SID = 25,"0 = No Encoder (positioning with Hallsensors). 
                                          # 1 = 200p Encoder"
            }                         
            self.key_list = list(self.dict.keys())

    # MAIN_ID = 3XXX
    class StepPara():
        # Public Enum
        class MainID(Enum):MAIN_ID = 3

        #Initial object.
        def __init__(self):
            self.dict={
                # Step name.
                "u8StepID" : 0,           # 3XXX
                "u8StepName" : '',        # SID = 1,32 byte long string which contains the name of the step.
                # property.
                "u16StepRpm" : 0,         # SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
                "u16StepSlope" : 0,       # SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
                "u16StepMaxCurrent" : 0,  # SID = 4,Maximum current of this step. Unit is [mA].
                "u16StepMaxTorque" : 0,   # SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
                "u16StepMaxRevol" : 0,    # SID = 6,Maximum Revolutions (after the Gearbox) of this step. 
                                          # Unit is [0,01] (1000 = 10,00 Revolutions)
                "u16StepTime" : 0,        # SID = 7,Execution Time- time. Unit is [ms].
                "u16StepAngle" : 0,       # SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
                "u16StepAngleWindow" : 0, # SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
                "u16StepTorqueWindow":0,  # SID = 10,Window of the torque monitoring. 
                                          # Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
                "u16MinDutyCycle" : 0,    # SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)
                "u16StepFlags" : 0,       # SID = 12,See description of step flags.

                # New 
                "u16StepAngleUpperLimit":0, # u16StepAngleUpperLimit
                "u16StepAngleLowerLimit":0,
                "u16StepTorqueUpperLimit":0,
                "u16StepTorqueLowerLimit":0,
                "u16StepMonitorType":0,

                # KLS Define
                "u16StepDelaytime":0,
            }            
            # Initial dictionary.
            self.key_list = list(self.dict.keys())

    # Main ID = 4XXX
    class ProcessPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1       
        # Initial object. 
        def __init__(self):
            # Initial step id dictionary.
            self.u16ProcStepId = {
                "step_1" : 0,
                "step_2" : 0,
                "step_3" : 0,
                "step_4" : 0,
                "step_5" : 0,
                "step_6" : 0,
                "step_7" : 0,
                "step_8" : 0,
            }
            # Initial Process dictionary.
            self.dict = {
                "u8ProcID" : 0,       # Process ID
                # Process name.
                "u8ProcName" : '',    # SID = 1,"32 byte long string which contains the name of the 
                                        # screwing process."
                # Process parameter.
                "u16ProcPGain" : 0,   # SID = 2,Proportional Gain of the screwing process.          
                "u16ProcIGain" : 0,   # SID = 3,Integral Gain of the screwing process.    
                "u16ProcMaxTime" : 0, # SID = 4,"0 = No maximum process time.
                                    # Every other value in [ms] is the timemonitoring of this process.
                                    # It leads to an error, when the process exceeds this time."
                "u16ProcMinTime" : 0, # SID = 5,"0 = No minimum process time.
                                    # Every other value in [ms] is the time monitoring of this process. 
                                    # It leads to an error, when the process undershoots this time."
                "u16ProcRevFunc" : 0, # SID = 6,"0 = Screwing direction of the whole process normal 
                                    # (tightening right) 1 = Screwing direction of the whole process 
                                    # reverse (tightening left)"
                "u16NbrSteps" : 0,    # SID = 7,Number of Steps in this Process

                # Stepo list in porcess,SID = 8,"Screwing process step ID Number 3000 -3499"
                # "u16ProcStepId":self.u16ProcStepId,    
            }
            self.key_list = list(self.dict.keys())
            self.step_id_list = list(self.u16ProcStepId.keys())

    # MAIN ID = 5
    class ProgramPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1
        # Initial object. 
        def __init__(self):
            # property.
            # Initial step id dictionary.
            self.u16ProgProcessId = {
                "Proc_1" : 0,
                "Proc_2" : 0,
                "Proc_3" : 0,
                "Proc_4" : 0,
                "Proc_5" : 0,
                "Proc_6" : 0,
                "Proc_7" : 0,
                "Proc_8" : 0,
            }

            self.dict = {
                "u8ProgName" : 0,         # SID = 1,32 byte long string which contains the name of the Program.
                "u16ProgResFunc" : 0,     # SID = 2,"0 = No Function 
                                          # 1 = a change of the reset flag resets the actual process.
                                          # 2 = a change of the reset flag resets the actual program".  
                "u16ProgRevFunc" : 0,     # SID = 3,"0 = No Reverse Drive
                                          # 1 = Reverse Drive is possible
                                          # 2 = Reverse Drive is possible and resets the last process".
                "u16ProgRevTime" : 0,     # SID = 4,Screwing Program Reverse Drive Time.
                "u16ProgErrResp" : 0,     # SID = 5,"0 = Wait for acknowledge, 1 = No waiting"
                "u16NbrProc" : 0,         # SID = 6,Number of processes in this program.
                # SID = 1000-1199,Screwing Process ID Number 4000 - 4249.
                # "u16ProgProcessId"  : self.u16ProgProcessId 
            }            

            self.key_list = list(self.dict.keys())
            
    # MAIN ID = 6
    class StatistcPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1
        # Initial object. 
        def __init__(self):
            # u32RevCountList
            self.u32RevCountList={
                "Proc_1" : 0,
                "Proc_2" : 0,
                "Proc_3" : 0,
                "Proc_4" : 0,
                "Proc_5" : 0,
                "Proc_6" : 0,
                "Proc_7" : 0,
                "Proc_8" : 0,
            }

            # property
            self.dict = {
                "u32StartCount" : 0,      # SID = 1,Counter for Process Start.
                "u32TighteningCount" : 0, # SID = 2,Counter for Process Tightening.
                "u32IMaxCount" : 0,       # SID = 3,Counter for Overcurrent Conditions.
                "u32tMaxCount" : 0,       # SID = 4,"Counter for Overtemperature Conditions".
                "u32RevCount" : 0,        # SID = 5,Counter for Reverse Operations.
                # SID = 6,Last 10 Screwdriver Errors (see Errorlist….). 
                # "u32RevCountList":self.u32RevCountList  
            }

    # MAIN ID = 7
    class CtrlPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1
        # Initial object. 
        def __init__(self):
            # Property.
            self.dict = {
                "u16ControlMode" : 0,     # SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                          # 3= PWM Mode,4=Para Write Mode"
                "u16WorkProc" : 0,        # SID = 2,It is the ID of the actual working process. 
                                          # This is relevant when Control Mode = Process Mode.
                "u16WritePara2Flash" : 0, # SID = 3,Reserved because the Parameters are stored in FRAM.
                "u16WriteStep2Flash" : 0, # SID = 4,"When receiving this Parameter the selected Step will be 
                                          # written to Flash Memory.
                                          # Value of this Parameter the ID of the Step (ID3XXX)".
                "u16WriteProc2Flash" : 0, # SID = 5,"When receiving this Parameter the selected Process will be 
                                          # written to Flash Memory.
                                          # Value of this Parameter the ID of the Process (ID4XXX)".
                "u16WriteProg2Flash" : 0, # SID = 6,"When receiving this Parameter the Program will be written 
                                          # to Flash Memory.Value of this Parameter is “Don't care”".
            }
            self.key_list = list(self.dict.keys())
    # Main_ID = 8
    class ManualPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1
        # Initial object. 
        def __init__(self):
            # Property.
            self.dict = {
                "u16ManRpm" : 0,          # SID = 1, Manual Rpm. Unit is [rpm] (after the Gearbox).
                "u16ManSlope" : 0,        # SID = 2, Manual slope. Unit is [rpm/s] (after the Gearbox).
                "u16ManMaxCurrent" : 0,   # SID = 3, Manual maximum current. Unit is [mA].
                "u16ManMaxTorque" : 0,    # SID = 4, Maximum Torque Value is 0- 1862 (max Raw TMD Value). 
            }
            self.key_list = list(self.dict.keys())

    # Main_ID = 9
    class StatusPara():
        # public Enum
        class MainID(Enum):MAIN_ID = 1
        # Initial object. 
        def __init__(self):
            # Property
            self.dict = {
                "u16ActError" : 0,        # SID = 1, Actual ErrorFlags of the Screwdriver.
                "u16ActProcID" : 0,       # SID = 2, Actual working Process ID. (4000- 4199).
                "u16ActProcNr" : 0,       # SID = 3, Actual working Process Number. (1- 200).
                "u16ActStepID" : 0,       # SID = 4, Actual working Step ID. (3000 - 3499).
                "u16ActStepNr" : 0,       # SID = 5, Actual working Step Nr. (1- 20).
                "u16ActCurr" : 0,         # SID = 6, Actual motor current [mA].
                "u16ActRPM" : 0,          # SID = 7, Actual motor rpm [rpm].
                "u16ActTorque" : 0,       # SID = 8, Actual Torque Value is 0- 1862 (max Raw TMD Value).
                "u16ActBusVolt" : 0,      # SID = 9, Actual Bus Voltage [0,01V] (6000 = 60V).
                "u16ActContTemp" : 0,     # SID = 10, Actual motorcontroller temperature [0,1°C].
                "u16ActMotTem" : 0,       # SID = 11, Actual motor temperature [0,1°C].
                "u16MaxCurrent" : 0,      # SID = 12, Actual maximum current of the process [mA].
                "u16MaxTorqu":0,        # SID = 13, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
                "u32Angle" : 0,           # SID = 14, Actual angle [0,1°].
                "u32Revolutions" : 0,     # SID = 15, Actual revolutions [0,01 revolutions].
            }
            self.key_list = list(self.dict.keys())
            
            self.u16ActErrorList = [None]*32 
            
            # Initial error list.
            for idx in range(len(self.u16ActErrorList)): self.u16ActErrorList[idx] = 0
    
    # public proporty
    identification_para = IdentificationPara()   # MAIN ID = 1
    basic_para = BasicPara()                     # MAIN ID = 2
    step_para = StepPara()                       # MAIN ID = 3
    process_para = ProcessPara()                 # MAIN ID = 4
    program_para = ProgramPara()                 # MAIN ID = 5
    statistc_para = StatistcPara()               # MAIN ID = 6           
    ctrl_para  = CtrlPara()                      # MAIN ID = 7
    manual_para = ManualPara()                   # MAIN ID = 8
    status_para = StatusPara()                   # MAIN ID = 9
    
    # Creat signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance 
    # Initial signleTon object. 
    def __init__(self):pass
    
#endregion

#region TcsTelegram
class TlgStructure():
    #region private class
    # Tlg structure object
    class TlgStructureBasic():
        #region private class 
        # header
        class Header():
            # private propority
            htype = 0
            para_address_2 = 0
            para_address_3 = 0
            para_address_4 = 0
            para_address_5 = 0
            para_address_6 = 0

            write_status = 0
            write_ok = 0

            donot_care = 0
            array_address = 0
            step_address = 0

            # public propority        
            array=[None]*8
    
            def __init__(self):
                super().__init__()
                self.initial_array()

            # Initial header array.
            def initial_array(self):
                _len = len(self.array)
                for idx in range(_len):self.array[idx]=0  
        #payload
        class Payload():
            # private  
            array = [None]*36    
            # public method.
            def __init__(self):
                self.initial_array()
    
            # Initial header array.
            def initial_array(self):
                _len = len(self.array)
                for idx in range(_len):self.array[idx]=0
        #endregion

        # Public Property.
        header = Header()
        payload = Payload()
        tlg_array = []
        # Private Property.
        __last_idx = 0
        pdata=[]

        #region public property.
        #region public method     
        def encode_harray(self):
            return self.header.array
        def encode_parray(self):
            return self.header.array
        def encode_tlg_array(self):
            # encode header array.
            self.encode_harray()
            # encode payload array.
            self.encode_parray()
            # Calaulate crc
            _buffer = self.header.array+self.payload.array+[0,0,0,0]
            pycall.calculateCrc32((c_uint8 * 48)(*_buffer))
            f = pycall.getCrc32Value_all 
            f.restype=POINTER(c_uint8 * 4)
            _arr = f().contents
            # Combin tlg array.
            self.tlg_array = self.header.array+self.payload.array
            self.tlg_array += [_arr[0],_arr[1],_arr[2],_arr[3]]
            # return tlg array.
            return self.tlg_array
        
        # Ckeck crc32 status.
        def check_crc32_array(self,tlg):
            check_ok=False
            try:
                if pycall.checkCrc32((c_uint8 * 48)(*tlg))==1:check_ok=True
            except Exception as e:
                check_ok = False
                print(e)
            return check_ok
            
        def decode_harray(self,tlg): return False
        def decode_parray(self,tlg): return False
        def decode_tlg_array(self,tlg): return False

        # Clear parameter address.
        def clear_para_address(self):
            self.header.para_address_2 = 0
            self.header.para_address_3 = 0
            self.header.para_address_4 = 0
            self.header.para_address_5 = 0
            self.header.para_address_6 = 0
        
        # Clear parameter array.
        def clear_para_array(self):
            self.clear_para_address()
            self.header.initial_array()
            self.payload.initial_array()
            self.pdata.clear()
            self.__last_idx = 0

        # Set RW address data. 
        def set_TLG_RW_addr(self,PAddr,MID,SID,data,dlengths):
            _set_ok = False
            _comm = comm.CommonFunction()
            self.pdata += _comm.convert_to_list(MID,2)
            self.pdata += _comm.convert_to_list(SID,2)
            self.pdata += _comm.convert_to_list(data,dlengths)        
            _idx = len(self.pdata)
            
            if (self.__last_idx<=36):
                if PAddr == 0:
                    self.__last_idx = _idx
                elif PAddr == 1:
                    self.header.para_address_2 = self.__last_idx
                    self.__last_idx = _idx
                elif PAddr == 2:
                    self.header.para_address_3 = self.__last_idx
                    self.__last_idx = _idx
                elif PAddr == 3:
                    self.header.para_address_4 = self.__last_idx
                    self.__last_idx = _idx
                elif PAddr == 4:
                    self.header.para_address_5 = self.__last_idx
                    self.__last_idx = _idx
                elif PAddr == 5:
                    self.header.para_address_6 = self.__last_idx
                    self.__last_idx = _idx
                else:pass
                _set_ok = True
            return _set_ok
        #endregion
        
    # type = 1
    class CtrlTlg(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # type num.
            self.type_num = 1
            # Telegram property.
            # Initial ctrl tlg.
            self.dict = dict()

            # Fasten ctrl telegram.   
            self.fasten_dict = {
                gdt.CtrlTlgEnum.CTRL_FLAGS.value : 9216,  # u16Ctrlflags,42496,8192,9728(鎖馬達),Flag10=Cyc
                gdt.CtrlTlgEnum.CTRL_MOD.value : 1,       # u16ControlMode
                gdt.CtrlTlgEnum.WORK_PROC.value : 4248,   # u16WorkProc = 4248
                gdt.CtrlTlgEnum.CTRL_PROG.value : 1,      # u16CtrlProgram
                gdt.CtrlTlgEnum.MAN_RPM.value : 1000,     # u16ManRpm
                gdt.CtrlTlgEnum.MAN_SLOP.value : 1000,    # u16ManSlope
                gdt.CtrlTlgEnum.MAX_TORQUE.value : 1862,  # u16ManMaxTorque
                gdt.CtrlTlgEnum.MAX_CURRENT.value : 30000,# u16ManMaxCurrent
                gdt.CtrlTlgEnum.MAN_RPM_MOD.value : 0,    # u16ManRpmMode
                gdt.CtrlTlgEnum.TMD_CTRL.value : 15,      # u8TMDControl
            }
            # Loosen ctrl telegram.
            self.loosen_dict = {
                gdt.CtrlTlgEnum.CTRL_FLAGS.value : 9216,  # u16Ctrlflags,42496,8192,9728(鎖馬達),Flag10=Cyc
                gdt.CtrlTlgEnum.CTRL_MOD.value : 1,       # u16ControlMode
                gdt.CtrlTlgEnum.WORK_PROC.value : 4249,   # u16WorkProc = 4249
                gdt.CtrlTlgEnum.CTRL_PROG.value : 1,      # u16CtrlProgram
                gdt.CtrlTlgEnum.MAN_RPM.value : 1000,     # u16ManRpm
                gdt.CtrlTlgEnum.MAN_SLOP.value : 1000,    # u16ManSlope
                gdt.CtrlTlgEnum.MAX_TORQUE.value : 1862,  # u16ManMaxTorque
                gdt.CtrlTlgEnum.MAX_CURRENT.value : 30000,# u16ManMaxCurrent
                gdt.CtrlTlgEnum.MAN_RPM_MOD.value : 0,    # u16ManRpmMode
                gdt.CtrlTlgEnum.TMD_CTRL.value : 15,      # u8TMDControl
            }
            # Self leaning ctrl telegram.
            self.SL_dict = {
                gdt.CtrlTlgEnum.CTRL_FLAGS.value : 9216,  # u16Ctrlflags,42496,8192,9216,9729,Flag10=Cyc
                gdt.CtrlTlgEnum.CTRL_MOD.value : 5,       # u16ControlMode
                gdt.CtrlTlgEnum.WORK_PROC.value : 4000,   # u16WorkProc = 4248
                gdt.CtrlTlgEnum.CTRL_PROG.value : 1,      # u16CtrlProgram
                gdt.CtrlTlgEnum.MAN_RPM.value : 1000,     # u16ManRpm
                gdt.CtrlTlgEnum.MAN_SLOP.value : 1000,    # u16ManSlope
                gdt.CtrlTlgEnum.MAX_TORQUE.value : 1862,  # u16ManMaxTorque
                gdt.CtrlTlgEnum.MAX_CURRENT.value : 30000,# u16ManMaxCurrent
                gdt.CtrlTlgEnum.MAN_RPM_MOD.value : 0,    # u16ManRpmMode
                gdt.CtrlTlgEnum.TMD_CTRL.value : 15,      # u8TMDControl
            }
            # 
            self.key_list = list(self.fasten_dict.keys())     
            self.__ctrl_flags = 0
            self.__TMD_flags = 0   
        
        # Config control status.
        def initial_ctrl_flags(self):
            """
            docstring
            """ 
            self.__ctrl_flags = 0   
            self.fasten_dict.update({gdt.CtrlTlgEnum.CTRL_FLAGS.value:self.__ctrl_flags})
        
        def set_ctrl_flags(self,ctrl_flags):  # Set ctrl flags.
            """
            docstring
            """
            self.fasten_dict.update({gdt.CtrlTlgEnum.CTRL_FLAGS.value:ctrl_flags})
        
        def set_ctrl_flag(self,idx:int):    # Set ctrl flag. 
            """
            docstring
            """   
            target_dict = "fasten"
            if target_dict == "fasten":
                self.fasten_dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value] |= 1<<idx
            elif target_dict == "loosen":
                self.loosen_dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value] |= 1<<idx
            elif target_dict == "SL":
                self.SL_dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value] |= 1<<idx
            else:
                print("Pls input current targert dict discrupt!!")
        
        def set_SL_ctrl_flag(self,idx:int):    # Set ctrl flag. 
            """
            docstring
            """   
            self.SL_dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value] |= 1<<idx
        
        def clear_SL_ctrl_flag(self,idx:int):    # Set ctrl flag. 
            """
            docstring
            """   
            self.SL_dict[gdt.CtrlTlgEnum.CTRL_FLAGS.value] = 0<<idx
            
        def set_max_torque(self,target_dict:str,torque:float): # Set self leaning target torque.
            """
            docstring
            """
            if target_dict == "fasten":
                self.fasten_dict[gdt.CtrlTlgEnum.MAX_TORQUE.value] = torque
            elif target_dict == "loosen":
                self.loosen_dict[gdt.CtrlTlgEnum.MAX_TORQUE.value] = torque
            elif target_dict == "SL":
                self.SL_dict[gdt.CtrlTlgEnum.MAX_TORQUE.value] = torque
            else:
                print("Pls input current targert dict discrupt!!")
        
        def set_max_current(self,target_dict:str,current:float):  # Set dict current.
            """
            docstring
            """
            if target_dict == "fasten":
                self.fasten_dict[gdt.CtrlTlgEnum.MAX_CURRENT.value] = current
            elif target_dict == "loosen":
                self.loosen_dict[gdt.CtrlTlgEnum.MAX_CURRENT.value] = current
            elif target_dict == "SL":
                self.SL_dict[gdt.CtrlTlgEnum.MAX_CURRENT.value] = current
            else:
                print("Pls input current targert dict discrupt!!")

        def initial_TMD_value(self):    # Config TMD status.
            """
            docstring
            """
            self.__TMD_flags = 0      
            self.fasten_dict.update({gdt.CtrlTlgEnum.TMD_CTRL.value:self.__TMD_flags})

        # encode header array.
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.donot_care
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array

        # encode payload array.
        def encode_parray(self):
            _ctrl_tlg_array=list(self.dict.values())
            for idx,val in enumerate(_ctrl_tlg_array):
                self.payload.array[idx*2]=val%256
                self.payload.array[idx*2+1]=val//256
            return self.payload.array    
    # type = 2
    class StatusTlg(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # public property 
            self.type_num =2

            # Error meaagse list.
            self.__err_msg_list={
                gdt.ErrMsg.CONT_TEMP.value,           #flag_00
                gdt.ErrMsg.MOT_TEMP.value,            #flag_01
                gdt.ErrMsg.MOT_CURR.value,            #flag_02
                gdt.ErrMsg.MOT_PEAK_CURR.value,       #flag_03
                gdt.ErrMsg.MOT_TORQUE.value,          #flag_04
                gdt.ErrMsg.DEADLOCK.value,            #flag_05
                gdt.ErrMsg.PROC_MINTIME.value,        #flag_06
                gdt.ErrMsg.PROC_MAXTIME.value,        #flag_07
                gdt.ErrMsg.ENCODER.value,             #flag_08
                gdt.ErrMsg.H_ALL.value,               #flag_09
                gdt.ErrMsg.BUSVOLTAGE_HIGH.value,     #flag_10
                gdt.ErrMsg.BUSVOLTAGE_LOW.value,      #flag_11
                gdt.ErrMsg.PROG_NA.value,             #flag_12
                gdt.ErrMsg.PROC_NA.value,             #flag_13
                gdt.ErrMsg.STEP_NA.value,             #flag_14
                gdt.ErrMsg.DMS_COMM.value,            #flag_15
                gdt.ErrMsg.FLASH.value,               #flag_16
                gdt.ErrMsg.FRAM.value,                #flag_17
                gdt.ErrMsg.ANGLE.value,               #flag_18
                gdt.ErrMsg.REVOLUTIONS.value,         #flag_19
                gdt.ErrMsg.PROTECT_CIRCUIT.value,     #flag_20
                gdt.ErrMsg.SWITCH_CONFIG.value,       #flag_21
                gdt.ErrMsg.DISPLAY_COMM_TO.value,     #flag_22
            }
            # Telegram property. 
            self.dict = { 
                gdt.StatusTlgEnum.STATUS_FLAGS.value : 0,  # u16Statusflags
                gdt.StatusTlgEnum.ACT_ERR.value :0,        # u16ActError
                gdt.StatusTlgEnum.ACT_PROCESS.value : 0,   # u16ActProcNr
                gdt.StatusTlgEnum.ACT_STEP.value : 0,      # u16ActStepNr
                gdt.StatusTlgEnum.ACT_CURRENT.value : 0,   # u16ActCurr
                gdt.StatusTlgEnum.ACT_TORQUE.value : 0,    # u16ActTorque
                gdt.StatusTlgEnum.ACT_RPM.value : 0,       # u16ActRPM
                gdt.StatusTlgEnum.MAX_CURRENT.value : 0,   # u16MaxCurrent
                gdt.StatusTlgEnum.MAX_TORQUE.value : 0,    # u16MaxTorque
                gdt.StatusTlgEnum.ACT_ANGLE.value : 0,     # u32Angle
                gdt.StatusTlgEnum.ACT_REVOLUTION.value : 0,# u32Revolutions
                gdt.StatusTlgEnum.TMD_FLAGS.value : 0,     # u16TMDFlags
                gdt.StatusTlgEnum.TMD_TMP.value : 0,       # s16Debug
                gdt.StatusTlgEnum.U32_DEBUG.value : 0,     # s32Debug
            }

            self.__gear_box_ratio = 0

            self.status_flg_list = []
            self.error_flg_list = []
            self.TMD_flags_list = [0,0,0,0]
            
            self.first_start_flag = False
            self.restart_flag = False
            self.error_occure = False
            self.ack_flag = False
            self.ack_cnt = 0
            self.actual_MRT_status = 0
            self.first_start_flg = False
            self.restart_flg  = False

        # Decode harry.
        def decode_harray(self):
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.donot_care
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
        
        # Decode parray.
        def decode_parray(self,parray):
            _comm = comm.CommonFunction()
            self.dict["u16Statusflags"] = _comm.convert_to_value(parray[0:2])   # u16Statusflags
            self.dict["u16ActError"] = _comm.convert_to_value(parray[2:6])      # u16ActError
            self.dict["u16ActProcNr"] = _comm.convert_to_value(parray[6:8])     # u16ActProcNr
            self.dict["u16ActStepNr"] = _comm.convert_to_value(parray[8:10])    # u16ActStepNr
            self.dict["u16ActCurr"] = _comm.convert_to_value(parray[10:12])     # u16ActCurr
            self.dict["u16ActTorque"] = _comm.convert_to_value(parray[12:14])   # u16ActTorque
            self.dict["u16ActRPM"] = _comm.convert_to_value(parray[14:16])      # u16ActRPM
            self.dict["u16MaxCurrent"] = _comm.convert_to_value(parray[16:18])  # u16MaxCurrent
            self.dict["u16MaxTorque"] = _comm.convert_to_value(parray[18:20])   # u16MaxTorque
            self.dict["u32Angle"] = int.from_bytes(bytes=parray[20:24],         # u32Angle,"big"
                                                  byteorder="little",
                                                  signed=True)       
            self.dict["u32Revolutions"] = int.from_bytes(bytes=parray[24:28],   # u32Revolutions
                                                        byteorder="little",
                                                        signed=True) 
            self.dict["u16TMDFlags"] = _comm.convert_to_value(parray[28:30])    # u16TMDFlags
            self.dict["s16Debug"] = _comm.convert_to_value(parray[30:32])       # s16Debug
            self.dict["s32Debug"] = _comm.convert_to_value(parray[32:36])       # s32Debug

        def decode_tlg_array(self,tlg):
            _decode_ok =False
            _comm = comm.CommonFunction() 
            if (self.check_crc32_array(tlg)==True):
                #self.header.array=tlg[0:8]
                #self.payload.array=tlg[8:44]
                self.decode_parray(tlg[8:44])
                self.status_flg_list = _comm.convert_to_bit_list(16,self.dict["u16Statusflags"])
                self.error_flg_list = _comm.convert_to_bit_list(32,self.dict["u16ActError"])
                self.TMD_flags_list = _comm.convert_to_bit_list(4,self.dict["u16TMDFlags"])
                _decode_ok =True
            else:
                print("Decode tlg state Error!")
            return _decode_ok 
        
        def set_gear_box_ratio(self,gear):self.__gear_box_ratio = gear
        def get_status_flag(self):return self.status_flg_list
        def get_TMD_status(self,idx):return self.TMD_flags_list[idx]
        def get_error_status(self,Idx):return self.error_flg_list[Idx]
    # type = 3
    class ParaWirteRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            self.type_num =3                           # Header type Num = 3.
            # private proporty
        
        # Encode header array.
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.para_address_2
            self.header.array[2] = self.header.para_address_3
            self.header.array[3] = self.header.para_address_4
            self.header.array[4] = self.header.para_address_5
            self.header.array[5] = self.header.para_address_6
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
        
        # Encoder payload array.
        def encode_parray(self):
            _Para_len = len(self.pdata)
            for idx,val in enumerate(self.payload.array):
                if (idx <_Para_len): 
                    self.payload.array[idx] = self.pdata[idx]
                else:
                    self.payload.array[idx] = 0
    # type = 4
    class ParaReadRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Header type Num.
            self.type_num =4
            # private proporty

        # Encode head array telegram.
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.para_address_2
            self.header.array[2] = self.header.para_address_3
            self.header.array[3] = self.header.para_address_4
            self.header.array[4] = self.header.para_address_5
            self.header.array[5] = self.header.para_address_6
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
    
        def encode_parray(self):
            _Para_len = len(self.pdata)        
            for idx,val in enumerate(self.payload.array):
                if (idx <_Para_len): 
                    self.payload.array[idx] = self.pdata[idx]
                else:
                    self.payload.array[idx] = 0
    # type = 5
    class ParaResponse(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # public property
            self.type_num = 5                     # Header type Num = 5.
            # Para length.
            self.__decode_type = 0

            self.__harray = [None]*6
            self.__parray = [None]*6
            self.__parray_str = ""
            self.__parray_int = 0
        
        def set_decode_type(self,type_num):
            self.__decode_type = type_num

        # Decode harray method.
        def decode_harray(self,harray):
            self.__harray[0] = 0
            self.__harray[1] = harray[1]
            self.__harray[2] = harray[2]
            self.__harray[3] = harray[3]
            self.__harray[4] = harray[4]
            self.__harray[5] = harray[5]

        # Decode parray method.
        def decode_parray(self,parray):    
            _comm = comm.CommonFunction()
            for idx,val in enumerate(self.__harray):
                self.__parray[idx] = _comm.convert_to_value(parray[val+4:val+6])

        # Decode str parray.
        def decode_str_parray(self,parray):
            _list = parray[4:]
            self.__parray_str = ""
            for val in _list:
                self.__parray_str += chr(val)
        
        # Decode str parray.
        def decode_int_parray(self,parray):
            _list = parray[4:8]
            self.__parray_int = 0
            for idx,val in enumerate(_list):
                self.__parray_int += (lambda i,j: i<<8*j)(val,idx)                      

        # Decode tlg array.
        def decode_tlg_array(self,tlg):              
            self.decode_harray(tlg[0:8])
            _type = self.__decode_type
            if _type==0:
                self.decode_parray(tlg[8:44])
            elif _type==1:
                self.decode_str_parray(tlg[8:44])
            elif _type==2:
                self.decode_int_parray(tlg[8:44])            
            return True

        # Get parameter array .
        def get_para_array(self):
            return self.__parray

        # Get parameter string array .
        def get_str_array(self):
            return self.__parray_str  

        # Get parameter string array .
        def get_int_array(self):
            return self.__parray_int
    # type = 6
    class TMDRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Header type Num.
            self.type_num = 6
            # public method
            self.__cmd_id = 0
            self.__addr   = [0,0] 
            self.__value  = [0,0] 
        
        # Set TMD struct data.
        def set_TMD_cmd(self,cmd_id:int,addr:int,value:int):
            self.__cmd_id = cmd_id
            self.__addr[0]= addr & 0x00ff           # Addr LSB 
            self.__addr[1]= (addr >>8) & 0x00ff     # Addr MSB
            self.__value[0]= value & 0x00ff         # Value LSB
            self.__value[1]= (value>>8) & 0x00ff    # Value MSB

        # encode header array.
        def encode_harray(self):
            self.header.htype = self.type_num 
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.donot_care
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array

        # encode payload array.
        def encode_parray(self):
            self.payload.array[0]= self.__cmd_id
            self.payload.array[1]= self.__value[0]
            self.payload.array[2]= self.__value[1]
            self.payload.array[3]= self.__addr[0]
            self.payload.array[4]= self.__addr[1]
            return self.payload.array   
    # type = 7
    class TMDResponse(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Header type Num.
            self.type_num = 7 
            # public method
            self.__cmd_id = 0      # CMD 
            self.__addr   = 0      # 
            self.__value  = 0      # 

        # Header array.
        def decode_harray(self,harray):
            self.search_addr = harray[1]
        
        # Payload array.      
        def decode_parray(self,parray):
            _decode_ok =False
            try:
                self.__cmd_id = parray[0]
                self.__value  = parray[1]
                self.__value += parray[2]<<8
                self.__addr   = parray[3]
                self.__addr  += parray[4]<<8
                _decode_ok = True
            except Exception as e:
                print(e)
            return _decode_ok
        
        # Decode tlg array. 
        def decode_tlg_array(self,tlg):
            _decode_ok =False
            if (self.check_crc32_array(tlg)==True):
                self.decode_harray(tlg[0:8])
                self.decode_parray(tlg[8:44])
                _decode_ok =True
            else:
                print("Decode tlg state Error!")
            return _decode_ok 
        
        # Get TMD cmd.
        def get_TMD_cmd(self):return self.__cmd_id  

        # Get TMD value.
        def get_TMD_value(self):return self.__value

    # type = 10
    class DataLogRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Type Num.
            self.type_num = 10
            # public property.
            self.search_addr = 0

        # encode header array.
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.array_address = self.search_addr
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.array_address
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array

        # encode payload array.
        def encode_parray(self):
            for idx in range(36):self.payload.array[idx]=0
            return self.payload.array        
    
    # type = 11
    class DataLogResponse(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            self.type_num = 11
            self.search_addr = 0
            self.datalog_list = []
        
        # Header array.
        def decode_harray(self,harray):self.search_addr = harray[1]
        
        # Payload array.      
        def decode_parray(self,parray):
            for idx in range(16):
                _data = parray[idx]+parray[idx+1]<<8
                self.datalog_list.append(_data)
            _decode_ok =False
        
        def decode_tlg_array(self,tlg):
            _decode_ok =False
            if (self.check_crc32_array(tlg)==True):
                self.decode_harray(tlg[0:8])
                self.decode_parray(tlg[8:44])
                _decode_ok =True
            else:
                print("Decode tlg state Error!")
            return _decode_ok
    
    # type = 12
    class ProcessDataRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Type nmber.
            self.type_num = 12
            # Telgram parameter       
            self.P_gain = 0              # SID = 2         
            self.I_gain = 0              # SID = 3
            self.max_time = 0            # SID = 4
            self.min_time = 0            # SID = 5
            self.REV_function = 0        # SID = 6
            self.steps_num = 0           # SID = 7
    
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.donot_care
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
    
        def encode_parray(self):
            _step_para_array=[
                self.P_gain, 
                self.I_gain,
                self.max_time,
                self.min_time,
                self.REV_function,
                self.steps_num,
            ]            

            # Clear payload array.
            _data_len = len(self.payload.array)
            for idx in range(_data_len):self.payload.array[idx]=0 

            # Access value to payload array. 
            for idx,val in enumerate(_step_para_array):
                self.payload.array[idx*2]=val%256
                self.payload.array[idx*2+1]=val//256           
            return self.payload.array        
    
    # type = 13
    class ProcessDataResponse(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Type number.
            self.type_num = 13
            # Parameter
            self.P_gain = 0              # SID = 2         
            self.I_gain = 0              # SID = 3
            self.max_time = 0            # SID = 4
            self.min_time = 0            # SID = 5
            self.REV_function = 0        # SID = 6
            self.steps_num = 0           # SID = 7
        
            # Step List.
            self.step_list = []         # SID = 1000
    
    # type =14 
    class StepDataRequest(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Type number.
            self.type_num = 14
            # wirte step adress
            self.step_addr = 0
            # Parameter
            self.step_rpm    = 0 
            self.step_slope  = 0
            self.max_current = 0
            self.max_torque  = 0
            self.max_revolutions = 0
            self.execution_time = 0
            self.angle = 0
            self.step_angle_window = 0
            self.step_torque_window = 0
            self.min_duty_cycle = 0
            self.step_flags = 0
        
        # public method.
        def encode_harray(self):
            self.header.htype = self.type_num
            self.header.step_address = self.step_addr
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.step_address
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
    
        def encode_parray(self):
            _step_parray=[
                self.step_rpm, 
                self.step_slope,
                self.max_current,
                self.max_torque,
                self.max_revolutions,
                self.execution_time,
                self.angle,
                self.step_angle_window,
                self.step_torque_window,
                self.min_duty_cycle,
                self.step_flags,
            ]            
            # Clear payload array.
            _data_len = len(self.payload.array)
            for idx in range(_data_len):self.payload.array[idx]=0 
            # Access value to payload array. 
            for idx,val in enumerate(_step_parray):
                self.payload.array[idx*2]=val%256
                self.payload.array[idx*2+1]=val//256           
            return self.payload.array        
    
    # type =15
    class StepDataResqposne(TlgStructureBasic):
        # Initial object.
        def __init__(self):
            # Type number.
            self.type_num = 15
            # wirte step adress
            self.step_addr = 0
            # Parameter
            self.step_rpm  = 0 
            self.step_slope = 0
            self.max_current = 0
            self.max_torque = 0
            self.max_revolutions = 0
            self.execution_time = 0
            self.angle = 0
            self.step_angle_window = 0
            self.step_torque_window = 0
            self.min_duty_cycle = 0
            self.step_flags = 0
            
        def decode_harray(self):
            self.header.step_address = self.step_addr
            self.header.htype = self.type_num
            self.header.array[0] = self.header.htype
            self.header.array[1] = self.header.step_address
            self.header.array[2] = self.header.donot_care
            self.header.array[3] = self.header.donot_care
            self.header.array[4] = self.header.donot_care
            self.header.array[5] = self.header.donot_care
            self.header.array[6] = self.header.donot_care
            self.header.array[7] = self.header.donot_care
            return self.header.array
    
        def decode_parray(self):
            return self.payload.array  
    #endregion

    # pulibc property   
    ctrl_request = CtrlTlg()                   # type = 1
    status_response = StatusTlg()              # type = 2
    Para_wirte_request = ParaWirteRequest()    # type = 3
    Para_read_request = ParaReadRequest()      # type = 4
    Para_response = ParaResponse()             # type = 5
    TMD_request = TMDRequest()                 # type = 6
    TMD_response= TMDResponse()                # type = 7
    datalog_request = DataLogRequest()         # type = 10
    datalog_response = DataLogResponse()       # type = 11
    process_request = ProcessDataRequest()     # type = 12
    process_response = ProcessDataResponse()   # type = 13
    step_request = StepDataRequest()           # type = 14
    step_resposne = StepDataResqposne()        # type = 15 

    first_run = True
    loosen_status = False

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object value.
    def __init__(self): pass

    #public method
    # encode tlg.
    def encode_request_tlg(self,tlgtype):
        _htpye = tlgtype
        _send_array =[]

        if (_htpye==self.ctrl_request.type_num):                     # header num = 1
            _send_array=self.ctrl_request.encode_tlg_array()         
        elif(_htpye==self.Para_wirte_request.type_num):              # header num = 3
            _send_array = self.Para_wirte_request.encode_tlg_array()
        elif(_htpye==self.Para_read_request.type_num):               # header num = 4
            _send_array = self.Para_read_request.encode_tlg_array()
        elif(_htpye==self.TMD_request.type_num):                     # header num = 6
            _send_array = self.TMD_request.encode_tlg_array()
        elif(_htpye==self.datalog_request.type_num):                 # header num = 10
            _send_array = self.datalog_request.encode_tlg_array()
        elif(_htpye==self.process_request.type_num):                 # header num = 12
            _send_array = self.process_request.encode_tlg_array()
        elif(_htpye==self.step_request.type_num):                    # header num = 14
            _send_array = self.step_request.encode_tlg_array()
        return _send_array

    # decode tlg.
    def decode_response_tlg(self,tlg):
        _check_ok=False
        _htpye = tlg[0]        

        if(_htpye==self.status_response.type_num):                  # header num = 2
            _check_ok = self.status_response.decode_tlg_array(tlg)            
        elif(_htpye==self.Para_response.type_num):                  # header num = 5
            _check_ok = self.Para_response.decode_tlg_array(tlg)
        elif(_htpye==self.TMD_response.type_num):                   # header num = 7
            _check_ok = self.TMD_response.decode_tlg_array(tlg)
        elif(_htpye==self.datalog_response.type_num):               # header num = 11
            _check_ok = self.datalog_response.decode_tlg_array(tlg)
        elif(_htpye==self.process_response.type_num):               # header num = 13
            _check_ok = self.process_response.decode_tlg_array(tlg)
        elif(_htpye==self.step_resposne.type_num):                  # header num = 15
             _check_ok = self.step_resposne.decode_tlg_array(tlg)
        return _check_ok
    
#endregion
