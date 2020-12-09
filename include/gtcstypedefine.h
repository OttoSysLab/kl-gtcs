#ifndef _GTCS_TYPE_DEFINE_
#define _GTCS_TYPE_DEFINE_
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

#pragma region GTCS MCB Protcol
// Identification Parameter.(MID1)
typedef struct 
{
    /* data */ 
    std::string u8MotSWVer;   // SID = 1,Software Version Motorcontrolle.
    std::string u8MotCoreID ; // SID = 2,Unique Core ID from Motor- STM32 Conrtroller.
    std::string u8MotSerNr;   // SID = 3,Serial Number of the Motor Controller.
    std::string u8DmsSWVer;   // SID = 4,Software Version DMS- Controller.
    std::string u8DmsCorel;   // SID = 5,Unique Core ID from DMS- STM32.
    std::string u8DmsSerNr;   // SID = 6,Serial Number of the DMS- Controller.
}ID1;
// (MID2) 
typedef struct 
{
    /* data */
    std::string s16MinTemp;         // SID = 1,Minimal Temperature of the motor and the motorcontroller. 
                                    // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
    std::string s16MaxTemp;         // SID = 2,Maximal Temperature of the motor and the motorcontroller. 
                                    // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
    std::string u16MaxCurrent;      // SID = 3,This is the absolute maximum motor current value. 
                                    // There are also maximum current values for the specific process steps 
                                    // (see….), but this parameter is the limit. Unit is [mA].
    std::string u16MaxPeakCurrent;  // SID = 4,This value controls the threshold of the peak current comparator.
                                    // The peak current detection is built in for the protection of the device 
                                    // when a latch up situation occurs. Unit is [A].
    std::string u16TorqueSensorType;// SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
    std::string u16MaxDutyCycle;    // SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]
    std::string u16MaxTorque;       // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
    std::string u16PWMFreq;         // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
    std::string u16MaxRPM;          // SID = 9,This is the absolute maximum motor rpm value. 
                                    // There are also rpm values for the specific process steps (see….) , 
                                    // but this parameter is the limit. Unit is [rpm].
    std::string u16MaxSlope;        // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                    // There are also rpm slope values for the specific process steps (see….) , 
                                    // but this parameter is the limit. Unit is [rpm/s].
    std::string u16MinBusVolt;      // SID = 11,Minimal Bus Voltage of the Power Supply. 
                                    // Underneath this Voltage the tool doesn’t work. 
                                    // Unit is [0,1V] (600 = 60V).
    std::string u16MaxBusVolt;      // SID = 12,Maximal Bus Voltage of the Power Supply. 
                                    // Above this Voltage the tool doesn’t work. Unit is [0,1V].
    std::string u16StartDutyCycle;  // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].
                                    // (20 = 10%)
    std::string u16GearBoxRatio;    // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    std::string u32StartInp;        // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    std::string u32RevInp;          // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
    std::string u16RevRpm;          // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
    std::string u16RevSlope;        // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
    std::string u16RevMaxCurrent;   // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."
    std::string u16RevMaxTorque;    // SID = 20,"Maximum Torque of reverse drive. 
                                    // Value is 0- 1862 (max TMD Raw Value)"
    std::string u16ErrorIdleTime;   // SID = 21,"Idle time of the motor controller after a Error condition.
                                    // Unit is [ms]."
    std::string u16BackLash;        // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or 
                                    // Hallsensors are used for angle Positioning. Unit is [Increments]."
    std::string u16PGain;           // SID = 23,Proportional Gain for the RPM Regulator.
    std::string u16IGain;           // SID = 24,Integral Gain for the RPM Regulator.
    std::string u16Encoder;         // SID = 25,"0 = No Encoder (positioning with Hallsensors). 
                                    // 1 = 200p Encoder"
}ID2;
// Step Parameter.(ID3)
typedef struct 
{
    // Step name.
    std::string u8StepID;           // 3XXX
    std::string u8StepName;         // SID = 1,32 byte long string which contains the name of the step.
    // property.
    std::string u16StepRpm;         // SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
    std::string u16StepSlope;       // SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
    std::string u16StepMaxCurrent;  // SID = 4,Maximum current of this step. Unit is [mA].
    std::string u16StepMaxTorque;   // SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
    std::string u16StepMaxRevol;    // SID = 6,Maximum Revolutions (after the Gearbox) of this step. 
                                    // Unit is [0,01] (1000 = 10,00 Revolutions)
    std::string u16StepTime;        // SID = 7,Execution Time- time. Unit is [ms].
    std::string u16StepAngle;       // SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
    std::string u16StepAngleWindow; // SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
    std::string u16StepTorqueWindow;// SID = 10,Window of the torque monitoring. 
                                    // Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
    std::string u16MinDutyCycle;    // SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)
    std::string u16StepFlags;       // SID = 12,See description of step flags.

    // New 
    std::string u16StepAngleUpperLimit;// u16StepAngleUpperLimit
    std::string u16StepAngleLowerLimit;
    std::string u16StepTorqueUpperLimit;
    std::string u16StepTorqueLowerLimit;
    std::string u16StepMonitorType;

    // KLS Define
    std::string u16StepDelaytime;
    // Stepo list in porcess,SID = 8,"Screwing process step ID Number 3000 -3499"
    // "u16ProcStepId":self.u16ProcStepId,
}ID3;
// Process Parameter.(ID4)
typedef struct 
{
    // Process ID
    std::string u8ProcID;       // Process ID
    // Process name.
    std::string u8ProcName;     // SID = 1,"32 byte long string which contains the name of the 
                                // screwing process."
    // Process parameter.
    std::string u16ProcPGain;   // SID = 2,Proportional Gain of the screwing process.          
    std::string u16ProcIGain;   // SID = 3,Integral Gain of the screwing process.    
    std::string u16ProcMaxTime; // SID = 4,"0 = No maximum process time.
                                // Every other value in [ms] is the timemonitoring of this process.
                                // It leads to an error, when the process exceeds this time."
    std::string u16ProcMinTime; // SID = 5,"0 = No minimum process time.
                                // Every other value in [ms] is the time monitoring of this process. 
                                // It leads to an error, when the process undershoots this time."
    std::string u16ProcRevFunc; // SID = 6,"0 = Screwing direction of the whole process normal 
                                // (tightening right) 1 = Screwing direction of the whole process 
                                // reverse (tightening left)"
    std::string u16NbrSteps;    // SID = 7,Number of Steps in this Process
}ID4;
// Program Parameter.(ID5)
typedef struct 
{
    /* data */
    std::string u8ProgName;     // SID = 1,32 byte long string which contains the name of the Program.
    std::string u16ProgResFunc; // SID = 2,"0 = No Function 
                                // 1 = a change of the reset flag resets the actual process.
                                // 2 = a change of the reset flag resets the actual program".  
    std::string u16ProgRevFunc; // SID = 3,"0 = No Reverse Drive
                                // 1 = Reverse Drive is possible
                                // 2 = Reverse Drive is possible and resets the last process".
    std::string u16ProgRevTime; // SID = 4,Screwing Program Reverse Drive Time.
    std::string u16ProgErrResp; // SID = 5,"0 = Wait for acknowledge, 1 = No waiting"
    std::string u16NbrProc;     // SID = 6,Number of processes in this program.
    // SID = 1000-1199,Screwing Process ID Number 4000 - 4249.
    // "u16ProgProcessId"  : self.u16ProgProcessId 
}ID5;
// Statistc Parameter.(ID6)
typedef struct 
{
    /* data */
    std::string u32StartCount;      //SID = 1,Counter for Process Start.
    std::string u32TighteningCount; // SID = 2,Counter for Process Tightening.
    std::string u32IMaxCount;       // SID = 3,Counter for Overcurrent Conditions.
    std::string u32tMaxCount;       // SID = 4,"Counter for Overtemperature Conditions".
    std::string u32RevCount;        // SID = 5,Counter for Reverse Operations.
}ID6;
// ID7
typedef struct 
{
    /* data */
    std::string u16ControlMode;     // SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                    // 3= PWM Mode,4=Para Write Mode"
    std::string u16WorkProc;        // SID = 2,It is the ID of the actual working process. 
                                    // This is relevant when Control Mode = Process Mode.
    std::string u16WritePara2Flash; // SID = 3,Reserved because the Parameters are stored in FRAM.
    std::string u16WriteStep2Flash; // SID = 4,"When receiving this Parameter the selected Step will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Step (ID3XXX)".
    std::string u16WriteProc2Flash; // SID = 5,"When receiving this Parameter the selected Process will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Process (ID4XXX)".
    std::string u16WriteProg2Flash; // SID = 6,"When receiving this Parameter the Program will be written 
                                    // to Flash Memory.Value of this Parameter is “Don't care”".
}ID7;
// ID8
typedef struct 
{
    /* data */
    std::string u16ManRpm;       // SID = 1, Manual Rpm. Unit is [rpm] (after the Gearbox).
    std::string u16ManSlope;     // SID = 2, Manual slope. Unit is [rpm/s] (after the Gearbox).
    std::string u16ManMaxCurrent;// SID = 3, Manual maximum current. Unit is [mA].
    std::string u16ManMaxTorque; // SID = 4, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
}ID8;
// ID9
typedef struct 
{
    /* data */
    std::string su16ActError;   // SID = 1, Actual ErrorFlags of the Screwdriver.
    std::string u16ActProcID;   // SID = 2, Actual working Process ID. (4000- 4199).
    std::string u16ActProcNr;   // SID = 3, Actual working Process Number. (1- 200).
    std::string u16ActStepID;   // SID = 4, Actual working Step ID. (3000 - 3499).
    std::string u16ActStepNr;   // SID = 5, Actual working Step Nr. (1- 20).
    std::string u16ActCurr;     // SID = 6, Actual motor current [mA].
    std::string u16ActRPM;      // SID = 7, Actual motor rpm [rpm].
    std::string u16ActTorque;   // SID = 8, Actual Torque Value is 0- 1862 (max Raw TMD Value).
    std::string u16ActBusVolt;  // SID = 9, Actual Bus Voltage [0,01V] (6000 = 60V).
    std::string u16ActContTemp; // SID = 10, Actual motorcontroller temperature [0,1°C].
    std::string u16ActMotTem;   // SID = 11, Actual motor temperature [0,1°C].
    std::string u16MaxCurrent;  // SID = 12, Actual maximum current of the process [mA].
    std::string u16MaxTorqu;    // SID = 13, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
    std::string u32Angle;       // SID = 14, Actual angle [0,1°].
    std::string u32Revolutions; // SID = 15, Actual revolutions [0,01 revolutions].
}ID9;
#pragma endregion

