# Finite State Machine
# -*- coding: utf-8 -*-
from abc import ABCMeta,abstractmethod
from module import database as db
from module import common as comm
from module import logging as log
import math 
from enum import Enum
from platform import system
import os

# SC direction.
class Dircetion(Enum):
    FASTEN = "fasten"
    LOOSEN = "loosen"
    SL     = "SL" 

# Gear box type
class GearBoxRatio(Enum):
    TCS_NT_3 = 11.11
    TCS_NT_5 = 16.00
    TCS_NT_7 = 27.04

class CtrlTlgEnum(Enum):
    CTRL_FLAGS  = "u16Ctrlflags"               # u16Ctrlflags,42496,8192,9216,Flag10=Cyc
    CTRL_MOD    = "u16ControlMode"             # u16ControlMode
    WORK_PROC   = "u16WorkProc"                # u16WorkProc = 4248
    CTRL_PROG   = "u16CtrlProgram"             # u16CtrlProgram
    MAN_RPM     = "u16ManRpm"                  # u16ManRpm
    MAN_SLOP    = "u16ManSlope"                # u16ManSlope
    MAX_TORQUE  = "u16ManMaxTorque"            # u16ManMaxTorque
    MAX_CURRENT = "u16ManMaxCurrent"           # u16ManMaxCurrent
    MAN_RPM_MOD = "u16ManRpmMode"              # u16ManRpmMode
    TMD_CTRL    = "u8TMDControl"               # u8TMDControl

class StatusTlgEnum(Enum):
    STATUS_FLAGS   = "u16Statusflags"
    ACT_ERR        = "u16ActError"
    ACT_PROCESS    = "u16ActProcNr"
    ACT_STEP       = "u16ActStepNr"
    ACT_CURRENT    = "u16ActCurr"
    ACT_TORQUE     = "u16ActTorque"
    ACT_RPM        = "u16ActRPM"
    MAX_CURRENT    = "u16MaxCurrent"
    MAX_TORQUE     = "u16MaxTorque"
    ACT_ANGLE      = "u32Angle"
    ACT_REVOLUTION = "u32Revolutions"
    TMD_FLAGS      = "u16TMDFlags"
    TMD_TMP        = "s16Debug"
    U32_DEBUG      = "s32Debug"
    
# Hearder Type
class HearderType(Enum):
    CTRL_TLG         = 1                # 
    STATUS_TLG       = 2                # 
    WRITE_REQUEST    = 3                # 
    READ_REQUEST     = 4                # 
    RESPONSE_TLG     = 5                # 
    TMD_REQUEST      = 6                #
    TMD_RESPONSE     = 7                # 
    DL_REQUEST       = 10               #
    DL_RESPONSE      = 11               #
    PR_REQUEST       = 12               # 
    PR_RESPONSE      = 13               #
    SR_REQUEST       = 14               #
    SR_RESPONSE      = 15               #

# Contol Flag.
class CtrlFlag(Enum):
    SI_TURN_BACK     = 0                # Flag 00
    SI_WAIT_START    = 1                # Flag 01 
    SI_ANGLE_POS     = 2                # Flag 02
    SI_ANGLE_MON     = 3                # Flag 03 

    LOCK_MOTOR       = 9                # Flag 09
    EN_TIMEOUT_200   = 10               # Flag 10 
    ERROR_ACK        = 11               # Flag 11
    RESET            = 12               # Flag 12
    SCREWDRIVER_EN   = 13               # Flag 13
    SCREWDRIVER_REV  = 14               # Flag 14
    SCREWDRIVER_STA  = 15               # Flag 15

# Status Flag.
class StatusFlagsIdx(Enum): 
    TORQUE_MCU_PRSESENT     = 7         # Flag 7
    REVERVED                = 8         # Flag 8
    TORQUE_MCU_CYCLIC_OFF   = 9         # Flag 9
    CYCLIC_TIMEOUT_ENABLE   = 10        # Flag 10
    ERROR_OCCURED           = 11        # Flag 11
    PROG_STATUS             = 12        # Flag 12 
    PROC_STATUS             = 13        # Flag 13
    TOOL_REVERSE            = 14        # Flag 14
    TOOL_START              = 15        # Flag 15

# TMD MCU Flag.
class TMDCtrlFlg(Enum):
    LED_1_ON                = 0                    # LED 1 
    LED_2_ON                = 1                    # LED 2
    LED_3_ON                = 2                    # LED 3
    LED_4_ON                = 3                    # LED 4 
    RESET_TORQUE_VALUE      = 3                    # Zero the torque value. 
    
# TMD MCU Flag.
class TMDStatusFlg(Enum):
    LERVER_SW               = 0         # Lever start switch.         
    PUSH_SW                 = 1         # Push start switch.
    REVERSE_SW              = 2         # Reverse start switch.
    TEMP_ALM                = 3         # Lever start switch.

