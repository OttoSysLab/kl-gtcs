#pragma once
#define _DEBUG_MODE_
// #define _DEBUG_RAM_MODE_
// #define _DEBUG_MODE_38_
// #define _DEBUG_MODE_54_
// #define _DEBUG_MODE_202_
// #define _DEBUG_MODE_207_
// #define _DEBUG_JOB_SEQ_
/*=======================================================================================
 Program Nane  	: gtcs_tlg_decoder.c
 Subject 		: SARM Serial Port Communication Driver Process
---------------------------------------------------------------------------------------
 Compiler/IDE  	: gcc 4.6.3
 Library       	:
 Commands      	: Geany Build Command
  				: Compile : gcc -Wall -I"../inc" -c "%f"
  				: Buid 	  : gcc -Wall -o "%e" "%f" "sarm2pcb.c" "pcb2sarm.c" "../lib/sarmdb.c" "../lib/sarmcomm.c" "../lib/sarmutility.c" -I"../inc" -l"sqlite3" -l"X11" `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 Programmer    	: Otto Chang
 Date	       	: 2019/08/06
=======================================================================================*/
#include "common.h"
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

#pragma region GTCS MCB Protcol
#pragma region Parameter
// Screw_target type
enum SCREW_TARGET_TYPE:int{
    TARGET_TORQUE       = 1,                //  
    TARGET_ANGLE        = 2,                //
    TARGET_REVALUTION   = 3,                //    
    TARGET_TIME         = 4,                //    
};
// GTCS batch mode
enum GTCS_BATCH_MODE:int{
    DEC = 0,                //  
    INC = 1,                //    
};
// GPIO Pin define.
enum GTCS_GPIO_REGEIST:int{
    GPIO2_DR = 0x020A0000,
    GPIO3_DR = 0x020A4000,
    GPIO5_DR = 0x020AC000,
    GPIO6_DR = 0x020B0000,
};
// GTCS_GPIO_IN
enum GTCS_GPIO_IN :int{
    IN_01 = 6,
    IN_02 = 4,
    IN_03 = 0,
    IN_04 = 27,
    IN_05 = 28,
    IN_06 = 29,
    IN_07 = 26,
    IN_08 = 30,
    IN_09 = 31,
    IN_10 = 4,
    IN_11 = 7,
    IN_12 = 16,
};
// GTCS_GPIO_IN
enum GTCS_GPIO_OUT :int{
    OUT_01 = 0,
    OUT_02 = 1,
    OUT_03 = 2,
    OUT_04 = 3,
    OUT_05 = 8,
    OUT_06 = 9,
    OUT_07 = 10,
    OUT_08 = 15,
    OUT_09 = 22,
    OUT_10 = 21,
    OUT_11 = 20,
    OUT_12 = 19,
    OUT_13 = 18,
    OUT_14 = 17,
};
// MCB_TELEGRAM_TYPE
enum MCB_TELEGRAM_TYPE : uint8_t{
    CTRL             = 1,
    STATUS           = 2,
    W_REQUEST        = 3,
    R_REQUEST        = 4,
    RW_RESPONSE      = 5,
    TMD_REQUEST      = 6,
    TMD_RESPONSE     = 7,
    
    LOG_REQUEST      = 10,
    LOG_RESPONSE     = 11,

    PROCESS_REQUST   = 12,
    PROCESS_RESPONSE = 13,
    STEP_REQUEST     = 14,
    STEP_RESPONSE    = 15,
};
// MAIN_FSM
enum MAIN_FSM : int{
    READY = 0,
    ALARM,
    SETTING,
    CHECK_SYS,
    INITIAL,
};
// TMD OUTPUT
enum TMD_OUTPUT:int {
    LED_1 = 0,
    LED_2,
    LED_3,
    LED_4,
};
// TMD INPUT
enum TMD_INPUT:int{
    LEVER_SW = 0,
    PUSH_SW,
    REV_SW,
    TEMP_ALM,
};
// MCB ctrl flags.
enum CTRL_FLAGS_IDX:int{
    SL_AUTO_TURNBLOCK = 0,
    SL_WAIT_START     = 1,
    SL_ANGLE          = 2,
    SL_ANGLE_MONITOR  = 3,
    TMD_OSF_300       = 7,
    NAM_NO_TRQERR     = 8,
    SHORT_UVW         = 9,
    EN_TIMEOUT_200MS  = 10,
    ERR_ACK           = 11,
    RESET             = 12,
    SC_ENABLE         = 13,
    SC_REVERSE        = 14,
    SC_START          = 15,
};
// Target type
enum TARGET_TYPE:int{
    TORQUE     = 1,
    ANGLE      = 2,
    REVOLUTION = 3,
    TIME       = 4,
};
// Configure Mcb StepFlags.
enum SCREW_STEP_FLAG : int{
    TIGHTENING_STEP      = 0,
    PROCESS_GAIN         = 1,
    STOP_MOTOR_END_STEP  = 2,
    RESET_REV_STA        = 3,
    RESET_ANGLE_STA      = 4,
    STEP_REVERSE         = 5,
    ERR_REVL_REACHED     = 6,
    ERR_TRQ_REACHED      = 7,
    ERR_ANGLE_REACJED    = 8,
    REVERSED             = 9,
    NEXT_ANGLE           = 10,
    NEXT_REVOLUTION      = 11,
    NEXT_CURRENT         = 12,
    NEXT_TORQUE          = 13,
    NEXT_RPM             = 14,
    NEXT_TIME            = 15, 
};
// Configure Mcb WindowModeFlags.
enum STEP_WINDOW_MODE_FLAG : int {
    LQ = 10,
    HQ = 11,
    WQ = 12,
    LA = 13,
    HA = 14,
    WA = 15,
};
//
enum LOCKED_STATUS : int {
    // IDLE
    IDLE = 0,

