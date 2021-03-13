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
#include "gtcsfilehandler.h"
#include "gtcsgpio.h"

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
    GtcsFileHandler txthandler;
    
    // method.
    // std::string GetMcbRtStatusString(MCB_RT_STATUS status);
    std::string GetRtLockedStatusMessage(int lcstatusnum);                                        // Define Locksed status string. 

    bool GetToolRunTimeStatus(GtcsScrewSequenceHandler &screwstatus,StatusTelegram &mcbstatus);
    
    bool GetStartSignalStatus(uint16_t &tmdflags);
    bool GetSystemErrorStatus(uint16_t &statusflags);
    bool CheckUiSettingFSM(int uicmd);

    std::string comport_name = "";
    std::string db_emmc_Path = "";          // Initial database path.
    std::string db_ramdisk_Path = "";       // Initial database path.

    std::string txt_ramdisk_Path = "";      // Initial txt path
    std::string db_screw_ramdisk_Path = ""; //Initial database path.
    
    // Thread.
    std::thread thread_tcpserver;
    std::thread thread_gpio;

    bool CopyDatabase(std::string destination ,std::string source);
    bool UpdateMcbBasicParaToDB(GtcsTcsDatabase &db,GtcsTcsDatabaseBasicInfo &db_basic,McbID2Struct &mcb_basic);
    bool CompareBasicStruct(GtcsTcsDatabaseBasicInfo &emmc,GtcsTcsDatabaseBasicInfo &ramdisk);
    bool SetSystemBasicParameter(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic);

    // Switch Job sequence.
    bool GetDatabaseUnscrewData(GtcsCtrlTelegramStrcut &telegram,int jobid);
    bool GetDatabaseScrewSequenceListData(std::vector<GtcsSequenceDataStruct> &seqlist,int jobid);
    bool GetDatabaseScrewStepListData(std::vector<GtcsStepDataStruct> &steplist,int jobid,int seqid);

    // MCB api.
    bool GetMcbProcessTelegramFromDBData(McbID4Struct &mcbprocess,McbID2Struct &mcbbasic,std::vector<GtcsStepDataStruct> &steplist);
    bool GetMcbStepTelegramFromDBData(McbID3Struct &mcbstep,McbID2Struct &mcbbasic,GtcsStepDataStruct &dbstep,int stepindex,bool endstepflag);
    
    // Configure Mcb enable & disable.
    bool EnableMcbScrewStatus();
    bool DisableMcbScrewStatus();

    // Configure Mcb Process parameter.
    bool GetMcbProcessParameter(McbID4Struct &mcbprocess);
    bool SetMcbProcessParameter(McbID4Struct &mcbprocess);

    // Configure Mcb Process step.
    bool GetMcbStepParameter(McbID3Struct &mcbstep);
    bool SetMcbStepParameter(McbID3Struct &mcbstep);

    // Configure Mcb StepFlags.
    bool GetMcbScrewStepFlags(McbID3Struct &mcbstep);
    bool SetMcbScrewStepFlags(McbID3Struct &mcbstep,GtcsStepDataStruct &dbstep,int stepindex,bool stopmotor);
    
    // Configure Mcb WindowModeFlags.
    bool GetMcbStepWindowModeFlags(McbID3Struct &mcbstep);
    bool SetMcbStepWindowModeFlags(McbID3Struct &mcbstep,GtcsStepDataStruct &dbstep);

    // Switch sequence function.
    bool ScrewDriverSwitchJobHandler(int jobid);
    bool ScrewDriverSwitchSequenceHandler(int jobid,int seqid);

    // Get tightening repeat counter
    bool CheckScrewDriverCountingFinished(GtcsScrewSequenceHandler &screwhandler);
    
    bool GetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler,int &tighteningcounter);
    bool SetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler);

    bool GetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler,int &tighteningcounter);
    bool SetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler);

    // Get and set screw driver next seqindex. 
    bool GetScrewDriverNextSeqindex(GtcsScrewSequenceHandler &screwhandler);
    bool SetScrewDriverNextSeqindex(GtcsScrewSequenceHandler &screwhandler);

    // AMS Protocol.
    bool SetDatabaseBasicParaToAns(AmsANS340Struct &amsans,GtcsTcsDatabaseBasicInfo &db_basic);   // DB_BASIC  ->AMS_ANS340
    bool SetDatabaseBasicParaToReq(AmsREQ301Struct &amsreq,GtcsTcsDatabaseBasicInfo &db_basic);   // DB_BASIC  ->AMS_REQ301
    void SetAmsCmdBaiscParaToAns(AmsANS340Struct &amsans,AmsCMD340Struct &amscmd);             // AMS_CMD340->AMS_ANS340
        
    // GTCS AMS DATA300
    bool GetRealTimeActuralValue(AmsDATA300Struct &data300,GtcsScrewSequenceHandler &screwhandler,GtcsStatusTelegramStrcut &mcbstatus);  
    // Write data300 to txt.
    bool ClearRamdiskTxtFile();
    bool WriteRealTimeActuralValueToRamdisk(AmsDATA300Struct &data300);    

    // SetAmsBasicToMcbStruct.
    bool SetAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &basic_para);       // AMC_CMD340->DB_Struct
    bool InsertRealTimeActuralValueToDatabase(AmsDATA300Struct &data300);                   // AMC_CMD300->DB_Struct

public:
    // Constructor.
    GtcsManager(/* args */);
    // Distructor.
    ~GtcsManager();

    // Intial set parameter.
    void SetMcbPortName(std::string comname);
    void SetEmmcDatabasePath(std::string Path);
    void SetRamdiskDatabasePath(std::string Path);
    void SetRamdiskTxtPath(std::string Path);     // Txt in ramdisk to swap run time screw data.
    // Creat screw data database in ramdisk.
    bool SetScrewStatusDatabase(std::string Path);
    bool CreatScrewStatusDatabase();

    // Tcp Socke server.
    std::string GetGtcsTcpSocketServerIP();
    bool StartGtcsTcpSocketServerThread(std::string ip,int port);    
    int GetGtcsTcpSocketServerPort();
    bool CheckUiRequestCmd(std::string reqest_string);
    std::string GetUiRequestCmd();
    std::string GetUiResponseCmd(std::string uicmd_string);
    bool GetUiSettingStatus();
    void SetUiSettingStatus(bool status);
    
    // GPIO.
    bool GetGtcsGPIOTConfigure();
    bool SetGtcsGPIOTConfigure();
    bool StartGtcsGPIOThread();
    bool StopGtcsGPIOThread();

    // Get Gtcs system datetimt & year.
    std::string GetCurrentDateYear();
    std::string GetCurrentDateime();

    // Get screw dreiver status.
    bool GetGtcsScrewSequenceHandlerStatus(GtcsScrewSequenceHandler &screwhandler);
    bool SetGtcsScrewSequenceHandlerStatus(GtcsScrewSequenceHandler &screwhandler);

    // Jion thread.
    bool StopAllThread();
    std::string GetMCBErrMessageString(uint32_t errorflagindex);
    std::string GetCurrentSystemStatusMessage(uint32_t &currentstatus,uint32_t errorflags);

    // Gtcs System Main state.(MAIN)FSM)
    bool InitialGtcsSystem();
    bool CheckGtcsSystem();
    bool RunGtcsSystem();
    bool ClearGtcsSystemAlarm();
    bool SettingGtcsSystem();    
};
#pragma endregion