# Step
class StepFlage(Enum):
    TIGHT_STEP                = 0      # Flage = 0  Tightening Step.
    USE_PROCESS_GAIN          = 1      # Flage = 1  Use process gain settings.
    STOP_STEP                 = 2      # Flage = 2  Stop motor at end of step.
    RESET_REVOULTION_STA      = 3      # Flage = 3  Reset Revolutions at Start.
    RESET_ANGLE_STA           = 4      # Flage = 4  Reset Angle at Start.
    STEP_REVERSE              = 5      # Flage = 5  Step Reverse*
    ERR_REVOLUTION_REACHED    = 6      # Flage = 6  Error if revolutions are reached.
    ERR_TORQUE_OUTSIDE        = 7      # Flage = 7  Error if torque is out of window.
    ERR_ANGLE_OUTSIDE         = 8      # Flage = 8  Error if angle is out of window.
    RESERVED                  = 9      # Flage = 9  reserved.
    SWITC_ANGLE_REACHED       = 10     # Flage = 10 Switch to next step when angle is reached.
    SWITC_REVOLUTION_REACHED  = 11     # Flage = 11 Switch to next step when revolutions are reached.
    SWITC_CURRENT_REACHED     = 12     # Flage = 12 Switch to next step when current is reached.
    SWITC_TORQUE_REACHED      = 13     # Flage = 13 Switch to next step when torque is reached.
    SWITC_RPM_REACHED         = 14     # Flage = 14 Switch to next step when rpm is reached.
    SWITC_TIME_REACHED        = 15     # Flage = 11 Switch to next step when revolutions are reached.
    
# Actual error.
class MCBErrIdx(Enum):    
    ERR_CONT_TEMP = 0                #flag_00,No error.
    ERR_MOT_TEMP = 1                 #flag_01,Error controller temperature.
    ERR_MOT_CURR = 2                 #flag_02,Error motor temperature.
    ERR_MOT_PEAK_CURR = 3            #flag_03,Error motor current too high.
    ERR_MOT_TORQUE = 4               #flag_04,Error motor peak current too high.
    ERR_DEADLOCK = 5                 #flag_05,Error motor torque too high.
    ERR_PROC_MINTIME = 6             #flag_06,Error motor doesn’t turn.
    ERR_PROC_MAXTIME = 7             #flag_07,Error process min time is below limit
    ERR_ENCODER = 8                  #flag_08,No pulses from encoder.
    ERR_HALL = 9                     #flag_09,No pulses from hall sensors.
    ERR_BUSVOLTAGE_HIGH = 10         #flag_10,Bus- Voltage is too high.
    ERR_BUSVOLTAGE_LOW = 11          #flag_11,Bus- Voltage is too low.
    ERR_PROG_NA = 12                 #flag_12,Program not available.
    ERR_PROC_NA = 13                 #flag_13,Process not available.
    ERR_STEP_NA = 14                 #flag_14,Step not available.
    ERR_DMS_COMM = 15                #flag_15,Error Torque Controller Communicatiom.
    ERR_FLASH = 16                   #flag_16,CRC Error STM32 Flash.
    ERR_FRAM = 17                    #flag_17,CRC Error Fram.
    ERR_ANGLE = 18                   #flag_18,Error Angle (only if selected in screwing step).
    ERR_REVOLUTIONS = 19             #flag_19,Error Revolutions (only if selected in screwing step)
    ERR_PROTECT_CIRCUIT =20          #flag_20
    ERR_SWITCH_CONFIG =21            #flag_21
    ERR_DISPLAY_COMM_TO=22           #flag_22

class ErrMsg(Enum):
    CONT_TEMP       = 'ERROR_CONT_TEMP',           #flag_00
    MOT_TEMP        = 'ERROR_MOT_TEMP',            #flag_01
    MOT_CURR        = 'ERROR_MOT_CURR',            #flag_02
    MOT_PEAK_CURR   = 'ERROR_MOT_PEAK_CURR',       #flag_03
    MOT_TORQUE      = 'ERROR_MOT_TORQUE',          #flag_04
    DEADLOCK        = 'ERROR_DEADLOCK',            #flag_05
    PROC_MINTIME    = 'ERROR_PROC_MINTIME',        #flag_06
    PROC_MAXTIME    = 'ERROR_PROC_MAXTIME',        #flag_07
    ENCODER         = 'ERROR_ENCODER',             #flag_08
    H_ALL           = 'ERROR_H_ALL',               #flag_09
    BUSVOLTAGE_HIGH = 'ERROR_BUSVOLTAGE_HIGH',     #flag_10
    BUSVOLTAGE_LOW  = 'ERROR_BUSVOLTAGE_LOW',      #flag_11
    PROG_NA         = 'ERROR_PROG_NA',             #flag_12
    PROC_NA         = 'ERROR_PROC_NA',             #flag_13
    STEP_NA         = 'ERROR_STEP_NA',             #flag_14
    DMS_COMM        = 'ERROR_DMS_COMM',            #flag_15
    FLASH           = 'RROR_FLASH',                #flag_16
    FRAM            = 'ERROR_FRAM',                #flag_17
    ANGLE           = 'ERROR_ANGLE',               #flag_18
    REVOLUTIONS     = 'ERROR_REVOLUTIONS',         #flag_19
    PROTECT_CIRCUIT = 'ERROR_PROTECT_CIRCUIT',     #flag_20
    SWITCH_CONFIG   = 'ERROR_SWITCH_CONFIG',       #flag_21
    DISPLAY_COMM_TO = 'ERROR_DISPLAY_COMM_TO',     #flag_22

