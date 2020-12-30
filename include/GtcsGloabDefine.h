#ifndef _GTCS_TYPE_DEFINE_
#define _GTCS_TYPE_DEFINE_
// #define _GTCS_TEST_
#pragma once
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
#include <string>
#include <list>
#include <map>

#pragma region GTCS MCB Protcol
#pragma region Parameter
// // Identification Parameter.(MID1)
// enum MCBMAID : int{
//     ID1 = 1,
//     ID2,
//     ID3,
//     ID4,
//     ID5,
//     ID6,
//     ID7,
//     ID8,
//     ID9,
// };
// 
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

enum MCB_RT_STATUS : int{
    OK          = 1,
    OK_SEQUENCE,
    OK_JOB     ,
    NG_F       , 
    NS_F       ,
    NGQ        ,
    NGA        ,
    NG_MCB     ,
    RUNNING    ,
    REVERSE    ,
    IDLE       ,
};

enum MAIN_FSM : int{
    READY = 0,
    SETTING,
    ALARM,
    CHECK_SYS,
    INITIAL,
};

enum DB_FSM :int {
    R_RAM_BAIIC_PARA = 0,
    W_RAM_BAIIC_PARA ,
};
// Enum MCB finite-state-machine status.
enum MCB_FSM:int {
    NORMAL_POLLING = 0,
    ADVANCE_POLLING,
    SETTING_CTRL_TELEGRAM,
    WRITE_MCB_BASIC,
    READ_MCB_BASIC,
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

    // Stepo list in porcess,SID = 8,"Screwing process step ID Number 3000 -3499"
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
    uint16_t step_id_1;      // SID = 2,Proportional Gain of the screwing process.          
    uint16_t step_id_2;      // SID = 3,Integral Gain of the screwing process.    
    uint16_t step_id_3;      // SID = 4,"0 = No maximum process time.
    uint16_t step_id_4;      // SID = 5,"0 = No minimum process time.
    uint16_t step_id_5;      // SID = 6,"0 = Screwing direction of the whole process normal 
    uint16_t step_id_6;      // SID = 7,Number of Steps in this Process
    uint16_t step_id_7;      // SID = 7,Number of Steps in this Process
    uint16_t step_id_8;      // SID = 7,Number of Steps in this Process
    uint16_t step_id_9;      // SID = 7,Number of Steps in this Process
    uint16_t step_id_10;     // SID = 7,Number of Steps in this Process
    uint16_t step_id_11;     // SID = 7,Number of Steps in this Process
    uint16_t step_id_12;     // SID = 7,Number of Steps in this Process
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
    uint16_t u16ControlMode;     // SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                    // 3= PWM Mode,4=Para Write Mode"
    uint16_t u16WorkProc;        // SID = 2,It is the ID of the actual working process. 
                                    // This is relevant when Control Mode = Process Mode.
    uint16_t u16WritePara2Flash; // SID = 3,Reserved because the Parameters are stored in FRAM.
    uint16_t u16WriteStep2Flash; // SID = 4,"When receiving this Parameter the selected Step will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Step (ID3XXX)".
    uint16_t u16WriteProc2Flash; // SID = 5,"When receiving this Parameter the selected Process will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Process (ID4XXX)".
    uint16_t u16WriteProg2Flash; // SID = 6,"When receiving this Parameter the Program will be written 
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
    uint16_t u16Ctrlflags;             //42496,8192,9728(鎖馬達),Flag10=Cyc
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
    std::string str30;  // CL,RF
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
    std::string str30;  // CL,RF
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
    // 
    std::string str30; //  
    std::string str31; // 
    std::string str32; // 
    std::string str33; // 
    std::string str34; // 
    std::string str35; // 
    std::string str36; // 
    std::string str37; // 
    std::string str38; // 
    std::string str39; // 

