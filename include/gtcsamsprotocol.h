#ifndef _GTCS_AMS_PROTOCOL_
#define _GTCS_AMS_PROTOCOL_
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
#include <map>
#include <iostream>
#include <cstring>
#include "gtcstypedefine.h"

#pragma region AMS bulletin
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
        .str30 = "\n\r",                                     // CR,LF
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
    };// ANS330, 回復Program.
    AmsANS340Struct ANS340Struct = {
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
        .str30 = "\n\r",                                     // CR,LF
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
        .str30 = "\n\r",                                     // CR,LF
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
        .str1 = "DATA300",                                  // str1:Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                        // str2:yyyyMMdd HH:mm:ss
        .str3 = "0",                                        // str3:check sum ,4 chars
        .str4 = "0",                                        // str4:Command_sn
        .str5 = "6",                                        // str5:Device type
        .str6 = "0",                                        // str6:Tool SN
        .str7 = "0",                                        // str7:Device SN
        .str8 = "0",                                        // str8:Job ID
        .str9 = "0",                                        // str9:Sequence ID
        .str10 = "0",                                        // str10:Program ID
        .str11 = "0",                                        // str11:Step ID
        .str12 = "0",                                        // str12:Direction
        .str13 = "0",                                        // str13:Torque unit
        .str14 = "0",                                        // str14:INC/DEC
        .str15 = "0",                                        // str15:Last_screw_count
        .str16 = "0",                                        // str16:Max_screw_count
        .str17 = "0",                                        // str17:Fastening time
        .str18 = "0",                                        // str18:Torque
        .str19 = "0",                                        // str19:Angle
        .str20 = "0",                                        // str20:Max Torque
        .str21 = "0",                                        // str21:Revolutions
        .str22 = "0",                                        // str22:Status
        .str23 = "0",                                        // str23:Inputio
        .str24 = "0",                                        // str24:Outputio
        .str25 = "0",                                        // str25:Error Masseage
        .str26 = "0",                                        // str26:Tool Count
        .str27 = "0",                                        // str27:RPM
        .str28 = "\n\r",                                     // CR,LF
    };// DATA300, 鎖附資料
    AmsDATA302Struct DATA302Struct = {
        .str1 = "DATA302",                                   // Header+DATA
        .str2 = "yyyyMMdd HH:mm:ss",                         // yyyyMMdd HH:mm:ss
        .str3 = "0",                                         // check sum ,4 chars        
        .str4 = "0",                                         // Command_sn
        .str5 = "0",                                          // status
        .str6 = "\n\r",                                     // CR,LF
    };// DATA300, 鎖附資料
    #pragma endregion
    
    AMSBulletin(/* args */){};
    ~AMSBulletin(){};
};
#pragma endregion

#pragma region AMS Protocol object
// Gtcs AMS Protocol
class GtcsAMSProtocol
{
private: 
    // SignleTon instance object.
    static GtcsAMSProtocol* instance;
    // Constructor.
    GtcsAMSProtocol(/* args */);   
public:    
    int cmdsn = 0; 
    ~GtcsAMSProtocol();
    static GtcsAMSProtocol* getInstance();
    AMSBulletin amsBulletin;
    std::string getAMSBulletinData(AMSCMD amscmd);
};
#pragma endregion
#endif