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
#include "../include/GtcsBulletinManager.h"

// Signalton.
// Constructor.
GtcsBulletinManager::GtcsBulletinManager(/* args */)
{}
// Distructor.
GtcsBulletinManager::~GtcsBulletinManager()
{}
// Get main FSM
int GtcsBulletinManager::GetMainFSM()
{
    return MainFSM;
}
int GtcsBulletinManager::SetMainFSM(int main_fsm)
{
    int result = -1;
    MainFSM = main_fsm;
    return result;
}
// Convert To Ams Torque.
double GtcsBulletinManager::ConvertToAmsTorque()
{
    double result  = 0;
    return result;
}
// Get MCB realy time status string.
std::string GtcsBulletinManager::GetMcbRtStatusString(MCB_RT_STATUS status)
{
    std::string result = "";
    switch(status)
    {
    case MCB_RT_STATUS::RUNNING:
        result = "Running";
        break;
    case MCB_RT_STATUS::REVERSE:
        result = "Reverse";
        break;
    case MCB_RT_STATUS::OK:
        result = "OK";
        break;
    case MCB_RT_STATUS::OK_SEQUENCE:
        result = "OK-Sequence";
        break;
    case MCB_RT_STATUS::OK_JOB:
        result = "OK-JOB";
        break;
    case MCB_RT_STATUS::NG_F:
        result = "NG-F";
        break;
    case MCB_RT_STATUS::NS_F:
        result = "NS-F";
        break;
    case MCB_RT_STATUS::NGQ:
        result = "NGQ";
        break;
    case MCB_RT_STATUS::NGA:
        result = "NGA";
        break;
    case MCB_RT_STATUS::NG_MCB:
        result = "NG-MCB";
        break;
    case MCB_RT_STATUS::IDLE:
        result = "Idel";
        break;
    }    
    return result;
}
// Get real time tool status
std::string GtcsBulletinManager::GetToolRunTimeStatus()
{
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();  
    GtcsMcbCommunication *mcb = GtcsMcbCommunication::GetInstance();
    // Checl flags
    std::array<bool,16> current_status_flags
        = BitArray::To16BiteArray(mcb->telegram.status.current_status.u16Statusflags);
    std::array<bool,16> last_status_lags 
        = BitArray::To16BiteArray(mcb->telegram.status.last_status.u16Statusflags);
    std::array<bool,16> tmd_flags
        = BitArray::To16BiteArray(mcb->telegram.status.current_status.u16TMDFlags);
    // Check sensor.
    bool tmd_start    = tmd_flags[TMD_INPUT::LEVER_SW];
    // Check flag status.
    bool tool_start   = current_status_flags[STATUS_FlAGS_IDX::TOOL_START];
    bool tool_reverse = current_status_flags[STATUS_FlAGS_IDX::TOOL_REVERSE];
    bool tool_NG      = current_status_flags[STATUS_FlAGS_IDX::ERROR_OCCURED];
    bool proc_status  = current_status_flags[STATUS_FlAGS_IDX::PROC_STATUS];
    bool last_proc_status = last_status_lags[STATUS_FlAGS_IDX::PROC_STATUS];
    
    // Get last RT status.
    std::string result = mcb->telegram.status.last_rt_status;

    // Check current RT Status.    
    if (tool_start)
    {
        if(tool_reverse == false)
        {
            // result = "Running    "; //
            result = "Running____"; //
            // result = "Running"; // 

        }
        else{
            // result = "Reverse    ";
            result = "Reverse____";
            // result = "Reverse";
        }
    }
    else if(tool_NG)
    {
        // result = "NG         ";
        result = "NG_________";
        // result = "NG";
    }    
    else if(last_proc_status!=proc_status)
    {
        // result = "OK         ";
        result = "OK_________";
        // result = "OK";
    }
    else if(last_proc_status==proc_status)
    {
        if (tmd_start==false)
        {
            // result = "NG         ";
            result = "NG_________";
            // result = "NG";
        }
    }
    mcb->telegram.status.last_rt_status = result;
    return result;
}
// Get MCB realy time status string.
int GtcsBulletinManager::ConvertActuralData300()
{
    int result = -1;
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();  
    GtcsMcbCommunication *mcb = GtcsMcbCommunication::GetInstance();
    AmsDATA300Struct *data300 = &bulletin->AmsBulletin.DATA300Struct;
    GtcsStatusTelegramStrcut *mcb_status = &mcb->telegram.status.current_status;
    
    // Get status.
    double toolmaxtorque = 7;
    std::array<bool,16> current_status_flags
        = BitArray::To16BiteArray(mcb->telegram.status.current_status.u16Statusflags);
    std::array<bool,16> last_status_lags 
        = BitArray::To16BiteArray(mcb->telegram.status.last_status.u16Statusflags);
    std::array<bool,32> error_flags  
        = BitArray::To32BiteArray(mcb->telegram.status.current_status.u32ActError);
    
    // Calcuate angle & revalution.
    double gear     = (double)bulletin->McbBulletin.BasicPara.u16GearBoxRatio/100; // Get gear box.
    double acttorque  = ((double)mcb_status->u16ActTorque/1862) * toolmaxtorque; // Calculate Act torque.
    double angle      = (double)mcb_status->u32Angle/(gear*200) * 360;           // Calculate revalution.
    double maxtorque  = ((double)mcb_status->u16MaxTorque/1862) * toolmaxtorque; // Calculate Act torque. 
    double revolution = (double)mcb_status->u32Revolutions/(gear * 200) * 360;   // Calculate revalution.
    std::string current_rt_status = GetToolRunTimeStatus();
    std::string current_mcb_err = "NO-ERR______________";
    // data300->header = std::to_string(0);      // str1:Header+DATA
    // time.
    time_t now = time(0);
    tm *ltm = localtime(&now);
    data300->datetime    = std::to_string(ltm->tm_year)+
                        std::to_string(ltm->tm_mon) +
                        std::to_string(ltm->tm_mday)+
                        '_'+
                        std::to_string(ltm->tm_hour)+
                        ':'+
                        std::to_string(ltm->tm_min)+
                        ':'+
                        std::to_string(ltm->tm_sec); // str2:yyyyMMdd HH:mm:ss
    data300->checksum    = std::to_string(angle);   // str3:check sum ,4 chars
    data300->cmdsn       = std::to_string(0);   // str4:Command_sn
    data300->dervicetype = std::to_string(0);   // str5:Device type
    data300->toolsn      = std::to_string(0);   // str6:Tool SN
    data300->dervicesn   = std::to_string(0);   // str7:Device SN
    data300->jobid         = std::to_string(0); // str8:Job ID
    data300->seqid         = std::to_string(0); // str9:Sequence ID
    data300->progid        = std::to_string(mcb_status->u16ActProcNr); // str10:Program ID
    data300->stepid        = std::to_string(mcb_status->u16ActStepNr); // str11:Step ID
    data300->dircetion     = std::to_string(0); // str12:Direction
    data300->torqueuint    = std::to_string(0); // str13:Torque unit
    data300->inc_dec       = std::to_string(0); // str14:INC/DEC
    data300->last_screwcnt = std::to_string(0); // str15:Last_screw_count
    data300->max_screwcnd  = std::to_string(0); // str16:Max_screw_count
    data300->fasteningtime = std::to_string(0); // str17:Fastening time
    data300->acttorque     = std::to_string(acttorque); // str18:Torque
    data300->actangle      = std::to_string(angle); // str19:Angle
    data300->maxtorque     = std::to_string(maxtorque); // str20:Max Torque
    data300->revolutions   = std::to_string(revolution); // str21:Revolutions
    data300->status        = current_rt_status; // str22:Status // data300->status        = std::to_string(0); // str22:Status
    data300->inputio       = std::to_string(0); // str23:Inputio
    data300->outputio      = std::to_string(0); // str24:Outputio
    data300->errmsg        = current_mcb_err; // str25:Error Masseage
    data300->toolcnt       = std::to_string(0); // str26:Tool Count
    data300->actrpm        = std::to_string(mcb_status->u16ActRPM); // str27:RPM
    data300->toolstatus    = std::to_string(0); // str28:Tool status
    return result;
}