    // Running
    RUNNING,        // Fasten running
    REVERSE,        // loosen running.

    // STOP
    OK,
    OK_SEQUENCE,
    OK_JOB,

    // Confirm 不能解決
    NG_MCB,
    // Confirm 能解決
    NG_OK,
    NG_F,
    NS_F,
    NGQ,
    NGA,
    NGR,
    NGT,
};
// MCB error flag index
enum MCB_ERR_FLAG:int{
    ERR_CONT_TEMP = 0,
    ERR_MOT_TEMP,
    ERR_MOT_CURR,
    ERR_MOT_PEAK_CURR,
    ERR_MOT_TORQUE,
    ERR_DEADLOCK,
    ERR_PROC_MINTIME,
    ERR_PROC_MAXTIME,
    ERR_ENCODER,
    ERR_HALL,
    ERR_BUSVOLT_HIGH,
    ERR_BUSVOLT_LOW,
    ERR_PROG_NA,
    ERR_PROC_NA,
    ERR_STEP_NA,
    ERR_DMS_COMM,
    ERR_FLASH,
    ERR_FRAM,
    ERR_ANGLE,
    ERR_REVOLUTIONS,
    ERR_PROTECT_CIRCUIT,
    ERR_SWITCH_CONFIG,
    ERR_DISPLAY_COMM_TO,
    ERR_STEP_NOT_REC,
    ERR_TMD_FRAM,
};
// MCB status flags.
enum STATUS_FlAGS_IDX:int{
    TORQUE_MCU_PRSESENT     = 7,         // Flag 7
    REVERVED                = 8,         // Flag 8
    TORQUE_MCU_CYCLIC_OFF   = 9,         // Flag 9
    CYCLIC_TIMEOUT_ENABLE   = 10,        // Flag 10
    ERROR_OCCURED           = 11,        // Flag 11
    PROG_STATUS             = 12,        // Flag 12
    PROC_STATUS             = 13,        // Flag 13
    TOOL_REVERSE            = 14,        // Flag 14
    TOOL_START              = 15,        // Flag 15
};
// Identification Parameter Struct (MID1)
typedef struct
{
    std::string u8MotSWVer;   // SID = 1,Software Version Motorcontrolle.
    std::string u8MotCoreID ; // SID = 2,Unique Core ID from Motor- STM32 Conrtroller.
    std::string u8MotSerNr;   // SID = 3,Serial Number of the Motor Controller.
    std::string u8DmsSWVer;   // SID = 4,Software Version DMS- Controller.
    std::string u8DmsCorel;   // SID = 5,Unique Core ID from DMS- STM32.
    std::string u8DmsSerNr;   // SID = 6,Serial Number of the DMS- Controller.
}McbID1Struct;
// (MID2)
typedef struct
{
    uint16_t s16MinTemp;         // SID = 1,Minimal Temperature of the motor and the motorcontroller.
                                 // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
    uint16_t s16MaxTemp;         // SID = 2,Maximal Temperature of the motor and the motorcontroller.
                                 // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
    uint16_t u16MaxCurrent;      // SID = 3,This is the absolute maximum motor current value.
                                 // There are also maximum current values for the specific process steps
                                 // (see….), but this parameter is the limit. Unit is [mA].
    uint16_t u16MaxPeakCurrent;  // SID = 4,This value controls the threshold of the peak current comparator.
                                 // The peak current detection is built in for the protection of the device
                                 // when a latch up situation occurs. Unit is [A].
    uint16_t u16TorqueSensorType;// SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
    uint16_t u16MaxDutyCycle;    // SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]
    uint16_t u16MaxTorque;       // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
    uint16_t u16PWMFreq;         // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
    uint16_t u16MaxRPM;          // SID = 9,This is the absolute maximum motor rpm value.
                                 // There are also rpm values for the specific process steps (see….) ,
                                 // but this parameter is the limit. Unit is [rpm].
    uint16_t u16MaxSlope;        // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                 // There are also rpm slope values for the specific process steps (see….) ,
                                 // but this parameter is the limit. Unit is [rpm/s].
    uint16_t u16MinBusVolt;      // SID = 11,Minimal Bus Voltage of the Power Supply.
                                 // Underneath this Voltage the tool doesn’t work.
                                 // Unit is [0,1V] (600 = 60V).
    uint16_t u16MaxBusVolt;      // SID = 12,Maximal Bus Voltage of the Power Supply.
                                 // Above this Voltage the tool doesn’t work. Unit is [0,1V].
    uint16_t u16StartDutyCycle;  // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].
                                 // (20 = 10%)
    uint16_t u16GearBoxRatio;    // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    uint16_t u32StartInp;        // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    uint16_t u32RevInp;          // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
    uint16_t u16RevRpm;          // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
    uint16_t u16RevSlope;        // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
    uint16_t u16RevMaxCurrent;   // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."
    uint16_t u16RevMaxTorque;    // SID = 20,"Maximum Torque of reverse drive.
                                 // Value is 0- 1862 (max TMD Raw Value)"
    uint16_t u16ErrorIdleTime;   // SID = 21,"Idle time of the motor controller after a Error condition.
                                 // Unit is [ms]."
    uint16_t u16BackLash;        // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or
                                 // Hallsensors are used for angle Positioning. Unit is [Increments]."
    uint16_t u16PGain;           // SID = 23,Proportional Gain for the RPM Regulator.
    uint16_t u16IGain;           // SID = 24,Integral Gain for the RPM Regulator.
    uint16_t u16Encoder;         // SID = 25,"0 = No Encoder (positioning with Hallsensors).
                                 // 1 = 200p Encoder"
}McbID2Struct;
// Step Parameter.(ID3)
typedef struct
{
    // Step name.
    uint16_t u8StepID;               // 3XXX
    std::string u8StepName;          // SID = 1,32 byte long string which contains the name of the step.
    // property.
    uint16_t u16StepRpm;             // SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
    uint16_t u16StepSlope;           // SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
    uint16_t u16StepMaxCurrent;      // SID = 4,Maximum current of this step. Unit is [mA].
    uint16_t u16StepMaxTorque;       // SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
    uint16_t u16StepMaxRevol;        // SID = 6,Maximum Revolutions (after the Gearbox) of this step.
                                     // Unit is [0,01] (1000 = 10,00 Revolutions)
    uint16_t u16StepTime;            // SID = 7,Execution Time- time. Unit is [ms].
    uint32_t s32StepAngle;           // SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
    uint16_t u16StepAngleWindow;     // SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
    uint16_t u16StepTorqueWindow;    // SID = 10,Window of the torque monitoring.
                                     // Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
    uint16_t u16MinDutyCycle;        // SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)
    uint16_t u16StepFlags;           // SID = 12,See description of step flags.

    // New
    uint16_t u16WindowMode;          // SID = 13
    uint16_t u16AngleWindow2;        // SID = 14
    uint16_t u16TorqueWindow2;       // SID = 15

    // Step list in porcess,SID = 8,"Screwing process step ID Number 3000 -3499"
    // "u16ProcStepId":self.u16ProcStepId,
}McbID3Struct;
// Process Parameter.(ID4)
typedef struct
{
    // Process ID
    uint16_t u8ProcID;       // Process ID
    // Process name.
    std::string u8ProcName;     // SID = 1,"32 byte long string which contains the name of the
                                // screwing process."
    // Process parameter.
    uint16_t u16ProcPGain;   // SID = 2,Proportional Gain of the screwing process.
    uint16_t u16ProcIGain;   // SID = 3,Integral Gain of the screwing process.
    uint16_t u16ProcMaxTime; // SID = 4,"0 = No maximum process time.
                                // Every other value in [ms] is the timemonitoring of this process.
                                // It leads to an error, when the process exceeds this time."
    uint16_t u16ProcMinTime; // SID = 5,"0 = No minimum process time.
                                // Every other value in [ms] is the time monitoring of this process.
                                // It leads to an error, when the process undershoots this time."
    uint16_t u16ProcRevFunc; // SID = 6,"0 = Screwing direction of the whole process normal
                                // (tightening right) 1 = Screwing direction of the whole process
                                // reverse (tightening left)"
    uint16_t u16NbrSteps;    // SID = 7,Number of Steps in this Process
    // Process step list.
    // uint16_t step_id_1;      // SID = 2,Proportional Gain of the screwing process.
    // uint16_t step_id_2;      // SID = 3,Integral Gain of the screwing process.
    // uint16_t step_id_3;      // SID = 4,"0 = No maximum process time.
    // uint16_t step_id_4;      // SID = 5,"0 = No minimum process time.
    // uint16_t step_id_5;      // SID = 6,"0 = Screwing direction of the whole process normal
    // uint16_t step_id_6;      // SID = 7,Number of Steps in this Process
    // uint16_t step_id_7;      // SID = 7,Number of Steps in this Process
    // uint16_t step_id_8;      // SID = 7,Number of Steps in this Process
    // uint16_t step_id_9;      // SID = 7,Number of Steps in this Process
    // uint16_t step_id_10;     // SID = 7,Number of Steps in this Process
    // uint16_t step_id_11;     // SID = 7,Number of Steps in this Process
    // uint16_t step_id_12;     // SID = 7,Number of Steps in this Process

    std::array<uint16_t,12> stepid_list;   
}McbID4Struct;
// Program Parameter.(ID5)
typedef struct
{
    /* data */
    std::string u8ProgName;  // SID = 1,32 byte long string which contains the name of the Program.
    uint16_t u16ProgResFunc; // SID = 2,"0 = No Function
                             // 1 = a change of the reset flag resets the actual process.
                             // 2 = a change of the reset flag resets the actual program".
    uint16_t u16ProgRevFunc; // SID = 3,"0 = No Reverse Drive
                             // 1 = Reverse Drive is possible
                             // 2 = Reverse Drive is possible and resets the last process".
    uint16_t u16ProgRevTime; // SID = 4,Screwing Program Reverse Drive Time.
    uint16_t u16ProgErrResp; // SID = 5,"0 = Wait for acknowledge, 1 = No waiting"
    uint16_t u16NbrProc;     // SID = 6,Number of processes in this program.
    // SID = 1000-1199,Screwing Process ID Number 4000 - 4249.
    // "u16ProgProcessId"  : self.u16ProgProcessId
}McbID5Struct;
// Statistc Parameter.(ID6)
typedef struct
{
    /* data */
    uint32_t u32StartCount;      // SID = 1,Counter for Process Start.
    uint32_t u32TighteningCount; // SID = 2,Counter for Process Tightening.
    uint32_t u32IMaxCount;       // SID = 3,Counter for Overcurrent Conditions.
    uint32_t u32tMaxCount;       // SID = 4,"Counter for Overtemperature Conditions".
    uint32_t u32RevCount;        // SID = 5,Counter for Reverse Operations.
}McbID6Struct;
// ID7
typedef struct
{
    /* data */
    uint16_t u16ControlMode;    // SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                // 3= PWM Mode,4=Para Write Mode"
    uint16_t u16WorkProc;       // SID = 2,It is the ID of the actual working process.
                                // This is relevant when Control Mode = Process Mode.
    uint16_t u16WritePara2Flash;// SID = 3,Reserved because the Parameters are stored in FRAM.
    uint16_t u16WriteStep2Flash;// SID = 4,"When receiving this Parameter the selected Step will be
                                // written to Flash Memory.
                                // Value of this Parameter the ID of the Step (ID3XXX)".
    uint16_t u16WriteProc2Flash;// SID = 5,"When receiving this Parameter the selected Process will be
                                // written to Flash Memory.
                                // Value of this Parameter the ID of the Process (ID4XXX)".
    uint16_t u16WriteProg2Flash;// SID = 6,"When receiving this Parameter the Program will be written
                                // to Flash Memory.Value of this Parameter is “Don't care”".
}McbID7Struct;
// ID8
typedef struct
{
    /* data */
    uint16_t u16ManRpm;       // SID = 1, Manual Rpm. Unit is [rpm] (after the Gearbox).
    uint16_t u16ManSlope;     // SID = 2, Manual slope. Unit is [rpm/s] (after the Gearbox).
    uint16_t u16ManMaxCurrent;// SID = 3, Manual maximum current. Unit is [mA].
    uint16_t u16ManMaxTorque; // SID = 4, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
}McbID8Struct;
// ID9
typedef struct
{
    /* data */
    uint32_t su16ActError;   // SID = 1, Actual ErrorFlags of the Screwdriver.
    uint16_t u16ActProcID;   // SID = 2, Actual working Process ID. (4000- 4199).
    uint16_t u16ActProcNr;   // SID = 3, Actual working Process Number. (1- 200).
    uint16_t u16ActStepID;   // SID = 4, Actual working Step ID. (3000 - 3499).
    uint16_t u16ActStepNr;   // SID = 5, Actual working Step Nr. (1- 20).
    uint16_t u16ActCurr;     // SID = 6, Actual motor current [mA].
    uint16_t u16ActRPM;      // SID = 7, Actual motor rpm [rpm].
    uint16_t u16ActTorque;   // SID = 8, Actual Torque Value is 0- 1862 (max Raw TMD Value).
    uint16_t u16ActBusVolt;  // SID = 9, Actual Bus Voltage [0,01V] (6000 = 60V).
    uint16_t u16ActContTemp; // SID = 10, Actual motorcontroller temperature [0,1°C].
    uint16_t u16ActMotTem;   // SID = 11, Actual motor temperature [0,1°C].
    uint16_t u16MaxCurrent;  // SID = 12, Actual maximum current of the process [mA].
    uint16_t u16MaxTorqu;    // SID = 13, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
    uint32_t u32Angle;       // SID = 14, Actual angle [0,1°].
    uint32_t u32Revolutions; // SID = 15, Actual revolutions [0,01 revolutions].
}McbID9Struct;
#pragma endregion
#pragma region Telegram Struct.
// GTCS ctrl telegram strut.
typedef struct
{
    uint16_t u16Ctrlflags;             // 42496,8192,9728(鎖馬達),Flag10=Cyc
    uint16_t u16ControlMode;           //
    uint16_t u16WorkProc ;             //
    uint16_t u16CtrlProgram;           //
    uint16_t u16ManRpm;                //
    uint16_t u16ManSlope;              //
    uint16_t u16ManMaxTorque;          //
    uint16_t u16ManMaxCurrent;         //
    uint16_t u16ManRpmMode;            //
    uint16_t u8TMDControl;             //
}GtcsCtrlTelegramStrcut;
// GTCS Status telegram struct.
typedef struct
{
    uint16_t u16Statusflags;
    uint32_t u32ActError;
    uint16_t u16ActProcNr;
    uint16_t u16ActStepNr;
    uint16_t u16ActCurr;
    uint16_t u16ActTorque;
    uint16_t u16ActRPM;
    uint16_t u16MaxCurrent;
    uint16_t u16MaxTorque;
    uint32_t u32Angle;
    uint32_t u32Revolutions;
    uint16_t u16TMDFlags;
    uint16_t s16Debug;
    uint32_t s32Debug;
}GtcsStatusTelegramStrcut;
#pragma endregion
#pragma endregion
#pragma region GTCS AMS Protcol
enum AMSCMD : int {
    // CMD
    CMD300 = 1,
    CMD301,
    CMD302,
    CMD303,
    CMD310,
    CMD311,
    CMD312,
    CMD320,
    CMD321,
    CMD322,
    CMD323,
    CMD330,
    CMD340,
    // ANS
    ANS301,
    ANS302,
    ANS303,
    ANS310,
    ANS311,
    ANS312,
    ANS320,
    ANS321,
    ANS322,
    ANS323,
    ANS330,
    ANS340,
    // REQ
    REQ300,
    REQ301,
    REQ302,
    //DATA
    DATA300,
    DATA302,
};
#pragma region CMD
//CMD300
typedef struct {
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // CL,RF
}AmsCMD300Struct;
// CMD301
typedef struct {
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Job ID
    std::string str6;  // CL,RF
}AmsCMD301Struct;
// CMD302
typedef struct {
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Tool control
    std::string str6;  // CL,RF
}AmsCMD302Struct;
// CMD303
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // CL,RF
}AmsCMD303Struct;
// CMD310
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Direction
    std::string str6;  // RPM
    std::string str7;  // Target Torque
    std::string str8;  // Torque threshold
    std::string str9;  // Joint offset
    std::string str10; // Monitoring mode
    std::string str11; // Torque window
    std::string str12; // High torque
    std::string str13; // Low torque
    std::string str14; // High angle
    std::string str15; // Low angle
    std::string str16;  // CL,RF
}AmsCMD310Struct;
// CMD311
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Direction
    std::string str6;  // RPM
    std::string str7;  // Target angle
    std::string str8;  // Angle threshold
    std::string str9;  // Monitoring mode
    std::string str10; // Angle window
    std::string str11; // High torque
    std::string str12; // Low torque
    std::string str13; // High angle
    std::string str14; // Low angle
    std::string str15; // CL,RF
}AmsCMD311Struct;
// CMD312
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // RPM
    std::string str6;  // Force
    std::string str7;  // Direction
    std::string str8;  // CL,RF
}AmsCMD312Struct;
// CMD320
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Target torque
    std::string str11; // Torque threshold
    std::string str12; // Joint offset
    std::string str13; // Monitoring mode
    std::string str14; // Torque window
    std::string str15; // High torque
    std::string str16; // Low torque
    std::string str17; // High angle
    std::string str18; // Low angle
    std::string str19;  // CL,RF
}AmsCMD320Struct;
// CMD321
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Target torque
    std::string str11; // Angle threshold
    std::string str12; // Monitoring mode
    std::string str13; // Angle window
    std::string str14; // High torque
    std::string str15; // Low torque
    std::string str16; // High angle
    std::string str17; // Low angle
    std::string str18;  // CL,RF
}AmsCMD321Struct;
// CMD322
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Revolutions
    std::string str11;  // CL,RF
}AmsCMD322Struct;
// CMD323
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Time
    std::string str11;  // CL,RF
}AmsCMD323Struct;
// CMD330
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Program id
    std::string str6;  // Program name
    std::string str7;  // Step1
    std::string str8;  // Step2
    std::string str9;  // Step3
    std::string str10; // Step4
    std::string str11; // Step5
    std::string str12; // Step6
    std::string str13; // Step7
    std::string str14; // Step8
    std::string str15; // Ok time
    std::string str16; // NG stop
    std::string str17;  // CL,RF
}AmsCMD330Struct;
//CMD340
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Min temperature
    std::string str6;  // Max temperature
    std::string str7;  // Max current
    std::string str8;  // Max peak current
    std::string str9;  // torque sensor type
    std::string str10; // Max duty cycle
    std::string str11; // Max torque
    std::string str12; // PWM frequency
    std::string str13; // Max rpm
    std::string str14; // Max slope
    std::string str15; // Min bus voltage
    std::string str16; // Max bus voltage
    std::string str17; // Start duty cycle
    std::string str18; // Gear box ratio
    std::string str19; // Start input source
    std::string str20; // Reverse ipnut source
    std::string str21; // Reverse rpm
    std::string str22; // Reverse slope
    std::string str23; // Reverse max current
    std::string str24; // Reverse max torque
    std::string str25; // Error idle time
    std::string str26; // Bachlash
    std::string str27; // Proportional gain
    std::string str28; // Integral gain
    std::string str29; // Encoder

    std::string str30;  // Min Torque
    std::string str31;  // Min RPM
    std::string str32;  // Reverse Min RPM
    std::string str33;  // DmsSWVer
    std::string str34;  // DmsCoreID
    std::string str35;  // DmsSerNr
    std::string str36;  // Led
    std::string str37;  // Lever Sensitivity
    std::string str38;  // Push Sensitivity
    std::string str39;  // MotSWVer
    std::string str40;  // CL,RF

}AmsCMD340Struct;
#pragma endregion
#pragma region ANS
// ANS301
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Job ID
    std::string str6;  // CL,RF
}AmsANS301Struct;
// ANS302
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Tool control
    std::string str6;  // CL,RF
}AmsANS302Struct;
// ANS303
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // CL,RF
}AmsANS303Struct;
// ANS310
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Direction
    std::string str6;  // RPM
    std::string str7;  // Target Torque
    std::string str8;  // Torque threshold
    std::string str9;  // Joint offset
    std::string str10; // Monitoring mode
    std::string str11; // Torque window
    std::string str12; // High torque
    std::string str13; // Low torque
    std::string str14; // High angle
    std::string str15; // Low angle
    std::string str16;  // CL,RF
}AmsANS310Struct;
// ANS311
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Direction
    std::string str6;  // RPM
    std::string str7;  // Target angle
    std::string str8;  // Angle threshold
    std::string str9;  // Monitoring mode
    std::string str10; // Angle window
    std::string str11; // High torque
    std::string str12; // Low torque
    std::string str13; // High angle
    std::string str14; // Low angle
    std::string str15;  // CL,RF
}AmsANS311Struct;
// ANS312
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // RPM
    std::string str6;  // Force
    std::string str7;  // Direction
    std::string str8;  // CL,RF
}AmsANS312Struct;
// ANS320
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Target torque
    std::string str11; // Torque threshold
    std::string str12; // Joint offset
    std::string str13; // Monitoring mode
    std::string str14; // Torque window
    std::string str15; // High torque
    std::string str16; // Low torque
    std::string str17; // High angle
    std::string str18; // Low angle
    std::string str19;  // CL,RF
}AmsANS320Struct;
// ANS321
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Target torque
    std::string str11; // Angle threshold
    std::string str12; // Monitoring mode
    std::string str13; // Angle window
    std::string str14; // High torque
    std::string str15; // Low torque
    std::string str16; // High angle
    std::string str17; // Low angle
    std::string str18;  // CL,RF
}AmsANS321Struct;
// ANS322
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Revolutions
    std::string str11;  // CL,RF
}AmsANS322Struct;
// ANS323
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Step id
    std::string str6;  // Step name
    std::string str7;  // Direction
    std::string str8;  // RPM
    std::string str9;  // Delay time
    std::string str10; // Time
    std::string str11;  // CL,RF
}AmsANS323Struct;
// ANS330
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Program id
    std::string str6;  // Program name
    std::string str7;  // Step1
    std::string str8;  // Step2
    std::string str9;  // Step3
    std::string str10; // Step4
    std::string str11; // Step5
    std::string str12; // Step6
    std::string str13; // Step7
    std::string str14; // Step8
    std::string str15; // Ok time
    std::string str16; // NG stop
    std::string str17;  // CL,RF
}AmsANS330Struct;
// ANS340
typedef struct
{
    /* data */
    std::string str1;   // Header+DATA
    std::string str2;   // yyyyMMdd HH:mm:ss
    std::string str3;   // check sum ,4 chars
    std::string str4;   // Command_sn
    std::string str5;   // Min temperature
    std::string str6;   // Max temperature
    std::string str7;   // Max current
    std::string str8;   // Max peak current
    std::string str9;   // torque sensor type
    std::string str10;  // Max duty cycle
    std::string str11;  // Max torque
    std::string str12;  // PWM frequency
    std::string str13;  // Max rpm
    std::string str14;  // Max slope
    std::string str15;  // Min bus voltage
    std::string str16;  // Max bus voltage
    std::string str17;  // Start duty cycle
    std::string str18;  // Gear box ratio
    std::string str19;  // Start input source
    std::string str20;  // Reverse ipnut source
    std::string str21;  // Reverse rpm
    std::string str22;  // Reverse slope
    std::string str23;  // Reverse max current
    std::string str24;  // Reverse max torque
    std::string str25;  // Error idle time
    std::string str26;  // Bachlash
    std::string str27;  // Proportional gain
    std::string str28;  // Integral gain
    std::string str29;  // Encoder
    // New
    std::string str30;  // Min Torque
    std::string str31;  // Min RPM
    std::string str32;  // Reverse Min RPM
    std::string str33;  // DmsSWVer
    std::string str34;  // DmsCoreID
    std::string str35;  // DmsSerNr
    std::string str36;  // Led
    std::string str37;  // Lever Sensitivity
    std::string str38;  // Push Sensitivity
    std::string str39;  // MotSWVer
    std::string str40;  // CL,RF
}AmsANS340Struct;
#pragma endregion
#pragma region REQ
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // CL,RF
}AmsREQ300Struct;
// REQ301
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Min temperature
    std::string str6;  // Max temperature
    std::string str7;  // Max current
    std::string str8;  // Max peak current
    std::string str9;  // torque sensor type
    std::string str10; // Max duty cycle
    std::string str11; // Max torque
    std::string str12; // PWM frequency
    std::string str13; // Max rpm
    std::string str14; // Max slope
    std::string str15; // Min bus voltage
    std::string str16; // Max bus voltage
    std::string str17; // Start duty cycle
    std::string str18; // Gear box ratio
    std::string str19; // Start input source
    std::string str20; // Reverse ipnut source
    std::string str21; // Reverse rpm
    std::string str22; // Reverse slope
    std::string str23; // Reverse max current
    std::string str24; // Reverse max torque
    std::string str25; // Error idle time
    std::string str26; // Bachlash
    std::string str27; // Proportional gain
    std::string str28; // Integral gain
    std::string str29; // Encoder
    // New
    std::string str30;  // Min Torque
    std::string str31;  // Min RPM
    std::string str32;  // Reverse Min RPM
    std::string str33;  // DmsSWVer
    std::string str34;  // DmsCoreID
    std::string str35;  // DmsSerNr
    std::string str36;  // Led
    std::string str37;  // Lever Sensitivity
    std::string str38;  // Push Sensitivity
    std::string str39;  // MotSWVer
    std::string str40;  // CL,RF
}AmsREQ301Struct;
// REQ302
typedef struct
{
    /* data */
    std::string str1;  // Header+DATA
    std::string str2;  // yyyyMMdd HH:mm:ss
    std::string str3;  // check sum ,4 chars
    std::string str4;  // Command_sn
    std::string str5;  // Target Torque
    std::string str6;  // Max Current
    std::string str7;  // Angle Selflearning
    std::string str8;  // SI Angle Monitoring
    std::string str9;  // SI Angle Positioning
    std::string str10; // Sl Wait Start
    std::string str11; // SI Turn Back
    std::string str12;  // CL,RF
}AmsREQ302Struct;
#pragma endregion
#pragma region DATA
// DATA300
typedef struct
{
    /* data */
    std::string header;         // str1:Header+DATA
    std::string datetime;       // str2:yyyyMMdd HH:mm:ss
    std::string checksum;       // str3:check sum ,4 chars
    std::string cmdsn;          // str4:Command_sn
    std::string dervicetype;    // str5:Device type
    std::string toolsn;         // str6:Tool SN
    std::string dervicesn;      // str7:Device SN
    std::string jobid;          // str8:Job ID
    std::string seqid;          // str9:Sequence ID
    std::string progid;         // str10:Program ID
    std::string stepid;         // str11:Step ID
    std::string dircetion;      // str12:Direction
    std::string torqueuint;     // str13:Torque unit
    std::string inc_dec;        // str14:INC/DEC
    std::string last_screwcnt;  // str15:Last_screw_count
    std::string max_screwcnd;   // str16:Max_screw_count
    std::string fasteningtime;  // str17:Fastening time
    std::string acttorque;      // str18:Torque
    std::string actangle;       // str19:Angle
    std::string maxtorque;      // str20:Max Torque
    std::string revolutions;    // str21:Revolutions
    std::string status;         // str22:Status
    std::string inputio;        // str23:Inputio
    std::string outputio;       // str24:Outputio
    std::string errmsg;         // str25:Error Masseage
    std::string toolcnt;        // str26:Tool Count
    std::string actrpm;         // str27:RPM
    std::string toolstatus;     // str28:Tool status
    std::string clrf;           // CL,RF
}AmsDATA300Struct;
// DATA301
typedef struct
{
    /* data */
    std::string str1;   // Header+DATA
    std::string str2;   // yyyyMMdd HH:mm:ss
    std::string str3;   // check sum ,4 chars
    std::string str4;   // Command_sn
    std::string str5;   // status
    std::string str6;   // CL,RF
}AmsDATA302Struct;
#pragma endregion
#pragma endregion
#pragma region GTCS Database Struct
// Database.
class GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseBaseInfo(/* args */);
    ~GtcsDatabaseBaseInfo();
    std::string dbfilePath = "";
    std::string dbtablename  = "";
    std::vector<std::string> columnnames;
    std::map<std::string,std::string> type;
    std::map<std::string,std::string> data;
    virtual void InitialColumnName(){};
    virtual void InitilaTypeList(){};
    void InitialDataStruct();
    // void SetDataValue(std::string *ptr);
    void SetDataValue(std::map<std::string,std::string> &pDatavalue);
};
// Gtcsdatabase deviece information.
class GtcsDatabaseDeviceInfo : public GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseDeviceInfo(/* args */);
    ~GtcsDatabaseDeviceInfo();
    std::string dbtablename = "device";
    void InitialColumnType();
    void InitialColumnName();
};
// Gtcs database basic information.
class GtcsDatabaseBasicInfo : public GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseBasicInfo(/* args */);
    ~GtcsDatabaseBasicInfo();
    std::string dbtablename = "basic";
    void InitialColumnType();
    void InitialColumnName();
};