# TMD
class TMD(Enum):
    # Device identification and administration.
    READ_SER_NO           = 0x01    # Read serial No.
    WRITE_SER_NO          = 0x02    # Write serial No.
    SET_DEVICE_ACCESS_PIN = 0x04    # Set Device Access PIN.
    READ_NOMINAL_RANGE    = 0x05    # Read Nominal Range.
    WRITE_NOMINAL_RANGE   = 0x06    # Write Nominal Range.
    READ_FIREWARE_VER     = 0x07    # Read Firmware version.
    READ_HARDWARE_VER     = 0x08    # Read hardware version.
    GET_LAST_DEV_ERR      = 0x0A    # Get Last Device Error.
    READ_SENSOR_CAHRA     = 0x0B    # Read Sensor Characteristic Value.
    WRITE_SENSOR_CHARA    = 0x0C    # write Sensor Characteristic Value.
    
    # Measuring value related
    READ_MEASURING_VALUE        = 0x10   # Read Measuring Value.
    READ_ALERT_SW_FLAGS         = 0x11   # Read Alert and Switch flags.
    READ_RAW_VALUE              = 0x12   # Read Raw measuring value.
    SET_ZERO                    = 0x13   # Read Raw measuring value.
    READ_ZERO_VALUE             = 0x14   # Read Zero value.
    WRITE_ZERO_VALUE            = 0x15   # Write Zero value.
    READ_TEMPERATURE            = 0x16   # Read Temperature.
    READ_lever_position         = 0x17   # Read lever switch position.
    READ_PUSH_START_POS         = 0x18   # Read push start switch position.
    READ_MEASURE_VALUE_FLAGS    = 0x19   # Read Measuring Value And Flags.
    READ_TORQUE_MAX_VALUE       = 0x1A   # Read Torque Maximum Value.
    CLEAR_TORQUE_MAX_VALUE      = 0x1B   # Clear torque maximum value.
    READ_TORQUE_IPPER_PEAK      = 0x1C   # Read Torque Upper Peak Value
    READ_TORQUE_LOWER_PEAK      = 0x1D   # Read Torque Lower Peak Value.
    
    # Alert an other thresholds.
    READ_TORQUE_ALERT_THRESHOLD  = 0x20   # Read Torque Alert threshold.
    WRITE_TORQUE_ALERT_THRESHOLD = 0x21   # Write Torque Alert threshold.
    READ_TORQUE_ALERT_TRANSIENT  = 0x22   # Read Torque Alert transient threshold.
    WRITE_TORQUE_ALERT_TRANSIENT = 0x23   # Read Torque Alert transient threshold.
    READ_TEMP_ALERT_THRESHOLD    = 0x24   # Read temperature alarm threshold.
    WRITE_TEMP_ALERT_THRESHOLD   = 0x25   # Read temperature alarm threshold.
    READ_ZERO_RAND_THRESHOLD     = 0x26   # Read zero range threshold.
    WRITE_ZERO_RANG_THRESHOLD    = 0x27   # Write zero range threshold.
    READ_START_LEVER_THRESHOLD   = 0x28   # Read start lever threshold.
    WRITE_START_LERVER_THRESHOLD = 0x29   # Write start lever threshold.
    READ_PUSH_START_THRESHOLD    = 0x2A   # Read push start threshold.
    WRITE_PUSH_START_THRESHOLD   = 0x2B   # Write push start threshold.

    # Calibration
    READ_SCALE_CALIBRATION_REF   = 0x38   # Read Scale Calibration Reference.
    WRITE_SCALE_CALIBRATION_REF  = 0x39   # Write Scale Calibration Reference.
    SET_SCALING                  = 0x3A   # Set Scaling.
    READ_CALIBRATION_OFFSET      = 0x3B   # Read Calibration Offset.
    WRITE_CALIBRATION_OFFSET     = 0x3C   # Write Calibration Offset.
    READ_SCALE_CALIBRATION       = 0x3D   # Read Factory Scale Calibration Value.
    WRITE_SCALE_CALIBRATION      = 0x3E   # Write Factory Scale Calibration Value.
    CALIBRATE_LEVER_SW           = 0x3F   # Calibrate lever start switch.
    CALIBRATE_PUSH_SW            = 0x40   # Calibrate push start switch.    
    READ_SCALE_RECALIBRATTION    = 0x41   # Read Scale Recalibration Value.
    WRITE_SCALE_RECALIBRATTION   = 0x42   # Write Scale Recalibration Value.
    READ_LAST_CALIBRATION_DATA   = 0x43   # Read Last Calibration Date.
    WRITE_LAST_CALIBRATION_DATA  = 0x44   # Write Last Calibration Date.
    ENTER_MEASURING_STATE        = 0x50   # Enter Measuring State .

    # Event counters, statistics and others
    READ_FW_START_CNT            = 0x58   # Read turning starts counter forward.
    WRITE_FW_START_CNT           = 0x59   # Write turning start counter forward.
    READ_BW_START_CNT            = 0x5A   # Read turning starts counter backward.
    WRITE_BW_START_CNT           = 0x5B   # Write turning starts counter backward.
    READ_REGULAR_STOP_CNT        = 0x5C   # Read regular turning stops counter.
    WRITE_REGULAR_STOP_CNT       = 0x5D   # Write regular turning stops counter.
    READ_TORQUE_EMG_STOP_CNT     = 0x5E   # Read torque emergency turning stops counter.
    WRITE_TORQUE_EMG_STOP_CNT    = 0x5F   # Write torque emergency turning stops counter.
    READ_OVER_TEMP_STOP_CNT      = 0x60   # Read over temperature turning stops counter.
    WRITE_OVER_TEMP_STOP_CNT     = 0x61   # Write over temperature turning stops counter.
    INCREMENT_START_STOP_CNT     = 0x62   # Increment turns start/stop counters.

    # LEDset CounterInc Combined
    LEDSET_CNT_COMBINED          = 0x63   # LEDset CounterInc Combined.
    READ_LED_ON_OFF_STATE        = 0x64   # Read LED On/Off state.
    WRITE_LED_ON_OFF_STATE       = 0x65   # Write LED On/Off state.
    READ_LED_BLINKING_STATE      = 0x66   # Read LED Blinking state.
    WRITE_LED_BLINKING_STATE     = 0x67   # Write LED Blinking state .
    READ_TONE_FREQUENCE          = 0x68   # Read Tone Frequency.
    WRITE_TONE_FREQUENCE         = 0x69   # Write Tone Frequency.
    READ_TONE_LENGTH             = 0x6A   # Read Tone Length.
    WRITE_TONE_LENGTH            = 0x6B   # Write Tone Length.
    PLAY_SOUND                   = 0x6E   # Play Sound.
    SET_MELODY_NOTE              = 0x6F   # Set Melody Note.
    PLAY_MELODY                  = 0x70   # Play Melody.
    READ_STORED_PARA             = 0x72   # Read Stored Parameter.
    WRITE_PARA_TO_STORE          = 0x73   # Write Parameter to store.
    
