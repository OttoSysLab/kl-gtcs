#ifndef _GTCS_BULLETIN_
#define _GTCS_BULLETIN_
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
#include "GtcsGloabDefine.h"

#pragma region bulletin
#pragma region Gtcs AMS bulletin
class AMSBulletin
{
private:
    /* data */
public:
    #pragma region CMD
    AmsCMD300Struct CMD300Struct = {
        .str1 = "CMD300",                                   
        .str2 = "yyyyMMdd HH:mm:ss",                                         
        .str3 = "0",                                        
        .str4 = "0",
        .str5 = "\n\r",
    };// CMD300, 回復確認.
    AmsCMD301Struct CMD301Struct = {
        .str1 = "CMD301",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Job ID
        .str6 = "\n\r",                                     // CR,LF
    };// CMD301, 條碼切換工序.
    AmsCMD302Struct CMD302Struct = {
        .str1 = "CMD302",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Tool control
        .str6 = "\n\r",                                     // CR,LF
    };// CMD302, 工具控制命令.
    AmsCMD303Struct CMD303Struct = {
        .str1 = "CMD303",            // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss", // yyyyMMdd HH:mm:ss
        .str3 = "0",                 // check sum ,4 chars
        .str4 = "0",                 // Command_sn
        .str5 = "\n\r",                                     // CR,LF
    };// CMD303, 跳工序.
    AmsCMD310Struct CMD310Struct = {
        .str1 = "CMD310",                                  // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                       // yyyyMMdd HH:mm:ss
        .str3 = "0",                                       // check sum ,4 chars
        .str4 = "0",                                       // Command_sn
        .str5 = "0",                                       // Direction
        .str6 = "0",                                       // RPM
        .str7 = "0",                                       // Target Torque
        .str8 = "0",                                       // Torque threshold
        .str9 = "0",                                       // Joint offset
        .str10 = "0",                                       // Monitoring mode
        .str11 = "0",                                       // Torque window
        .str12 = "0",                                       // High torque
        .str13 = "0",                                       // Low torque
        .str14 = "0",                                       // High angle
        .str15 = "0",                                       // Low angle
        .str16 = "\n\r",                                     // CR,LF
    };// CMD310, 設定Normal Torque.
    AmsCMD311Struct CMD311Struct = {
        .str1  = "CMD311",                                  // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                       // yyyyMMdd HH:mm:ss
        .str3  = "0",                                       // check sum ,4 chars
        .str4  = "0",                                       // Command_sn
        .str5  = "0",                                       // Direction 
        .str6  = "0",                                       // RPM
        .str7  = "0",                                       // Target angle
        .str8  = "0",                                       // Angle threshold
        .str9  = "0",                                       // Monitoring mode
        .str10 = "0",                                       // Angle window
        .str11 = "0",                                       // High torque
        .str12 = "0",                                       // Low torque
        .str13 = "0",                                       // High angle
        .str14 = "0",                                       // Low angle
        .str15 = "\n\r",                                     // CR,LF
    };// CMD311, 設定Normal Angle.
    AmsCMD312Struct CMD312Struct = {
        .str1 = "CMD312",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // RPM 
        .str6 = "0",                                        // Force
        .str7 = "0",                                        // Direction
        .str8 = "\n\r",                                     // CR,LF
    };// CMD312, 設定Normal Reverse.
    AmsCMD320Struct CMD320Struct = {
        .str1  = "CMD320",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Target torque
        .str11 = "0",                                        // Torque threshold
        .str12 = "0",                                        // Joint offset
        .str13 = "0",                                        // Monitoring mode
        .str14 = "0",                                        // Torque window
        .str15 = "0",                                        // High torque
        .str16 = "0",                                        // Low torque
        .str17 = "0",                                        // High angle
        .str18 = "0",                                        // Low angle
        .str19 = "\n\r",                                     // CR,LF
    };// CMD320, 設定Advanced Torque.
    AmsCMD321Struct CMD321Struct = {
        .str1  = "CMD321",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Target torque
        .str11 = "0",                                        // Angle threshold
        .str12 = "0",                                        // Monitoring mode
        .str13 = "0",                                        // Angle window
        .str14 = "0",                                        // High torque
        .str15 = "0",                                        // Low torque
        .str16 = "0",                                        // High angle
        .str17 = "0",                                        // Low angle
        .str18 = "\n\r",                                     // CR,LF
    };// CMD321, 設定Advanced Angle.
    AmsCMD322Struct CMD322Struct = {
        .str1  = "CMD322",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Revolutions
        .str11 = "\n\r",                                     // CR,LF
    };// CMD322, 設定Advanced Revolutions.
    AmsCMD323Struct CMD323Struct = {
        .str1  = "CMD323",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Time
        .str11 = "\n\r",                                     // CR,LF
    };// CMD323, 設定Advanced Time.
    AmsCMD330Struct CMD330Struct = {
        .str1  = "CMD330",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Program id 
        .str6  = "0",                                        // Program name
        .str7  = "0",                                        // Step1
        .str8  = "0",                                        // Step2
        .str9  = "0",                                        // Step3
        .str10 = "0",                                        // Step4
        .str11 = "0",                                        // Step5
        .str12 = "0",                                        // Step6 
        .str13 = "0",                                        // Step7
        .str14 = "0",                                        // Step8
        .str15 = "0",                                        // Ok time
        .str16 = "0",                                        // NG stop
        .str17 = "\n\r",                                     // CR,LF
    };// CMD330, 設定Program.
    AmsCMD340Struct CMD340Struct = {
        .str1 = "CMD340",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars        
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Min temperature 
        .str6 = "0",                                        // Max temperature
        .str7 = "0",                                        // Max current
        .str8 = "0",                                        // Max peak current
        .str9 = "0",                                        // torque sensor type
        .str10 = "0",                                        // Max duty cycle
        .str11 = "0",                                        // Max torque
        .str12 = "0",                                        // PWM frequency 
        .str13 = "0",                                        // Max rpm
        .str14 = "0",                                        // Max slope
        .str15 = "0",                                        // Min bus voltage
        .str16 = "0",                                        // Max bus voltage
        .str17 = "0",                                        // Start duty cycle
        .str18 = "0",                                        // Gear box ratio
        .str19 = "0",                                        // Start input source
        .str20 = "0",                                        // Reverse ipnut source
        .str21 = "0",                                        // Reverse rpm
        .str22 = "0",                                        // Reverse slope
        .str23 = "0",                                        // Reverse max current
        .str24 = "0",                                        // Reverse max torque
        .str25 = "0",                                        // Error idle time
        .str26 = "0",                                        // Bachlash 
        .str27 = "0",                                        // Proportional gain
        .str28 = "0",                                        // Integral gain
        .str29 = "0",                                        // Encoder
        // New
        .str30 = "0",     // Min Torque
        .str31 = "0",     // Min RPM
        .str32 = "0",     // Reverse Min RPM
        .str33 = "0",     // DmsSWVer
        .str34 = "0",     // DmsCoreID
        .str35 = "0",     // DmsSerNr
        .str36 = "0",     // Led
        .str37 = "0",     // Lever Sensitivity
        .str38 = "0",     // Push Sensitivity
        .str39 = "0",     // MotSWVer 
        .str40 = "\n\r",  // CL,RF        
    };// CMD340, 設定Initial.
    #pragma endregion
    #pragma region ANS
    AmsANS301Struct ANS301Struct = {
        .str1 = "ANS301",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Job ID
        .str6 = "\n\r",                                     // CR,LF
    };// ANS301, 回復條碼切換工序.
    AmsANS302Struct ANS302Struct = {
        .str1 = "ANS302",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Tool control 
        .str6 = "\n\r",                                     // CR,LF
    };// ANS302, 回復工具控制命令. 
    AmsANS310Struct ANS310Struct = {
        .str1 = "ANS310",                                  // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                       // yyyyMMdd HH:mm:ss
        .str3 = "0",                                       // check sum ,4 chars
        .str4 = "0",                                       // Command_sn
        .str5 = "0",                                       // Direction
        .str6 = "0",                                       // RPM
        .str7 = "0",                                       // Target Torque
        .str8 = "0",                                       // Torque threshold
        .str9 = "0",                                       // Joint offset
        .str10 = "0",                                       // Monitoring mode
        .str11 = "0",                                       // Torque window
        .str12 = "0",                                       // High torque
        .str13 = "0",                                       // Low torque
        .str14 = "0",                                       // High angle
        .str15 = "0",                                       // Low angle
        .str16 = "\n\r",                                     // CR,LF
    };// ANS310, 回復Normal Torque.
    AmsANS311Struct ANS311Struct = {
        .str1  = "ANS311",                                  // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                       // yyyyMMdd HH:mm:ss
        .str3  = "0",                                       // check sum ,4 chars
        .str4  = "0",                                       // Command_sn
        .str5  = "0",                                       // Direction 
        .str6  = "0",                                       // RPM
        .str7  = "0",                                       // Target angle
        .str8  = "0",                                       // Angle threshold
        .str9  = "0",                                       // Monitoring mode
        .str10 = "0",                                       // Angle window
        .str11 = "0",                                       // High torque
        .str12 = "0",                                       // Low torque
        .str13 = "0",                                       // High angle
        .str14 = "0",                                       // Low angle
        .str15 = "\n\r",                                     // CR,LF
    };// ANS311, 回復Normal Angle.
    AmsANS312Struct ANS312Struct = {
        .str1 = "ANS312",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // RPM 
        .str6 = "0",                                        // Force
        .str7 = "0",                                        // Direction
        .str8 = "\n\r",                                     // CR,LF
    };// ANS312, 回復Normal Reverse.
    AmsANS320Struct ANS320Struct = {
        .str1  = "ANS320",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Target torque
        .str11 = "0",                                        // Torque threshold
        .str12 = "0",                                        // Joint offset
        .str13 = "0",                                        // Monitoring mode
        .str14 = "0",                                        // Torque window
        .str15 = "0",                                        // High torque
        .str16 = "0",                                        // Low torque
        .str17 = "0",                                        // High angle
        .str18 = "0",                                        // Low angle
        .str19 = "\n\r",                                     // CR,LF
    };// ANS320, 回復Advanced Torque.
    AmsANS321Struct ANS321Struct = {
        .str1  = "ANS321",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Target torque
        .str11 = "0",                                        // Angle threshold
        .str12 = "0",                                        // Monitoring mode
        .str13 = "0",                                        // Angle window
        .str14 = "0",                                        // High torque
        .str15 = "0",                                        // Low torque
        .str16 = "0",                                        // High angle
        .str17 = "0",                                        // Low angle
        .str18 = "\n\r",                                     // CR,LF
    };// ANS321, 回復Advanced Angle.
    AmsANS322Struct ANS322Struct = {
        .str1  = "ANS322",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Revolutions
        .str11 = "\n\r",                                     // CR,LF
    };// ANS322, 回復Advanced Revolutions.
    AmsANS323Struct ANS323Struct = {
        .str1  = "ANS323",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Step id 
        .str6  = "0",                                        // Step name
        .str7  = "0",                                        // Direction
        .str8  = "0",                                        // RPM
        .str9  = "0",                                        // Delay time
        .str10 = "0",                                        // Time
        .str11 = "\n\r",                                     // CR,LF
    };// ANS323, 回復Advanced Time.
    AmsANS330Struct ANS330Struct = {
        .str1  = "ANS330",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Program id 
        .str6  = "0",                                        // Program name
        .str7  = "0",                                        // Step1
        .str8  = "0",                                        // Step2
        .str9  = "0",                                        // Step3
        .str10 = "0",                                        // Step4
        .str11 = "0",                                        // Step5
        .str12 = "0",                                        // Step6 
        .str13 = "0",                                        // Step7
        .str14 = "0",                                        // Step8
        .str15 = "0",                                        // Ok time
        .str16 = "0",                                        // NG stop
        .str17 = "\n\r",                                     // CR,LF
    };// ANS330, 回復Program.
    AmsANS340Struct ANS340Struct = {
        .str1 = "ANS340",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // check sum ,4 chars        
        .str4 = "0",                                        // Command_sn
        .str5 = "0",                                        // Min temperature 
        .str6 = "0",                                        // Max temperature
        .str7 = "0",                                        // Max current
        .str8 = "0",                                        // Max peak current
        .str9 = "0",                                        // torque sensor type
        .str10 = "0",                                        // Max duty cycle
        .str11 = "0",                                        // Max torque
        .str12 = "0",                                        // PWM frequency 
        .str13 = "0",                                        // Max rpm
        .str14 = "0",                                        // Max slope
        .str15 = "0",                                        // Min bus voltage
        .str16 = "0",                                        // Max bus voltage
        .str17 = "0",                                        // Start duty cycle
        .str18 = "0",                                        // Gear box ratio
        .str19 = "0",                                        // Start input source
        .str20 = "0",                                        // Reverse ipnut source
        .str21 = "0",                                        // Reverse rpm
        .str22 = "0",                                        // Reverse slope
        .str23 = "0",                                        // Reverse max current
        .str24 = "0",                                        // Reverse max torque
        .str25 = "0",                                        // Error idle time
        .str26 = "0",                                        // Bachlash 
        .str27 = "0",                                        // Proportional gain
        .str28 = "0",                                        // Integral gain
        .str29 = "0",                                        // Encoder
        // New
        .str30 = "0",     // Min Torque
        .str31 = "0",     // Min RPM
        .str32 = "0",     // Reverse Min RPM
        .str33 = "0",     // DmsSWVer
        .str34 = "0",     // DmsCoreID
        .str35 = "0",     // DmsSerNr
        .str36 = "0",     // Led
        .str37 = "0",     // Lever Sensitivity
        .str38 = "0",     // Push Sensitivity
        .str39 = "0",     // MotSWVer 
        .str40 = "\n\r",  // CL,RF  
    };// ANS340, 回復Initial.
	#pragma endregion
    #pragma region REQ
    AmsREQ300Struct REQ300Struct = {
        .str1 = "REQ300",                                    // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                         // yyyyMMdd HH:mm:ss
        .str3 = "0",                                         // check sum ,4 chars
        .str4 = "0",                                         // Command_sn 
        .str5 = "\n\r",                                     // CR,LF
    };// REQ300, 保持連線.
    AmsREQ301Struct REQ301Struct = {
        .str1  = "REQ301",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars        
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Min temperature 
        .str6  = "0",                                        // Max temperature
        .str7  = "0",                                        // Max current
        .str8  = "0",                                        // Max peak current
        .str9  = "0",                                        // torque sensor type
        .str10 = "0",                                        // Max duty cycle
        .str11 = "0",                                        // Max torque
        .str12 = "0",                                        // PWM frequency 
        .str13 = "0",                                        // Max rpm
        .str14 = "0",                                        // Max slope
        .str15 = "0",                                        // Min bus voltage
        .str16 = "0",                                        // Max bus voltage
        .str17 = "0",                                        // Start duty cycle
        .str18 = "0",                                        // Gear box ratio
        .str19 = "0",                                        // Start input source
        .str20 = "0",                                        // Reverse ipnut source
        .str21 = "0",                                        // Reverse rpm
        .str22 = "0",                                        // Reverse slope
        .str23 = "0",                                        // Reverse max current
        .str24 = "0",                                        // Reverse max torque
        .str25 = "0",                                        // Error idle time
        .str26 = "0",                                        // Bachlash 
        .str27 = "0",                                        // Proportional gain
        .str28 = "0",                                        // Integral gain
        .str29 = "0",                                        // Encoder
        // New
        .str30 = "0",                                        // Min Torque
        .str31 = "0",                                        // Min RPM
        .str32 = "0",                                        // Reverse Min RPM
        .str33 = "0",                                        // DmsSWVer
        .str34 = "0",                                        // DmsCoreID
        .str35 = "0",                                        // DmsSerNr
        .str36 = "0",                                        // Led
        .str37 = "0",                                        // Lever Sensitivity
        .str38 = "0",                                        // Push Sensitivity
        .str39 = "0",                                        // MotSWVer 
        .str40 = "\n\r",                                     // CR,LF
    };// REQ301, 開機啟動MCB狀態.
    AmsREQ302Struct REQ302Struct = {
        .str1  = "REQ302",                                   // Header+DATA
        .str2  = "yyyyMMdd HH:mm:ss",                        // yyyyMMdd HH:mm:ss
        .str3  = "0",                                        // check sum ,4 chars        
        .str4  = "0",                                        // Command_sn
        .str5  = "0",                                        // Target Torque 
        .str6  = "0",                                        // Max Current
        .str7  = "0",                                        // Angle Selflearning
        .str8  = "0",                                        // SI Angle Monitoring
        .str9  = "0",                                        // SI Angle Positioning
        .str10 = "0",                                        // Sl Wait Start
        .str11 = "0",                                        // SI Turn Back
        .str12 = "\n\r",                                     // CR,LF
    };// REQ302, 自動學習連線
    #pragma endregion
    #pragma region DATA
    AmsDATA300Struct DATA300Struct = {
        .header        = "DATA300",                                  // str1:Header+DATA
        .datetime      = "yyyyMMdd HH:mm:ss",                        // str2:yyyyMMdd HH:mm:ss
        .checksum      = "0",                                        // str3:check sum ,4 chars
        .cmdsn         = "0",                                        // str4:Command_sn
        .dervicetype   = "0",                                        // str5:Device type
        .toolsn        = "0",                                        // str6:Tool SN
        .dervicesn     = "0",                                        // str7:Device SN
        .jobid         = "0",                                        // str8:Job ID
        .seqid         = "0",                                        // str9:Sequence ID
        .progid        = "0",                                        // str10:Program ID
        .stepid        = "0",                                        // str11:Step ID
        .dircetion     = "0",                                        // str12:Direction
        .torqueuint    = "0",                                        // str13:Torque unit
        .inc_dec       = "0",                                        // str14:INC/DEC
        .last_screwcnt = "0",                                        // str15:Last_screw_count
        .max_screwcnd  = "0",                                        // str16:Max_screw_count
        .fasteningtime = "0",                                        // str17:Fastening time
        .acttorque     = "0",                                        // str18:Torque
        .actangle      = "0",                                        // str19:Angle
        .maxtorque     = "0",                                        // str20:Max Torque
        .revolutions   = "0",                                        // str21:Revolutions
        .status        = "0",                                        // str22:Status
        .inputio       = "0",                                        // str23:Inputio
        .outputio      = "0",                                        // str24:Outputio
        .errmsg        = "0",                                        // str25:Error Masseage
        .toolcnt       = "0",                                        // str26:Tool Count
        .actrpm        = "0",                                        // str27:RPM
        .toolstatus    = "0",                                        // str28:Tool Status
        .clrf          = "\n\r",                                     // CR,LF
    };// DATA300, 鎖附資料
    AmsDATA302Struct DATA302Struct = {
        .str1 = "DATA302",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                         // yyyyMMdd HH:mm:ss
        .str3 = "0",                                         // check sum ,4 chars        
        .str4 = "0",                                         // Command_sn
        .str5 = "0",                                         // status
        .str6 = "\n\r",                                      // CR,LF
    };// DATA300, 鎖附資料
    #pragma endregion
    