// Gtcs database program information.
class GtcsDatabaseJobInfo : public GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseJobInfo(/* args */);
    ~GtcsDatabaseJobInfo();
    std::string dbtablename = "jobsequence";
    void InitialColumnType();
    void InitialColumnName();
};

// Gtcs database program information.
class GtcsDatabaseSequenceInfo : public GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseSequenceInfo (/* args */);
    ~GtcsDatabaseSequenceInfo();
    std::string dbtablename = "program";
    void InitialColumnType();
    void InitialColumnName();
};

// Gtcs database step information.
class GtcsDatabaseStepInfo : public GtcsDatabaseBaseInfo
{
private:
    /* data */
public:
    GtcsDatabaseStepInfo (/* args */);
    ~GtcsDatabaseStepInfo();
    std::string dbtablename = "step";
    void InitialColumnType();
    void InitialColumnName();
};
#pragma endregion
#pragma region 
// Gtcs database screw data information.
class GtcsScrewStatusbaseInfo :public GtcsDatabaseBaseInfo
{
private:
public:
    GtcsScrewStatusbaseInfo();
    ~GtcsScrewStatusbaseInfo();
    std::string dbtablename = "screwstatus";
    void InitialColumnType();
    void InitialColumnName();
};
#pragma endregion
#pragma region GTCS JobSequence
// Gtcs step data struct.
typedef struct
{
    int job_id;                // colunm index = 0
    int seq_id;                // colunm index = 1
    int target_type;           // colunm index = 2
    std::string program_name;  // colunm index = 3
    int ScrewStepID;           // colunm index = 4
    std::string u8StepName;    // colunm index = 5
    int u16StepRpm;            // colunm index = 6
    float u16StepMaxTorque;    // colunm index = 7
    float u16StepMaxRevol;     // colunm index = 8
    float u16StepTime;         // colunm index = 9
    float u16StepAngle;        // colunm index = 10
    float u16StepAngleWindow;  // colunm index = 11
    float u16StepTorqueWindow; // colunm index = 12
    float ScrewHiTorque;       // colunm index = 13
    float ScrewLoTorque;       // colunm index = 14
    float ScrewHiAngle;        // colunm index = 15
    float ScrewLoAngle;        // colunm index = 16
    float TorqueThreshold;     // colunm index = 17
    float TorqueJointOffset;   // colunm index = 18
    float StepDelaytime;       // colunm index = 19
    int ScrewStepDirection;    // colunm index = 20
    int StepMonitoringMode;    // colunm index = 21
    int off_set;               // colunm index = 22
    int enable_downshift;      // colunm index = 23
    float downshift_torque;    // colunm index = 24
    int downshift_speed;       // colunm index = 25
}GtcsStepDataStruct;