#endregion

class MSTATUS(Enum):
    # FSM
    # RUNNING     = "Running"
    SETTING     = "Setting"
    # IDLE        = "Idle"
    ALARM       = "alarm"
    # INITIAL     = "InitialSYS"
    CHECK       = "CheckInit"
    START       = "StartSYS"
    # OTHER
    READY       = "Ready"
    STOP        = "Stop"
    NONE        = None

# Define all 
class PROCESS(Enum):
    M_FSM       = "M_FSM" 
    M_Ready     = "M_Ready"
    MCB       = "MCB_cmd"       # MCB cmd   
    UI_C        = "UI_cmd"        # Command form UI to Driver. 
    UI_S        = "UI_status"     # Status form Driver to UI.
    LAN_C       = "LAN_cmd"
    LAN_S       = "LAN_status"
    RS232_C     = "RS232_cmd"
    RS232_S     = "RS232_status"
    RS485_C     = "RS485_cmd"
    RS485_S     = "RS485_cmd"
    GPIO        = "GPIO_cmd"
    DB          = "DB_cmd"
    SD          = "SD_cmd"
    VIEW        = "VIEW_cmd"
     
# Define all 
class IomItem(Enum):
    cmd_id      = 0
    update_time = 1
    data        = 2

class TLGC(Enum):
    CTRL_NON = "CtrlNon"
    CTRL_ADV = "CtrlAdv"
    CTRL_SL  = "CtrlSL" 
    STATUS   = "status"
    W_PARA   = "WritePara"
    R_PARA   = "ReadPara"
    PARA_E   = "ParaEcho"

# Define all protocol cmd.
class AMSCmd(Enum):
    # CMD
    CMD_ACK            = "CMD300"   # 
    CMD_CTRL           = "CMD302"   # CMD302, 工具控制命令
    CMD_SET_NORMAL_TRQ = "CMD310"   # CMD310, 設定Normal Torque
    CMD_W_BASIC        = "CMD340"   # CMD340, update basice data.

    # ANS
    ANS_CTRL           = "ANS302"   # ANS302, 回復工具控制命令
    ANS_W_BASIC_PARA   = "ANS340"   # ANS340, 回復Initial

    # DATA
    DATA_RT_STATUS     = "DATA300"  # 
    DATA_SL_STATUS     = "DATA302"
    # REQ
    REQ_IDLE           = "REQ300"   # 
    REQ_I_BASIC_PARA   = "REQ301"   # 
    REQ_SL             = "REQ302"   # Self leaning. 

    # # PRIVATE
    CMD_R_MCB_BASIC    = "CMD341"   # Read MCB data.
    CMD_W_DB_BASIC     = "CMD342"   # Write DB basice data.
    CMD_R_DB_BASIC     = "CMD343"   # Read DB nasice data.