#pragma region GTCS AMS Protcol
enum class AMSCMD :int{
    // CMD 
    CMD300,
    CMD301,
    CMD302,
    CMD303,
    CMD310,
    CMD311,
    CMD312,
    CMD313,
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
    std::string str30;  // CL,RF
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
    std::string str1; // str1:Header+DATA
    std::string str2; // str2:yyyyMMdd HH:mm:ss
    std::string str3; // str3:check sum ,4 chars
    std::string str4; // str4:Command_sn
    std::string str5; // str5:Device type
    std::string str6; // str6:Tool SN
    std::string str7; // str7:Device SN
    std::string str8; // str8:Job ID
    std::string str9; // str9:Sequence ID
    std::string str10; // str10:Program ID
    std::string str11; // str11:Step ID
    std::string str12; // str12:Direction
    std::string str13;  // str13:Torque unit
    std::string str14; // str14:INC/DEC
    std::string str15; // str15:Last_screw_count
    std::string str16; // str16:Max_screw_count
    std::string str17; // str17:Fastening time
    std::string str18; // str18:Torque
    std::string str19; // str19:Angle
    std::string str20; // str20:Max Torque
    std::string str21; // str21:Revolutions
    std::string str22; // str22:Status
    std::string str23; // str23:Inputio
    std::string str24; // str24:Outputio
    std::string str25; // str25:Error Masseage
    std::string str26; // str26:Tool Count
    std::string str27; // str27:RPM
    std::string str28;  // CL,RF
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

#pragma endregion
#endif