// Gtcs Sequence struct.
typedef struct 
{
    // Basic information.
    int job_id;               // colunm index = 0
    int seq_id;               // colunm index = 1
    std::string program_name; // colunm index = 2
    int ok_time;              // colunm index = 3
    int ng_stop;              // colunm index = 4
    int tr;                   // colunm index = 5
    int ok_seq;               // colunm index = 6
    float ok_seq_time;        // colunm index = 7
    int seq_stop;             // colunm index = 8
    float program_mintime;    // colunm index = 9
    float program_maxtime;    // colunm index = 10
    // Step list. 
    std::vector<GtcsStepDataStruct> steplist;
}GtcsSequenceDataStruct;

// Gtcs Sequence struct.
typedef struct
{
    // Basic information.
    int jobid;                // colunm index = 1
    std::string job_name;     // colunm index = 2
    int unscrew_direction;    // colunm index = 3
    int unscrew_force;        // colunm index = 4
    int unscrew_rpm;          // colunm index = 5
    int enable_unscrew_force; // colunm index = 6
    // Sequemce list.
    std::vector<GtcsSequenceDataStruct> sequencelist; 
}GtcsJobStruct;
#pragma endregion

#pragma region GTCS Screw sequence handler
class ScrewSequenceHandler
{
public:
    ScrewSequenceHandler(){};
    ~ScrewSequenceHandler(){};
    int statusnum = (int)LOCKED_STATUS::IDLE;
    bool IsEnable = false;
    int screwcounter = 0;    
    int maxscrewcounter = 0;
    bool screwok  = false;
    bool screwcounterlocked = false;
    bool screwrunning = false;
    bool loosen   = false;
};
// Gtcs
class GtcsScrewSequenceHandler : public ScrewSequenceHandler
{
private:
    /* data */
public:
    GtcsJobStruct GtcsJob;
    uint16_t u16GearBoxRatio;    // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    uint16_t u32StartInp;        // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    uint16_t currentstatusnum = 0; 
    std::string currentstatus = "___________";
    std::string currentstatusmessage = "___________";
    int currentsequenceindex = 0;
    int lastsequenceindex = 0;
    uint16_t currenjobid = 0;
    int currentsequenceid = 0;
    uint16_t currentstepid = 0; 
    uint16_t laststepid = 1;
    std::string currentprogramname = "_";   
    uint16_t batchmode = (uint16_t)GTCS_BATCH_MODE::DEC;  // DEC = 0, INC = 1;
    uint16_t torqueunit = 0;
    uint16_t inputstatus = 0;
    uint16_t outputstatus = 0; 
    clock_t fastenstartclock;
    uint32_t fasteningtime;
    
    GtcsScrewSequenceHandler(/* args */);
    ~GtcsScrewSequenceHandler();
};
#pragma endregion

#pragma region Gtcs GPIO
typedef struct
{
public:
    int jobid;      // 
    int pinnum;     // 
    int plustype;   // 
}GtcsGPIOInputStruct;
#pragma endregion