#region ams idx.
# REQ302
class REQ302(Enum):
    HEADER       = "str1"   # Header+DATA ,{DATA300
    TIME         = "str2"   # yyyyMMdd HH:mm:ss
    CHECH_SUM    = "str3"   # Checksum
    CMD_SN       = "str4"   # Command_sn        
    TRG_TRQ      = "str5"   # Target Torque
    MAX_CURR     = "str6"   # Max Current.
    ANGLE_SL     = "str7"   # Angle Selflearning        
    SL_ANGLE_MON = "str8"   # SI Angle Monitoring.
    SL_ANGLE_POS = "str9"   # SI Angle Positioning.
    SL_WART_STR  = "str10"  # Sl Wait Start.
    SL_TURN_BACK = "str11"  # SI Turn Back.

# DATA300.
class DATA300(Enum):
    HEADER    = "str1"   # Header+DATA ,{DATA300
    TIME      = "str2"   # yyyyMMdd HH:mm:ss
    CHECH_SUM = "str3"   # Checksum
    CMD_SN    = "str4"   # Command_sn        
    DEV_TYPE  = "str5"   # Device type
    TOOL_SN   = "str6"   # Tool SN
    DEV_SN    = "str7"   # Device SN    
    JOB_ID    = "str8"   # Job ID
    SEQ_ID    = "str9"   # Sequence ID
    PRG_ID    = "str10"  # Program ID
    STEP_ID   = "str11"  # Step ID
    DIRECTION = "str12"  # Direction
    UNIT      = "str13"  # Torque unit
    INC_DEC   = "str14"  # INC/DEC
    LAST_CNT  = "str15"  # Last_screw_count
    MAX_CNT   = "str16"  # Max_screw_count
    FASTEN_TR = "str17"  # Fastening time
    TORQUE    = "str18"  # Torque
    ANGLE     = "str19"  # Angle
    MAX_TRQ   = "str20"  # Max Torque
    REVOLUTION= "str21"  # Revolutions
    STATUS    = "str22"  # Status
    GPIO_IN   = "str23"  # Inputio
    GPIO_OUT  = "str24"  # Outputio
    ERR_MSG   = "str25"  # Error Masseage
    TOOL_CNT  = "str26"  # Tool Count
    TOOL_RPM  = "str27"  # RPM

# DATA300 status enum.
class DATA300Status(Enum):
    OK          = "OK"
    NG          = "NG"
    RUN         = "Running"
    STOP        = "Stop"
    REV         = "Reverse"
    # IDLE        = "Idle"
    OK_SEQ      = "OK Sequence"
    OK_JOB      = "OK JOB"

# DATA302
class DATA302(Enum):
    HEADER       = "str1"   # Header+DATA ,{DATA300
    TIME         = "str2"   # yyyyMMdd HH:mm:ss
    CHECH_SUM    = "str3"   # Checksum
    CMD_SN       = "str4"   # Command_sn        
    STATUS       = "str5"   # status

# DATA302 status enum.
class DATA302Status(Enum):
    SL_FINISHED        = 11    # :Selflearning finished successful.
    SL_R_STA           = 20    # Selflearning Release Start.
    SL_ENGLE_STA       = 21    # Selflearning Engage Start.
    SL_TO_ERR          = 1000  # Selflearning Error Timeout.
    SL_TO_THREAD_STA   = 1001  # Selflearning Error Timeout Thread Start.
    SL_TABLE_ERR       = 1002  # Selflearning Table Erro.
    SL_A_P_MON_ERR     = 1003  # Angle Monitoring and Positioning not Possible.
    SL_NO_WAIT_TB_RR   = 1004  # No Wait Start and no Turn Back not Possible.
#endregion

