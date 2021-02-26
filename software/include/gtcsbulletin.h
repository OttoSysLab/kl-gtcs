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
#include "gtcsgloabdefine.h"
// #include <list>

#pragma region bulletin
#pragma region Gtcs AMS bulletin
class AMSBulletin
{
private:
public:
    #pragma region CMD
    AmsCMD300Struct CMD300Struct;// CMD300, 回復確認.
    AmsCMD301Struct CMD301Struct;// CMD301, 條碼切換工序.
    AmsCMD302Struct CMD302Struct;// CMD302, 工具控制命令.
    AmsCMD303Struct CMD303Struct;// CMD303, 跳工序.
    AmsCMD310Struct CMD310Struct;// CMD310, 設定Normal Torque.
    AmsCMD311Struct CMD311Struct;// CMD311, 設定Normal Angle.
    AmsCMD312Struct CMD312Struct;// CMD312, 設定Normal Reverse.
    AmsCMD320Struct CMD320Struct;// CMD320, 設定Advanced Torque.
    AmsCMD321Struct CMD321Struct;// CMD321, 設定Advanced Angle.
    AmsCMD322Struct CMD322Struct;// CMD322, 設定Advanced Revolutions.
    AmsCMD323Struct CMD323Struct;// CMD323, 設定Advanced Time.
    AmsCMD330Struct CMD330Struct;// CMD330, 設定Program.
    AmsCMD340Struct CMD340Struct;// CMD340, 設定Initial.
    #pragma endregion
    #pragma region ANS
    AmsANS301Struct ANS301Struct;// ANS301, 回復條碼切換工序.
    AmsANS302Struct ANS302Struct;// ANS302, 回復工具控制命令.
    AmsANS310Struct ANS310Struct;// ANS310, 回復Normal Torque.
    AmsANS311Struct ANS311Struct;// ANS311, 回復Normal Angle.
    AmsANS312Struct ANS312Struct;// ANS312, 回復Normal Reverse.
    AmsANS320Struct ANS320Struct;// ANS320, 回復Advanced Torque.
    AmsANS321Struct ANS321Struct;// ANS321, 回復Advanced Angle.
    AmsANS322Struct ANS322Struct;// ANS322, 回復Advanced Revolutions.
    AmsANS323Struct ANS323Struct;// ANS323, 回復Advanced Time.
    AmsANS330Struct ANS330Struct;// ANS330, 回復Program.
    AmsANS340Struct ANS340Struct;// ANS340, 回復Initial.
	#pragma endregion
    #pragma region REQ
    AmsREQ300Struct REQ300Struct;// REQ300, 保持連線.
    AmsREQ301Struct REQ301Struct;// REQ301, 開機啟動MCB狀態.
    AmsREQ302Struct REQ302Struct;// REQ302, 自動學習連線
    #pragma endregion
    #pragma region DATA
    AmsDATA300Struct DATA300Struct;// DATA300, 鎖附資料
    AmsDATA302Struct DATA302Struct;// DATA300, 鎖附資料
    #pragma endregion

    AMSBulletin(/* args */);
    ~AMSBulletin();
    void InitialStructValue();
};
#pragma endregion
#pragma region Gtcs Parameter.
// Class strcut.
class MCBBulletin
{
private:
public:
    #pragma region Bulletin(GTCSMCB)
    // Identification Parameter.
    McbID1Struct IdentificationPara;
    // Basic Parameter.
    McbID2Struct BasicPara;
    //  Step Parameter.
    McbID3Struct StepPara;
    //  Process Parameter.
    McbID4Struct ProcessPara;
    // Program Parameter.
    McbID5Struct ProgPara;
    // Statistc Parameter.
    McbID6Struct StatisicPara;
    // Ctrl Parameter.
    McbID7Struct CtrlPara;
    // Manual Parameter.
    McbID8Struct ManualPara;
    // Status Parameter.
    McbID9Struct StatusPara;
    #pragma endregion
    MCBBulletin();
    ~MCBBulletin();
    void InitialStructValue();
};
#pragma endregion

#pragma region 
class TcpSocketInfo
{
private:
    /* data */
    std::string IP="";
    int Port = 0;
public:
    TcpSocketInfo(/* args */){};
    ~TcpSocketInfo(){};
    // IP information.
    std::string GetIpAddress(){
        return IP;
    };
    void SetIpAddress(std::string ipaddr){
        IP = ipaddr;
    };
    // Port information.
    int GetPort(){return Port;};
    void SetPortNum(int port_num){Port = port_num;};
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
    GtcsScrewSequenceHandler ScrewHandler;
    bool checksysok = true;
    bool settingstatus = false;
    bool alarmstatus = false;
    
    std::string uisockrevcmd = "";
    TcpSocketInfo TcpServer;
    TcpSocketInfo TcpClient;
};
#pragma endregion
#pragma endregion