    std::string str40; // CL,RF
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
    std::string header;      // str1:Header+DATA
    std::string datetime;    // str2:yyyyMMdd HH:mm:ss
    std::string checksum;    // str3:check sum ,4 chars
    std::string cmdsn;       // str4:Command_sn
    std::string dervicetype; // str5:Device type
    std::string toolsn;      // str6:Tool SN
    std::string dervicesn;   // str7:Device SN
    std::string jobid;       // str8:Job ID
    std::string seqid;       // str9:Sequence ID
    std::string progid;      // str10:Program ID
    std::string stepid;      // str11:Step ID
    std::string dircetion;   // str12:Direction
    std::string torqueuint;    // str13:Torque unit
    std::string inc_dec;       // str14:INC/DEC
    std::string last_screwcnt; // str15:Last_screw_count
    std::string max_screwcnd;  // str16:Max_screw_count
    std::string fasteningtime; // str17:Fastening time
    std::string acttorque;     // str18:Torque
    std::string actangle;      // str19:Angle
    std::string maxtorque;     // str20:Max Torque
    std::string revolutions;   // str21:Revolutions
    std::string status;        // str22:Status
    std::string inputio;       // str23:Inputio
    std::string outputio;      // str24:Outputio
    std::string errmsg;        // str25:Error Masseage
    std::string toolcnt;       // str26:Tool Count
    std::string actrpm;        // str27:RPM
    std::string toolstatus;    // str28:Tool status
    std::string clrf;          // CL,RF
}AmsDATA300Struct;
// DATA301
typedef struct 
{
    /* data */
    std::string str1; // Header+DATA
    std::string str2; // yyyyMMdd HH:mm:ss
    std::string str3; // check sum ,4 chars        
    std::string str4; // Command_sn
    std::string str5; // status
    std::string str6;  // CL,RF
}AmsDATA302Struct;
#pragma endregion
#pragma endregion
#pragma region GTCS Database Struct
typedef struct 
{
    /* data */
    std::string mintemp;           // Min temperature       (REAL)
    std::string maxtemp;           // Max temperature       (REAL)
    std::string maxcurrent;        // Max current           (REAL)
    std::string maxpeakcurrent;    // Max peak current      (INTEGER)
    std::string torquesensortype;  // torque sensor type    (INTEGER)
    std::string maxdutycycle;      // Max duty cycle        (REAL)
    std::string maxtorque;         // Max torque            (REAL)
    std::string pwmfreq;           // PWM frequency         (INTEGER)
    std::string maxrpm;            // Max rpm               (INTEGER)
    std::string maxslope;          // Max slope             (INTEGER)
    std::string minbusvolt;        // Min bus voltage       (REAL)
    std::string maxbusvolt;        // Max bus voltage       (REAL)
    std::string startdutycycle;    // Start duty cycle      (REAL)
    std::string gearboxratio;      // Gear box ratio        (REAL)
    std::string startinp;          // Start input source    (INTEGER)
    std::string revinp;            // Reverse ipnut source  (INTEGER)
    std::string revrpm;            // Reverse rpm           (INTEGER)
    std::string revslope;          // Reverse slope         (INTEGER)
    std::string revmaxcurrent;     // Reverse max current   (INTEGER)
    std::string revmaxtorque;      // Reverse max torque    (REAL)
    std::string erroridletime;     // Error idle time       (INTEGER)
    std::string backlash;          // Bachlash              (INTEGER)
    std::string pgain;             // Proportional gain     (INTEGER)
    std::string igain;             // Integral gain         (INTEGER)
    std::string encoder;           // Encoder               (INTEGER)
    // 
    std::string mintorque;         // (REAL)
    std::string minrpm;            // (INTEGER)
    std::string revminrpm;         // (INTEGER)
    std::string dmsswver;          // (INTEGER)
    std::string dmscoreid;         // (INTEGER)
    std::string dmssernr;          // (INTEGER)
    std::string led;               // (INTEGER)
    std::string lever_sensitivity; // (INTEGER)
    std::string push_sensitivity;  // (INTEGER) 
    std::string motswver;          // (TEXT)
    std::string end;               // CL,RF
}GtcsDatabaseBasicStruct;

