#ifndef _GTCS_TELEGRAM_
#define _GTCS_TELEGRAM_
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
#include <iostream>          // 
#include <array>             // 
#include <cstring>           // 
#include "gtcstypedefine.h"

#pragma region GTCS Parameter.
// Class strcut.
class GTCSParameter
{
private:
    /* data */
public:
    #pragma region Bulletin(GTCSMCB)
    // Identification Parameter.
    ID1 MID1 = {
        .u8MotSWVer  = "",    // SID = 1,Software Version Motorcontrolle.
        .u8MotCoreID = "",    // SID = 2,Unique Core ID from Motor- STM32 Conrtroller.
        .u8MotSerNr  = "",    // SID = 3,Serial Number of the Motor Controller.
        .u8DmsSWVer  = "",    // SID = 4,Software Version DMS- Controller.
        .u8DmsCorel  = "",    // SID = 5,Unique Core ID from DMS- STM32.
        .u8DmsSerNr  = "",    // SID = 6,Serial Number of the DMS- Controller.
    };    
    // Basic Parameter.
    ID2 MID2 = {
        .s16MinTemp           = "0",// SID = 1,Minimal Temperature of the motor and the motorcontroller. 
                                    // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
        .s16MaxTemp           = "0",// SID = 2,Maximal Temperature of the motor and the motorcontroller. 
                                    // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
        .u16MaxCurrent        = "0",// SID = 3,This is the absolute maximum motor current value. 
                                    // There are also maximum current values for the specific process steps 
                                    // (see….), but this parameter is the limit. Unit is [mA].
        .u16MaxPeakCurrent    = "0",// SID = 4,This value controls the threshold of the peak current comparator.
                                    // The peak current detection is built in for the protection of the device 
                                    // when a latch up situation occurs. Unit is [A].
        .u16TorqueSensorType  = "0",// SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
        .u16MaxDutyCycle      = "0",// SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]
        .u16MaxTorque         = "0",// SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
        .u16PWMFreq           = "0",// SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
        .u16MaxRPM            = "0",// SID = 9,This is the absolute maximum motor rpm value. 
                                    // There are also rpm values for the specific process steps (see….) , 
                                    // but this parameter is the limit. Unit is [rpm].
        .u16MaxSlope          = "0",// SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                    // There are also rpm slope values for the specific process steps (see….) , 
                                    // but this parameter is the limit. Unit is [rpm/s].
        .u16MinBusVolt        = "0",// SID = 11,Minimal Bus Voltage of the Power Supply. 
                                    // Underneath this Voltage the tool doesn’t work. 
                                    // Unit is [0,1V] (600 = 60V).
        .u16MaxBusVolt        = "0",// SID = 12,Maximal Bus Voltage of the Power Supply. 
                                    // Above this Voltage the tool doesn’t work. Unit is [0,1V].
        .u16StartDutyCycle    = "0",// SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].
                                    // (20 = 10%)
        .u16GearBoxRatio      = "0",// SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
        .u32StartInp          = "0",// SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
        .u32RevInp            = "0",// SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
        .u16RevRpm            = "0",// SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
        .u16RevSlope          = "0",// SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
        .u16RevMaxCurrent     = "0",// SID = 19,"Maximum Current of reverse drive. Unit is [mA]."
        .u16RevMaxTorque      = "0",// SID = 20,"Maximum Torque of reverse drive. 
                                    // Value is 0- 1862 (max TMD Raw Value)"
        .u16ErrorIdleTime     = "0",// SID = 21,"Idle time of the motor controller after a Error condition.
                                    // Unit is [ms]."
        .u16BackLash          = "0",// SID = 22,"Backlash of the gearbox. The value depends if a Encoder or 
                                    // Hallsensors are used for angle Positioning. Unit is [Increments]."
        .u16PGain             = "0",// SID = 23,Proportional Gain for the RPM Regulator.
        .u16IGain             = "0",// SID = 24,Integral Gain for the RPM Regulator.
        .u16Encoder           = "0",// SID = 25,"0 = No Encoder (positioning with Hallsensors). 
                                    // 1 = 200p Encoder"
    };
    //  Step Parameter.
    ID3 MID3 = {
        // Step name.
        .u8StepID            = "0",// 3XXX
        .u8StepName          = "-",// SID = 1,32 byte long string which contains the name of the step.
        // property.
        .u16StepRpm          = "0",// SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
        .u16StepSlope        = "0",// SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
        .u16StepMaxCurrent   = "0",// SID = 4,Maximum current of this step. Unit is [mA].
        .u16StepMaxTorque    = "0",// SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
        .u16StepMaxRevol     = "0",// SID = 6,Maximum Revolutions (after the Gearbox) of this step. 
                                   // Unit is [0,01] (1000 = 10,00 Revolutions)
        .u16StepTime         = "0",// SID = 7,Execution Time- time. Unit is [ms].
        .u16StepAngle        = "0",// SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
        .u16StepAngleWindow  = "0",// SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
        .u16StepTorqueWindow = "0",// SID = 10,Window of the torque monitoring. 
                                   // Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
        .u16MinDutyCycle     = "0",// SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)
        .u16StepFlags        = "0",// SID = 12,See description of step flags.

        // New 
        .u16StepAngleUpperLimit  = "0",// u16StepAngleUpperLimit
        .u16StepAngleLowerLimit  = "0",
        .u16StepTorqueUpperLimit = "0",
        .u16StepTorqueLowerLimit = "0",
        .u16StepMonitorType      = "0",

        // KLS Define
        .u16StepDelaytime        = "0",
        // Stepo list in porcess,SID = 8,"Screwing process step ID Number 3000 -3499"
        // "u16ProcStepId":self.u16ProcStepId,   
    };   
    //  Process Parameter.
    ID4 MID4 = {
        // Process ID
        .u8ProcID           = "0", // Process ID
        // Process name.
        .u8ProcName         = "-", // SID = 1,"32 byte long string which contains the name of the 
                                   // screwing process."
        // Process parameter.
        .u16ProcPGain       = "0", // SID = 2,Proportional Gain of the screwing process.          
        .u16ProcIGain       = "0", // SID = 3,Integral Gain of the screwing process.    
        .u16ProcMaxTime     = "0", // SID = 4,"0 = No maximum process time.
                                   // Every other value in [ms] is the timemonitoring of this process.
                                   // It leads to an error, when the process exceeds this time."
        .u16ProcMinTime     = "0", // SID = 5,"0 = No minimum process time.
                                   // Every other value in [ms] is the time monitoring of this process. 
                                   // It leads to an error, when the process undershoots this time."
        .u16ProcRevFunc     = "0", // SID = 6,"0 = Screwing direction of the whole process normal 
                                   // (tightening right) 1 = Screwing direction of the whole process 
                                   // reverse (tightening left)"
        .u16NbrSteps        = "0", // SID = 7,Number of Steps in this Process
    };
    // Program Parameter.
    ID5 MID5 = {
        .u8ProgName         = "0", // SID = 1,32 byte long string which contains the name of the Program.
        .u16ProgResFunc     = "0", // SID = 2,"0 = No Function 
                                   // 1 = a change of the reset flag resets the actual process.
                                   // 2 = a change of the reset flag resets the actual program".  
        .u16ProgRevFunc     = "0", // SID = 3,"0 = No Reverse Drive
                                   // 1 = Reverse Drive is possible
                                   // 2 = Reverse Drive is possible and resets the last process".
        .u16ProgRevTime     = "0", // SID = 4,Screwing Program Reverse Drive Time.
        .u16ProgErrResp     = "0", // SID = 5,"0 = Wait for acknowledge, 1 = No waiting"
        .u16NbrProc         = "0", // SID = 6,Number of processes in this program.
        // SID = 1000-1199,Screwing Process ID Number 4000 - 4249.
        // "u16ProgProcessId"  : self.u16ProgProcessId 
    };
    // Statistc Parameter.
    ID6 MID6 = {
        .u32StartCount      = "0", //SID = 1,Counter for Process Start.
        .u32TighteningCount = "0", // SID = 2,Counter for Process Tightening.
        .u32IMaxCount       = "0", // SID = 3,Counter for Overcurrent Conditions.
        .u32tMaxCount       = "0", // SID = 4,"Counter for Overtemperature Conditions".
        .u32RevCount        = "0", // SID = 5,Counter for Reverse Operations.
        // SID = 6,Last 10 Screwdriver Errors (see Errorlist….). 
        // "u32RevCountList":self.u32RevCountList 
    };
    // Ctrl Parameter.
    ID7 MID7 = {
        .u16ControlMode      = "0", // SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                    // 3= PWM Mode,4=Para Write Mode"
        .u16WorkProc         = "0", // SID = 2,It is the ID of the actual working process. 
                                    // This is relevant when Control Mode = Process Mode.
        .u16WritePara2Flash  = "0", // SID = 3,Reserved because the Parameters are stored in FRAM.
        .u16WriteStep2Flash  = "0", // SID = 4,"When receiving this Parameter the selected Step will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Step (ID3XXX)".
        .u16WriteProc2Flash  = "0", // SID = 5,"When receiving this Parameter the selected Process will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Process (ID4XXX)".
        .u16WriteProg2Flash  = "0", // SID = 6,"When receiving this Parameter the Program will be written 
                                    // to Flash Memory.Value of this Parameter is “Don't care”".
    };
    // Manual Parameter.
    ID8 MID8 = {
        .u16ManRpm        = "0",   // SID = 1, Manual Rpm. Unit is [rpm] (after the Gearbox).
        .u16ManSlope      = "0",   // SID = 2, Manual slope. Unit is [rpm/s] (after the Gearbox).
        .u16ManMaxCurrent = "0",   // SID = 3, Manual maximum current. Unit is [mA].
        .u16ManMaxTorque  = "0",   // SID = 4, Maximum Torque Value is 0- 1862 (max Raw TMD Value).   
    };
    // Status Parameter.
    ID9 MID9 = {
        .su16ActError    = "0", // SID = 1, Actual ErrorFlags of the Screwdriver.
        .u16ActProcID    = "0", // SID = 2, Actual working Process ID. (4000- 4199).
        .u16ActProcNr    = "0", // SID = 3, Actual working Process Number. (1- 200).
        .u16ActStepID    = "0", // SID = 4, Actual working Step ID. (3000 - 3499).
        .u16ActStepNr    = "0", // SID = 5, Actual working Step Nr. (1- 20).
        .u16ActCurr      = "0", // SID = 6, Actual motor current [mA].
        .u16ActRPM       = "0", // SID = 7, Actual motor rpm [rpm].
        .u16ActTorque    = "0", // SID = 8, Actual Torque Value is 0- 1862 (max Raw TMD Value).
        .u16ActBusVolt   = "0", // SID = 9, Actual Bus Voltage [0,01V] (6000 = 60V).
        .u16ActContTemp  = "0", // SID = 10, Actual motorcontroller temperature [0,1°C].
        .u16ActMotTem    = "0", // SID = 11, Actual motor temperature [0,1°C].
        .u16MaxCurrent   = "0", // SID = 12, Actual maximum current of the process [mA].
        .u16MaxTorqu     = "0", // SID = 13, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
        .u32Angle        = "0", // SID = 14, Actual angle [0,1°].
        .u32Revolutions  = "0", // SID = 15, Actual revolutions [0,01 revolutions].
    };
    #pragma endregion  
    // Constructor.
    GTCSParameter(/* args */){};
    // Disstructor.
    ~GTCSParameter(){};
};
#pragma endregion
#pragma region Telgram
class GtcsMCBTelegram
{
private:
    /* data */
    static GtcsMCBTelegram* instance;
    GtcsMCBTelegram(/* args */);
public:
    ~GtcsMCBTelegram();    
    static GtcsMCBTelegram* getInstance();
    GTCSParameter gtcsParameter;
};

class fuckyou
{
private:
    /* data */
public:
    fuckyou(/* args */);
    ~fuckyou();
    void fuck();
};
#pragma endregion
#endif