#ifndef _GTCS_MANAGER_
#define _GTCS_MANAGER_
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
#include "GtcsBulletin.h"
#include "GtcsGloabDefine.h"
#include "GtcsAmsProtocol.h"
#include "GtcsMcbComm.h"
#include "GtcsDatabase.h"
// #include "Common.h"
// #include <iostream>
// #include <string.h>
// #include <ctime>

// #define SETTING_READY -1
// #define SETTING_OK 0
// #define SETTING_NG 1

#pragma region 
class Manager
{
private:
    /* data */
public:
    Manager(/* args */);
    ~Manager();
    // 
    int MainFSM = MAIN_FSM::INITIAL;                         // READY    
    // Get & Set MAIN FSM.
    int GetMainFSM();
    void SetMainFSM(int main_fsm);
    // virtual int CheckMainFSM(int main_fsm){}=0;
    // virtual int CheckMainFSM(int main_fsm){};
    int CheckMainFSM(int main_fsm);
};

// NTCS manager.
class NtcsManager : public Manager
{
private:
    /* data */
public:
    NtcsManager(/* args */);
    ~NtcsManager();
    // int CheckMainFSM(int main_fsm);
};

// GTCS manager.
class GtcsManager : public Manager
{
private:
    // Attribute.
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance(); 
    GtcsMcbComm *mcb       = GtcsMcbComm::GetInstance(); 
    GtcsAmsProtocol *ams = GtcsAmsProtocol::GetInstance();
    // method.
    std::string GetMcbRtStatusString(MCB_RT_STATUS status);
    std::string GetToolRunTimeStatus();
    bool CheckUiSettingFSM(int uicmd);    

    std::string comport_name = "";
    std::string db_emmc_Path = "";     // Initial database path.
    std::string db_ramdisk_Path = "";  // Initial database path.
    
    // Gtcs System Main state.
    bool InitialGtcsSystem();
    bool CheckGtcsSystem();
    bool RunGtcsSystem();
    bool ClearGtcsSystemAlarm();
    bool SettingGtcsSystem();
    // 
    bool CopyDatabase(std::string destination ,std::string source);
    bool UpdateMcbBasicParaToDB(GtcsDatabase &db,GtcsDatabaseBasicInfo &db_basic,McbID2Struct &mcb_basic); 
    bool CompareBasicStruct(GtcsDatabaseBasicInfo &emmc,GtcsDatabaseBasicInfo &ramdisk);
    bool SetSystemBasicParameter(AmsCMD340Struct &amscmd,GtcsDatabaseBasicStruct &db_basic,McbID2Struct &mcb_basic);
    
    // AMS Protocol.
    bool SetAmsBulletinBasicPara(GtcsDatabaseBasicInfo &basic);   // GTCS AMS REQ301 
    bool ConvertReadlTimeActuralValue();                          // GTCS AMS DATA300  
    bool ConvertAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &basic_para);//
    // bool ConvertAmsBasicToDBStruct(AmsCMD340Struct &amscmd,GtcsDatabaseBasicStruct &db_basic); 
public:
    // Constructor.
    GtcsManager(/* args */);
    // Distructor.
    ~GtcsManager();
    // 
    void SetMcbPortName(std::string comname);
    void SetEmmcDatabasePath(std::string Path);
    void SetRamdiskDatabasePath(std::string Path);
    
    std::string CheckRequestStatus(std::string reqest_string);
    std::string CheckUiCmdRequest(std::string reqest_string);
    std::string GetUiCmdResponse(std::string uicmd_string);

    int CheckMainFSM(int main_fsm);
};
#pragma endregion
#endif