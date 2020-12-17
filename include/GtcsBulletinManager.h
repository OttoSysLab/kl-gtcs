#ifndef _GTCS_BULLETIN_MANAGER_
#define _GTCS_BULLETIN_MANAGER_
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
#include "GtcsTypeDefine.h"
#include "GtcsAmsProtocol.h"
#include "GtcsMcbCommunication.h"
#include "Common.h"
#include <ctime>

#pragma endregion
// GTCS Bulletin.
class GtcsBulletinManager
{
private:
    int MainFSM = MAIN_FSM::READY;
    std::string GetMcbRtStatusString(MCB_RT_STATUS status);
    double ConvertToAmsTorque();
public:
    // Constructor.
    GtcsBulletinManager(/* args */);
    // Distructor.
    ~GtcsBulletinManager();
    // Get & Set FSM.
    int GetMainFSM();
    int SetMainFSM(int main_fsm);
    int ConvertActuralData300(GtcsStatusTelegramStrcut* ptr);
};
#endif