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
#include "gtcsbulletin.h"
#include "gtcsgloabdefine.h"
#include "gtcsamsprotocol.h"
#include "gtcsmcbcomm.h"
#include "gtcsdatabase.h"
#include "gtcstcpsocket.h"

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
};

// NTCS manager.
class NtcsManager : public Manager
{
private:
    /* data */
public:
    NtcsManager(/* args */);
    ~NtcsManager();
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
    // std::string GetMcbRtStatusString(MCB_RT_STATUS status);
    std::string GetRtLockedStatusMessage(int lcstatusnum);                                        // Define Locksed status string. 

    std::string GetToolRunTimeStatus(GtcsScrewSequenceHandler &screwstatus);
    bool CheckUiSettingFSM(int uicmd);

    std::string comport_name = "";
    std::string db_emmc_Path = "";     // Initial database path.
    std::string db_ramdisk_Path = "";  // Initial database path.
    
    // Thread
    std::thread thread_tcpserver;

    bool CopyDatabase(std::string destination ,std::string source);
    bool UpdateMcbBasicParaToDB(GtcsDatabase &db,GtcsDatabaseBasicInfo &db_basic,McbID2Struct &mcb_basic);
    bool CompareBasicStruct(GtcsDatabaseBasicInfo &emmc,GtcsDatabaseBasicInfo &ramdisk);
    bool SetSystemBasicParameter(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic);

    // Switch Job sequence.
    bool GetDatabaseUnscrewData(GtcsCtrlTelegramStrcut &telegram,int jobid);
    bool GetDatabaseScrewSequenceListData(std::vector<GtcsSequenceDataStruct> &seqlist,int jobid);
    bool GetDatabaseScrewStepListData(std::vector<GtcsStepDataStruct> &steplist,int jobid,int seqid);

    // AMS Protocol.
    bool SetDatabaseBasicParaToAns(AmsANS340Struct &amsans,GtcsDatabaseBasicInfo &db_basic);   // DB_BASIC  ->AMS_ANS340
    bool SetDatabaseBasicParaToReq(AmsREQ301Struct &amsreq,GtcsDatabaseBasicInfo &db_basic);   // DB_BASIC  ->AMS_REQ301
    bool SetAmsCmdBaiscParaToAns(AmsANS340Struct &amsans,AmsCMD340Struct &amscmd);             // AMS_CMD340->AMS_ANS340
    bool ConvertReadlTimeActuralValue();                                                       // GTCS AMS DATA300
    bool ConvertAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &basic_para);         // AMC_CMD340->DB_Struct
public:
    // Constructor.
    GtcsManager(/* args */);
    // Distructor.
    ~GtcsManager();

    // Intial set parameter.
    void SetMcbPortName(std::string comname);
    void SetEmmcDatabasePath(std::string Path);
    void SetRamdiskDatabasePath(std::string Path);

    // Tcp Socke server.
    std::string GetGtcsTcpSocketServerIP();
    bool SetGtcsTcpSocketServerInfo(std::string ip,int port);
    int GetGtcsTcpSocketServerPort();
    bool CheckUiRequestCmd(std::string reqest_string);
    std::string GetUiRequestCmd();
    std::string GetUiResponseCmd(std::string uicmd_string);
    bool GetUiSettingStatus();
    void SetUiSettingStatus(bool status);

    // Jion thread.
    bool StopAllThread();
    std::string GetMCBErrMessageString(uint32_t errorflagindex);
    std::string GetCurrentMCBErrorMessage(uint32_t errorflags);

    // Gtcs System Main state.(MAIN)FSM)
    bool InitialGtcsSystem();
    bool CheckGtcsSystem();
    bool RunGtcsSystem();
    bool ClearGtcsSystemAlarm();
    bool SettingGtcsSystem();
    
};
#pragma endregion