# Event table struct.
class GtcsData():
    # 
    __MCB_errmsg_list = {
        MCBErrIdx.ERR_CONT_TEMP.value     : "ERR_CONT_TEMP",    # flag_00,No error.
        MCBErrIdx.ERR_MOT_TEMP.value      : "ERR_MOT_TEMP",     # flag_01,Error controller temperature.
        MCBErrIdx.ERR_MOT_CURR.value      : "ERR_MOT_CURR",     # flag_02,Error motor temperature.
        MCBErrIdx.ERR_MOT_PEAK_CURR.value : "ERR_MOT_PEAK_CURR",# flag_03,Error motor current too high.
        MCBErrIdx.ERR_MOT_TORQUE.value    : "ERR_MOT_TORQUE",   # flag_04,Error motor peak current too high.
        MCBErrIdx.ERR_DEADLOCK.value      : "ERR_DEADLOCK",     # flag_05,Error motor torque too high.
        MCBErrIdx.ERR_PROC_MINTIME.value  : "ERR_PROC_MINTIME", # flag_06,Error motor doesn’t turn.
        MCBErrIdx.ERR_PROC_MAXTIME.value  : "ERR_PROC_MAXTIME", # flag_07,Error process min time is below limit
        MCBErrIdx.ERR_ENCODER.value       : "ERR_ENCODER",      # flag_08,No pulses from encoder.
        MCBErrIdx.ERR_HALL.value          : "ERR_HALL",         # flag_09,No pulses from hall sensors.
        MCBErrIdx.ERR_BUSVOLTAGE_HIGH.value : "ERR_BUSVOLTAGE_HIGH",# flag_10,Bus- Voltage is too high.
        MCBErrIdx.ERR_BUSVOLTAGE_LOW.value  : "ERR_BUSVOLTAGE_LOW", # flag_11,Bus- Voltage is too low.
        MCBErrIdx.ERR_PROG_NA.value         : "ERR_PROG_NA",        # flag_12,Program not available.
        MCBErrIdx.ERR_PROC_NA.value         : "ERR_PROC_NA",        # flag_13,Process not available.
        MCBErrIdx.ERR_STEP_NA.value         : "ERR_STEP_NA",        # flag_14,Step not available.
        MCBErrIdx.ERR_DMS_COMM.value        : "ERR_DMS_COMM",       # flag_15,Error Torque Controller Communicatiom.
        MCBErrIdx.ERR_FLASH.value           : "ERR_FLASH",          # flag_16,CRC Error STM32 Flash.
        MCBErrIdx.ERR_FRAM.value            : "ERR_FRAM",           # flag_17,CRC Error Fram.
        MCBErrIdx.ERR_ANGLE.value           : "ERR_ANGLE",          # flag_18,Error Angle (only if selected in screwing step).
        MCBErrIdx.ERR_REVOLUTIONS.value     : "ERR_REVOLUTIONS",    # flag_19,Error Revolutions (only if selected in screwing step)
        MCBErrIdx.ERR_PROTECT_CIRCUIT.value : "ERR_PROTECT_CIRCUIT",# flag_20
        MCBErrIdx.ERR_SWITCH_CONFIG.value   : "ERR_SWITCH_CONFIG",  # flag_21
        MCBErrIdx.ERR_DISPLAY_COMM_TO.value : "ERR_DISPLAY_COMM_TO",# flag_22
    }
    # 
    __CMD_340 = {
        "mintemp"             : 0.0,      # REAL
        "maxtemp"             : 0.0,      # REAL
        "maxcurrent"          : 0,        # INTEGER
        "maxpeakcurrent"      : 0,        # INTEGER
        "torquesensortype"    : 0,        # INTEGER
        "maxdutycycle"        : 0.0,      # REAL
        "maxtorque"           : 0.0,      # REAL
        "pwmfreq"             : 0,        # INTEGER
        "maxrpm"              : 0,        # INTEGER 
		"maxslope"            : 0,        # INTEGER 
		"minbusvolt"          : 0.0,      # REAL 
		"maxbusvolt"          : 0.0,      # REAL 
		"startdutycycle"      : 0.0,      # REAL 
		"gearboxratio"        : 0.0,      # REAL 
		"startinp"            : 0,        # INTEGER 
		"revinp"              : 0,        # INTEGER 
		"revrpm"              : 0,        # INTEGER 
		"revslope"            : 0,        # INTEGER
		"revmaxcurrent"       : 0,        # INTEGER 
		"revmaxtorque"        : 0.0,      # REAL 
		"erroridletime"       : 0,        # INTEGER 
		"backlash"            : 0,        # INTEGER 
		"pgain"               : 0,        # INTEGER 
		"igain"               : 0,        # INTEGER 
		"encoder"             : 0,        # INTEGER
    }

    # Rowdata
    __ROWDATA_340 = {
        "mintemp"             : 0,        # INTEGER
        "maxtemp"             : 0,        # INTEGER
        "maxcurrent"          : 0,        # INTEGER
        "maxpeakcurrent"      : 0,        # INTEGER
        "torquesensortype"    : 0,        # INTEGER
        "maxdutycycle"        : 0,        # INTEGER
        "maxtorque"           : 0,        # INTEGER
        "pwmfreq"             : 0,        # INTEGER
        "maxrpm"              : 0,        # INTEGER 
		"maxslope"            : 0,        # INTEGER 
		"minbusvolt"          : 0,        # INTEGER 
		"maxbusvolt"          : 0,        # INTEGER 
		"startdutycycle"      : 0,        # INTEGER 
		"gearboxratio"        : 0,        # INTEGER 
		"startinp"            : 0,        # INTEGER 
		"revinp"              : 0,        # INTEGER 
		"revrpm"              : 0,        # INTEGER 
		"revslope"            : 0,        # INTEGER
		"revmaxcurrent"       : 0,        # INTEGER 
		"revmaxtorque"        : 0,        # INTEGER 
		"erroridletime"       : 0,        # INTEGER 
		"backlash"            : 0,        # INTEGER 
		"pgain"               : 0,        # INTEGER 
		"igain"               : 0,        # INTEGER 
		"encoder"             : 0,        # INTEGER
    }

    __ams_data = {
        "CMD340"    :__CMD_340,
        "ROWDATA340":__ROWDATA_340,
    }

    __log = log.log()

    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass
    
    # Get 
    def get_data_keys(self)->list:
        return self.__CMD_340

    # Calculate basic rawdata
    def calculate_basic_rawdata(self,pKey:int,pValue):
        sub_id = pKey
        data = sub_id #// 10
        {
            1 : lambda :self.__CMD_340.update({"mintemp":pValue/10}),        # REAL
            2 : lambda :self.__CMD_340.update({"maxtemp":pValue/10}),        # REAL
            3 : lambda :self.__CMD_340.update({"maxcurrent":pValue}),        # INTEGER
            4 : lambda :self.__CMD_340.update({"maxpeakcurrent":pValue}),    # INTEGER
            5 : lambda :self.__CMD_340.update({"torquesensortype":pValue}),  # INTEGER
            6 : lambda :self.__CMD_340.update({"maxdutycycle":pValue/10}),   # REAL
            7 : lambda :self.__CMD_340.update({"maxtorque":round((pValue/1862)*5,2)}),  # REAL 
            8 : lambda :self.__CMD_340.update({"pwmfreq":pValue}),           # INTEGER
            9 : lambda :self.__CMD_340.update({"maxrpm":pValue}),            # INTEGER 
		    10 : lambda :self.__CMD_340.update({"maxslope":pValue}),         # INTEGER 
		    11 : lambda :self.__CMD_340.update({"minbusvolt":pValue/10}),    # REAL 
		    12 : lambda :self.__CMD_340.update({"maxbusvolt":pValue/10}),    # REAL 
		    13 : lambda :self.__CMD_340.update({"startdutycycle":pValue/10}),# REAL 
		    14 : lambda :self.__CMD_340.update({"gearboxratio":pValue/100}), # REAL 
		    15 : lambda :self.__CMD_340.update({"startinp":pValue}),         # INTEGER 
		    16 : lambda :self.__CMD_340.update({"revinp":pValue}),           # INTEGER 
		    17 : lambda :self.__CMD_340.update({"revrpm":pValue}),           # INTEGER 
		    18 : lambda :self.__CMD_340.update({"revslope":pValue}),         # INTEGER
		    19 : lambda :self.__CMD_340.update({"revmaxcurrent":pValue}),    # INTEGER 
		    20 : lambda :self.__CMD_340.update({"revmaxtorque":round((pValue/1862)*5,2)}),# REAL 
		    21 : lambda :self.__CMD_340.update({"erroridletime":pValue}),    # INTEGER 
		    22 : lambda :self.__CMD_340.update({"backlash":pValue}),         # INTEGER 
		    23 : lambda :self.__CMD_340.update({"pgain":pValue}),            # INTEGER 
		    24 : lambda :self.__CMD_340.update({"igain":pValue}),            # INTEGER 
		    25 : lambda :self.__CMD_340.update({"encoder":pValue}),          # INTEGER
        }.get(data, lambda: self.__log.warning("calculate basic rawdata error!"))()       
        # print(data)
    
    # Calculate basic rawdata
    def convert_to_basic_rowdata(self,pKey:int,pValue):
        sub_id = pKey
        data = sub_id #// 10
        {
            1 : lambda :self.__ROWDATA_340.update({"mintemp":int(float(pValue)*10)}), # INTEGER
            2 : lambda :self.__ROWDATA_340.update({"maxtemp":int(float(pValue)*10)}),        # INTEGER
            3 : lambda :self.__ROWDATA_340.update({"maxcurrent":int(pValue)}),        # INTEGER
            4 : lambda :self.__ROWDATA_340.update({"maxpeakcurrent":int(pValue)}),    # INTEGER
            5 : lambda :self.__ROWDATA_340.update({"torquesensortype":int(pValue)}),  # INTEGER
            6 : lambda :self.__ROWDATA_340.update({"maxdutycycle":int(float(pValue)*10)}),   # INTEGER
            7 : lambda :self.__ROWDATA_340.update({"maxtorque":int(int(float(pValue))/5*1862)}),# INTEGER 
            8 : lambda :self.__ROWDATA_340.update({"pwmfreq":int(pValue)}),           # INTEGER
            9 : lambda :self.__ROWDATA_340.update({"maxrpm":int(pValue)}),            # INTEGER 
		    10 : lambda :self.__ROWDATA_340.update({"maxslope":int(pValue)}),         # INTEGER 
		    11 : lambda :self.__ROWDATA_340.update({"minbusvolt":int(float(pValue)*10)}),    # INTEGER 
		    12 : lambda :self.__ROWDATA_340.update({"maxbusvolt":int(float(pValue)*10)}),    # INTEGER 
		    13 : lambda :self.__ROWDATA_340.update({"startdutycycle":int(float(pValue)*10)}),# INTEGER 
		    14 : lambda :self.__ROWDATA_340.update({"gearboxratio":int(float(pValue)*100)}), # INTEGER 
		    15 : lambda :self.__ROWDATA_340.update({"startinp":int(pValue)}),         # INTEGER 
		    16 : lambda :self.__ROWDATA_340.update({"revinp":int(pValue)}),           # INTEGER 
		    17 : lambda :self.__ROWDATA_340.update({"revrpm":int(pValue)}),           # INTEGER 
		    18 : lambda :self.__ROWDATA_340.update({"revslope":int(pValue)}),         # INTEGER
		    19 : lambda :self.__ROWDATA_340.update({"revmaxcurrent":int(pValue)}),    # INTEGER 
		    20 : lambda :self.__ROWDATA_340.update({"revmaxtorque":int(float(pValue)/5*1862)}),# INTEGER 
		    21 : lambda :self.__ROWDATA_340.update({"erroridletime":int(pValue)}),    # INTEGER 
		    22 : lambda :self.__ROWDATA_340.update({"backlash":int(pValue)}),         # INTEGER 
		    23 : lambda :self.__ROWDATA_340.update({"pgain":int(pValue)}),            # INTEGER 
		    24 : lambda :self.__ROWDATA_340.update({"igain":int(pValue)}),            # INTEGER 
		    25 : lambda :self.__ROWDATA_340.update({"encoder":int(pValue)}),          # INTEGER
        }.get(data, lambda: self.__log.error("convert to basic rowdata error!"))()       
        # print(data)
    
    # Get data object.
    def get_obj_data(self,key:str)->object:
        return self.__ams_data.get(key)

    # Set basic data.
    def set_basic_data(self,pdict:dict):
        self.__CMD_340.update(pdict)
    
    # Calculate MCB basic rowdata to DB data fromate.
    def calaulate_MCB_basic_rawdata(self,pdict:dict)->dict:
        for idx,val in enumerate(pdict): 
            try:
                self.calculate_basic_rawdata(pKey = idx+1,pValue=pdict.get(val))
            except Exception as e :
                self.__log.error(e)
        _obj = self.get_obj_data(key="CMD340")
        return _obj

    # Convert DB data fromate ro MCB basic rowdata.
    def convert_MCB_basic_rawdata(self,plist:list)->dict:
        for idx,val in enumerate(plist): 
            try:
                self.convert_to_basic_rowdata(pKey = idx+1,pValue = val)
            except Exception as e :
                self.__log.error(e)
        _obj = self.get_obj_data(key="ROWDATA340")  
        return _obj
    
    # Calculate angle.
    def convert_to_angle(self,rawdata:int,gear:int,direction:int)->float:
        _angle = 0
        _comm = comm.CommonFunction()  
        _bit_list = _comm.convert_to_bit_list(length=32,data=rawdata)  
        # if direction == 0:  # CW
        #     _angle = float(rawdata)/(gear*200)*360
        # else:               # CCW
        #     _angle = float(0xffffffff-rawdata)/(gear*200)*360
        _angle = float(rawdata)/(gear*200)*360
        return _angle

    # Calculate revolutions.
    def convert_to_revolutions(self,rawdata:int,gear:int,direction:int)->float:
        _rev = 0
        _comm = comm.CommonFunction()
        if gear ==0:
            pass
        else:
            if direction==0: # CW
                _rev = float(rawdata)/(200*gear)
            else:            # CCW
                _temp = 0xffffffff-rawdata
                _rev = float(_temp)/(200*gear)
        return _rev
    
    # Get MCB error message.
    def get_MCB_err_msg(self,idx:int)->str:
        _str = self.__MCB_errmsg_list.get(idx)
        return _str

# region IOM
# IOM 
class IOM(metaclass=ABCMeta):
    def __init__(self):pass

    @abstractmethod
    def creat_iom(self,table):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    @abstractmethod
    def read_iom(self):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    @abstractmethod
    def update_iom(self, x, y):
        'Make animal walk to position (x, y).'
        return NotImplemented

# CMD IOM
class CmdIOM(IOM):  
    __db = db.SqliteManager()
    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass     

    def creat_iom(self,table):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    def read_iom(self):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    def update_iom(self, x, y):
        'Make animal walk to position (x, y).'
        return NotImplemented

# ANS IOM
class AnsIOM(IOM):
    # Initial signleTon object.
    _instance = None 
    def __new__(cls, *args, **kwargs): 
        if cls._instance is None: 
            cls._instance = super().__new__(cls) 
        return cls._instance  
    # Initial object.
    def __init__(self): pass 

    def creat_iom(self,table):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    def read_iom(self):
        'Return when animal screaming the sound hear likes'
        return NotImplemented
    
    def update_iom(self, x, y):
        'Make animal walk to position (x, y).'
        return NotImplemented
#endregion
    