//
class GtcsDataBaseMap
{
private:
    /* data */
public:
    std::map<std::string,std::string> basic;
    GtcsDataBaseMap(GtcsDatabaseBasicStruct basic_struct)
    {
        /* data */
        basic["mintemp"]           = basic_struct.mintemp;  // Min temperature       (REAL)
        basic["maxtemp"]           = basic_struct.maxtemp;  // Max temperature       (REAL)
        basic["maxcurrent"]        = basic_struct.maxcurrent;  // Max current           (REAL)
        basic["maxpeakcurrent"]    = basic_struct.maxpeakcurrent;  // Max peak current      (INTEGER)
        basic["torquesensortype"]  = basic_struct.torquesensortype;  // torque sensor type    (INTEGER)
        basic["maxdutycycle"]      = basic_struct.maxdutycycle;  // Max duty cycle        (REAL)
        basic["maxtorque"]         = basic_struct.maxtorque;  // Max torque            (REAL)
        basic["pwmfreq"]           = basic_struct.pwmfreq;  // PWM frequency         (INTEGER)
        basic["maxrpm"]            = basic_struct.maxrpm;  // Max rpm               (INTEGER)
        basic["maxslope"]          = basic_struct.maxslope;  // Max slope             (INTEGER)
        basic["minbusvolt"]        = basic_struct.minbusvolt;  // Min bus voltage       (REAL)
        basic["maxbusvolt"]        = basic_struct.maxbusvolt;  // Max bus voltage       (REAL)
        basic["startdutycycle"]    = basic_struct.startdutycycle;  // Start duty cycle      (REAL)
        basic["gearboxratio"]      = basic_struct.gearboxratio;  // Gear box ratio        (REAL)
        basic["startinp"]          = basic_struct.startinp;  // Start input source    (INTEGER)
        basic["revinp"]            = basic_struct.revinp;  // Reverse ipnut source  (INTEGER)
        basic["revrpm"]            = basic_struct.revrpm;  // Reverse rpm           (INTEGER)
        basic["revslope"]          = basic_struct.revslope;  // Reverse slope         (INTEGER)
        basic["revmaxcurrent"]     = basic_struct.revmaxcurrent;  // Reverse max current   (INTEGER)
        basic["revmaxtorque"]      = basic_struct.revmaxtorque;  // Reverse max torque    (REAL)
        basic["erroridletime"]     = basic_struct.erroridletime;  // Error idle time       (INTEGER)
        basic["backlash"]          = basic_struct.backlash;  // Bachlash              (INTEGER)
        basic["pgain"]             = basic_struct.pgain;  // Proportional gain     (INTEGER)
        basic["igain"]             = basic_struct.igain;  // Integral gain         (INTEGER)
        basic["encoder"]           = basic_struct.encoder;  // Encoder               (INTEGER)
        // 
        basic["mintorque"]         = basic_struct.mintorque;  // (REAL)
        basic["minrpm"]            = basic_struct.minrpm;  // (INTEGER)
        basic["revminrpm"]         = basic_struct.revminrpm;  // (INTEGER)
        basic["dmsswver"]          = basic_struct.dmsswver;  // (INTEGER)
        basic["dmscoreid"]         = basic_struct.dmscoreid;  // (INTEGER)
        basic["dmssernr"]          = basic_struct.dmssernr;  // (INTEGER)
        basic["led"]               = basic_struct.led;  // (INTEGER)
        basic["lever_sensitivity"] = basic_struct.lever_sensitivity;  // (INTEGER)
        basic["push_sensitivity"]  = basic_struct.push_sensitivity;  // (INTEGER) 
        basic["motswver"]          = basic_struct.motswver;  // (TEXT)
        basic["end"]               = basic_struct.end;  // CL,RF
        
    };
    ~GtcsDataBaseMap(){};
};
#pragma endregion
#endif