#ifndef _GTCS_BULLETIN_MANAGER_
#define _GTCS_BULLETIN_MANAGER_
#define _DEBUG_MODE_
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
#include "Common.h"
#include <iostream>
#include <string.h>
#include <ctime>

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
    #pragma region 
    // Attribute.
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance(); 
    GtcsMcbComm *mcb       = GtcsMcbComm::GetInstance(); 
    GtcsAmsProtocol *ams = GtcsAmsProtocol::GetInstance();
    // method.
    std::string GetMcbRtStatusString(MCB_RT_STATUS status);
    std::string GetToolRunTimeStatus();
    int ConvertActuralData300();
    #pragma endregion

    #pragma region uisetting
    int CheckUiSettingFSM(int uicmd);
    #pragma endregion
    
    // Gtcs System Main state.
    int InitialGtcsSystem();
    int CheckGtcsSystem();
    int RunGtcsSystem();
    int ClearGtcsSystemAlarm();
    int SettingGtcsSystem();
public:
    // Constructor.
    GtcsManager(/* args */);
    // Distructor.
    ~GtcsManager();
    std::string CheckRequestStatus(std::string reqest_string);
    std::string CheckUiCmdRequest(std::string reqest_string);
    std::string GetUiCmdResponse(std::string uicmd_string);
    int CheckMainFSM(int main_fsm);
};
#pragma endregion
#endif