    AMSBulletin(/* args */){};
    ~AMSBulletin(){};
};
#pragma endregion
#pragma region Gtcs Parameter.
// Class strcut.
class MCBBulletin
{
private:
    /* data */
public:
    #pragma region Bulletin(GTCSMCB)
    // Identification Parameter.
    McbID1Struct IdentificationPara = {
        .u8MotSWVer  = "-",    // SID = 1,Software Version Motorcontrolle.
        .u8MotCoreID = "-",    // SID = 2,Unique Core ID from Motor- STM32 Conrtroller.
        .u8MotSerNr  = "-",    // SID = 3,Serial Number of the Motor Controller.
        .u8DmsSWVer  = "-",    // SID = 4,Software Version DMS- Controller.
        .u8DmsCorel  = "-",    // SID = 5,Unique Core ID from DMS- STM32.
        .u8DmsSerNr  = "-",    // SID = 6,Serial Number of the DMS- Controller.
    };    
    // Basic Parameter.
    McbID2Struct BasicPara = {
        .s16MinTemp           = 0, // SID = 1,Minimal Temperature of the motor and the motorcontroller. 
                                   // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
        .s16MaxTemp           = 0, // SID = 2,Maximal Temperature of the motor and the motorcontroller. 
                                   // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
        .u16MaxCurrent        = 0, // SID = 3,This is the absolute maximum motor current value. 
                                   // There are also maximum current values for the specific process steps 
                                   // (see….), but this parameter is the limit. Unit is [mA].
        .u16MaxPeakCurrent    = 0, // SID = 4,This value controls the threshold of the peak current comparator.
                                   // The peak current detection is built in for the protection of the device 
                                   // when a latch up situation occurs. Unit is [A].
        .u16TorqueSensorType  = 0, // SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
        .u16MaxDutyCycle      = 0, // SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]
        .u16MaxTorque         = 0, // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
        .u16PWMFreq           = 0, // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
        .u16MaxRPM            = 0, // SID = 9,This is the absolute maximum motor rpm value. 
                                   // There are also rpm values for the specific process steps (see….) , 
                                   // but this parameter is the limit. Unit is [rpm].
        .u16MaxSlope          = 0, // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                   // There are also rpm slope values for the specific process steps (see….) , 
                                   // but this parameter is the limit. Unit is [rpm/s].
        .u16MinBusVolt        = 0, // SID = 11,Minimal Bus Voltage of the Power Supply. 
                                   // Underneath this Voltage the tool doesn’t work. 
                                   // Unit is [0,1V] (600 = 60V).
        .u16MaxBusVolt        = 0, // SID = 12,Maximal Bus Voltage of the Power Supply. 
                                   // Above this Voltage the tool doesn’t work. Unit is [0,1V].
        .u16StartDutyCycle    = 0, // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].
                                   // (20 = 10%)
        .u16GearBoxRatio      = 0, // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
        .u32StartInp          = 0, // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
        .u32RevInp            = 0, // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
        .u16RevRpm            = 0, // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
        .u16RevSlope          = 0, // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
        .u16RevMaxCurrent     = 0, // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."
        .u16RevMaxTorque      = 0, // SID = 20,"Maximum Torque of reverse drive. 
                                   // Value is 0- 1862 (max TMD Raw Value)"
        .u16ErrorIdleTime     = 0, // SID = 21,"Idle time of the motor controller after a Error condition.
                                   // Unit is [ms]."
        .u16BackLash          = 0, // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or 
                                   // Hallsensors are used for angle Positioning. Unit is [Increments]."
        .u16PGain             = 0, // SID = 23,Proportional Gain for the RPM Regulator.
        .u16IGain             = 0, // SID = 24,Integral Gain for the RPM Regulator.
        .u16Encoder           = 0, // SID = 25,"0 = No Encoder (positioning with Hallsensors). 
                                   // 1 = 200p Encoder"
    };
    //  Step Parameter.
    McbID3Struct StepPara = {
        // Step name.
        .u8StepID            = 0,  // 3XXX
        .u8StepName          = "-",// SID = 1,32 byte long string which contains the name of the step.
        // property.
        .u16StepRpm          = 0,  // SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
        .u16StepSlope        = 0,  // SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
        .u16StepMaxCurrent   = 0,  // SID = 4,Maximum current of this step. Unit is [mA].
        .u16StepMaxTorque    = 0,  // SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
        .u16StepMaxRevol     = 0,  // SID = 6,Maximum Revolutions (after the Gearbox) of this step. 
                                   // Unit is [0,01] (1000 = 10,00 Revolutions)
        .u16StepTime         = 0,  // SID = 7,Execution Time- time. Unit is [ms].
        .s32StepAngle        = 0,  // SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
        .u16StepAngleWindow  = 0,  // SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
        .u16StepTorqueWindow = 0,  // SID = 10,Window of the torque monitoring. 
                                   // Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
        .u16MinDutyCycle     = 0,  // SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)
        .u16StepFlags        = 0,  // SID = 12,See description of step flags.

        // New 
        .u16WindowMode  = 0,       // SID = 13.
        .u16AngleWindow2  = 0,     // SID = 14.
        .u16TorqueWindow2 = 0,     // SID = 15.
        // .u16StepTorqueLowerLimit = 0,
        // .u16StepMonitorType      = 0,

        // KLS Define
        // .u16StepDelaytime        = 0,
        // "u16ProcStepId":self.u16ProcStepId,   
    };   
    //  Process Parameter.
    McbID4Struct ProcessPara = {
        // Process ID
        .u8ProcID           = 0, // Process ID
        // Process name.
        .u8ProcName         = "-", // SID = 1,"32 byte long string which contains the name of the 
                                   // screwing process."
        // Process parameter.
        .u16ProcPGain       = 0, // SID = 2,Proportional Gain of the screwing process.          
        .u16ProcIGain       = 0, // SID = 3,Integral Gain of the screwing process.    
        .u16ProcMaxTime     = 0, // SID = 4,"0 = No maximum process time.
                                 // Every other value in [ms] is the timemonitoring of this process.
                                 // It leads to an error, when the process exceeds this time."
        .u16ProcMinTime     = 0, // SID = 5,"0 = No minimum process time.
                                 // Every other value in [ms] is the time monitoring of this process. 
                                 // It leads to an error, when the process undershoots this time."
        .u16ProcRevFunc     = 0, // SID = 6,"0 = Screwing direction of the whole process normal 
                                   // (tightening right) 1 = Screwing direction of the whole process 
                                   // reverse (tightening left)"
        .u16NbrSteps        = 0, // SID = 7,Number of Steps in this Process

        // Step list.
        .step_id_1          = 0,
        .step_id_2          = 0,
        .step_id_3          = 0,
        .step_id_4          = 0,
        .step_id_5          = 0,
        .step_id_6          = 0,
        .step_id_7          = 0,
        .step_id_8          = 0,
        .step_id_9          = 0,
        .step_id_10         = 0,
        .step_id_11         = 0,
        .step_id_12         = 0,
    };
    // Program Parameter.
    McbID5Struct ProgPara = {
        .u8ProgName         = "-", // SID = 1,32 byte long string which contains the name of the Program.
        .u16ProgResFunc     = 0, // SID = 2,"0 = No Function 
                                 // 1 = a change of the reset flag resets the actual process.
                                 // 2 = a change of the reset flag resets the actual program".  
        .u16ProgRevFunc     = 0, // SID = 3,"0 = No Reverse Drive
                                 // 1 = Reverse Drive is possible
                                 // 2 = Reverse Drive is possible and resets the last process".
        .u16ProgRevTime     = 0, // SID = 4,Screwing Program Reverse Drive Time.
        .u16ProgErrResp     = 0, // SID = 5,"0 = Wait for acknowledge, 1 = No waiting"
        .u16NbrProc         = 0, // SID = 6,Number of processes in this program.
        // SID = 1000-1199,Screwing Process ID Number 4000 - 4249.
        // "u16ProgProcessId"  : self.u16ProgProcessId 
    };
    // Statistc Parameter.
    McbID6Struct StatisicPara = {
        .u32StartCount      = 0, //SID = 1,Counter for Process Start.
        .u32TighteningCount = 0, // SID = 2,Counter for Process Tightening.
        .u32IMaxCount       = 0, // SID = 3,Counter for Overcurrent Conditions.
        .u32tMaxCount       = 0, // SID = 4,"Counter for Overtemperature Conditions".
        .u32RevCount        = 0, // SID = 5,Counter for Reverse Operations.
        // SID = 6,Last 10 Screwdriver Errors (see Errorlist….). 
        // "u32RevCountList":self.u32RevCountList 
    };
    // Ctrl Parameter.
    McbID7Struct CtrlPara = {
        .u16ControlMode      = 0, // SID = 1,"0 = Manual Mode,1 = Process Mode,2 = Program Mode,
                                    // 3= PWM Mode,4=Para Write Mode"
        .u16WorkProc         = 0, // SID = 2,It is the ID of the actual working process. 
                                    // This is relevant when Control Mode = Process Mode.
        .u16WritePara2Flash  = 0, // SID = 3,Reserved because the Parameters are stored in FRAM.
        .u16WriteStep2Flash  = 0, // SID = 4,"When receiving this Parameter the selected Step will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Step (ID3XXX)".
        .u16WriteProc2Flash  = 0, // SID = 5,"When receiving this Parameter the selected Process will be 
                                    // written to Flash Memory.
                                    // Value of this Parameter the ID of the Process (ID4XXX)".
        .u16WriteProg2Flash  = 0, // SID = 6,"When receiving this Parameter the Program will be written 
                                    // to Flash Memory.Value of this Parameter is “Don't care”".
    };
    // Manual Parameter.
    McbID8Struct ManualPara = {
        .u16ManRpm        = 0,   // SID = 1, Manual Rpm. Unit is [rpm] (after the Gearbox).
        .u16ManSlope      = 0,   // SID = 2, Manual slope. Unit is [rpm/s] (after the Gearbox).
        .u16ManMaxCurrent = 0,   // SID = 3, Manual maximum current. Unit is [mA].
        .u16ManMaxTorque  = 0,   // SID = 4, Maximum Torque Value is 0- 1862 (max Raw TMD Value).   
    };
    // Status Parameter.
    McbID9Struct StatusPara = {
        .su16ActError    = 0, // SID = 1, Actual ErrorFlags of the Screwdriver.
        .u16ActProcID    = 0, // SID = 2, Actual working Process ID. (4000- 4199).
        .u16ActProcNr    = 0, // SID = 3, Actual working Process Number. (1- 200).
        .u16ActStepID    = 0, // SID = 4, Actual working Step ID. (3000 - 3499).
        .u16ActStepNr    = 0, // SID = 5, Actual working Step Nr. (1- 20).
        .u16ActCurr      = 0, // SID = 6, Actual motor current [mA].
        .u16ActRPM       = 0, // SID = 7, Actual motor rpm [rpm].
        .u16ActTorque    = 0, // SID = 8, Actual Torque Value is 0- 1862 (max Raw TMD Value).
        .u16ActBusVolt   = 0, // SID = 9, Actual Bus Voltage [0,01V] (6000 = 60V).
        .u16ActContTemp  = 0, // SID = 10, Actual motorcontroller temperature [0,1°C].
        .u16ActMotTem    = 0, // SID = 11, Actual motor temperature [0,1°C].
        .u16MaxCurrent   = 0, // SID = 12, Actual maximum current of the process [mA].
        .u16MaxTorqu     = 0, // SID = 13, Maximum Torque Value is 0- 1862 (max Raw TMD Value).
        .u32Angle        = 0, // SID = 14, Actual angle [0,1°].
        .u32Revolutions  = 0, // SID = 15, Actual revolutions [0,01 revolutions].
    };
    #pragma endregion  
};
#pragma endregion
#pragma region Gtcs database
class DatabasteBulletin{
private:
public:
    GtcsDatabaseBasicStruct basic = {
        .mintemp           = "0",  // Min temperature       (REAL)
        .maxtemp           = "0",  // Max temperature       (REAL)
        .maxcurrent        = "0",  // Max current           (REAL)
        .maxpeakcurrent    = "0",  // Max peak current      (INTEGER)
        .torquesensortype  = "0",  // torque sensor type    (INTEGER)
        .maxdutycycle      = "0",  // Max duty cycle        (REAL)
        .maxtorque         = "0",  // Max torque            (REAL)
        .pwmfreq           = "0",  // PWM frequency         (INTEGER)
        .maxrpm            = "0",  // Max rpm               (INTEGER)
        .maxslope          = "0",  // Max slope             (INTEGER)
        .minbusvolt        = "0",  // Min bus voltage       (REAL)
        .maxbusvolt        = "0",  // Max bus voltage       (REAL)
        .startdutycycle    = "0",  // Start duty cycle      (REAL)
        .gearboxratio      = "0",  // Gear box ratio        (REAL)
        .startinp          = "0",  // Start input source    (INTEGER)
        .revinp            = "0",  // Reverse ipnut source  (INTEGER)
        .revrpm            = "0",  // Reverse rpm           (INTEGER)
        .revslope          = "0",  // Reverse slope         (INTEGER)
        .revmaxcurrent     = "0",  // Reverse max current   (INTEGER)
        .revmaxtorque      = "0",  // Reverse max torque    (REAL)
        .erroridletime     = "0",  // Error idle time       (INTEGER)
        .backlash          = "0",  // Bachlash              (INTEGER)
        .pgain             = "0",  // Proportional gain     (INTEGER)
        .igain             = "0",  // Integral gain         (INTEGER)
        .encoder           = "0",  // Encoder               (INTEGER)
        // New.
        .mintorque         = "0",  // (REAL)
        .minrpm            = "0",  // (INTEGER)
        .revminrpm         = "0",  // (INTEGER)
        .dmsswver          = "0",  // (INTEGER)
        .dmscoreid         = "0",  // (INTEGER)
        .dmssernr          = "0",  // (INTEGER)
        .led               = "0",  // (INTEGER)
        .lever_sensitivity = "0",  // (INTEGER)
        .push_sensitivity  = "0",  // (INTEGER) 
        .motswver          = "0",  // (TEXT)
        .end               = "\n\r",   // end
    };
    // DatabasteBulletin(){};
    // ~DatabasteBulletin(){};
};
#pragma endregion
#pragma region bulletin
//GTCS Bulletin.
class GtcsBulletin
{
private:
    // SignleTon instance object.
    static GtcsBulletin* instance;
    GtcsBulletin(/* args */);
public:
    // Constructor.
    ~GtcsBulletin();
    // Get Instance.
    static GtcsBulletin* GetInstance();
    AMSBulletin AmsBulletin;
    MCBBulletin McbBulletin;
    DatabasteBulletin  DbBulletin;
    bool checksysok = true;  
    bool uisetting = false;  
    std::string uisockrevcmd = ""; 
};
#pragma endregion
#pragma endregion
#endif