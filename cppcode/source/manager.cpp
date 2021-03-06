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
#include "../include/manager.h"

/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      Manager::Manager()
 *
 *  @brief   Manager constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
Manager::Manager(/* args */)
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      Manager::Manager()
 *
 *  @brief   Manager distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
Manager::~Manager()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      Manager::GetMainFSM()
 *
 *  @brief   Get main fasm.
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int Manager::GetMainFSM()
{
    return MainFSM;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      Manager::Manager()
 *
 *  @brief   Set Miain fsm.
 *
 *  @param   int main_fsm
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void Manager::SetMainFSM(int main_fsm)
{
    MainFSM = main_fsm;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      NtcsManager::NtcsManager()
 *
 *  @brief   NtcsManager constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
NtcsManager::NtcsManager()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      NtcsManager::~NtcsManager()
 *
 *  @brief   NtcsManager distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
NtcsManager::~NtcsManager()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GtcsManager()
 *
 *  @brief   GtcsManager Constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsManager::GtcsManager()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::~GtcsManager()
 *
 *  @brief   GtcsManager distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsManager::~GtcsManager()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetRtLockedStatusMessage(int lcstatusnum)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   int lcstatusnum
 *
 *  @return  string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetRtLockedStatusMessage(int lcstatusnum)
{
    std::string result = "";
    switch (lcstatusnum)
    {
    case LOCKED_STATUS::RUNNING:
        result = "RUNNING____";
        break;
    case LOCKED_STATUS::REVERSE:
        result = "REVERSE____";
        break;
    case LOCKED_STATUS::IDLE:
        result = "IDLE_______";
        break;
    case LOCKED_STATUS::OK:
        result = "OK_________";
        break;
    case LOCKED_STATUS::OK_SEQUENCE:
        result = "OK_SEQUENCE";
        break;
    case LOCKED_STATUS::OK_JOB:
        result = "OK_JOB_____";
        break;
    case LOCKED_STATUS::NG_F:
        result = "NG_F_______";
        break;
    case LOCKED_STATUS::NS_F:
        result = "NS_F_______";
        break;
    case LOCKED_STATUS::NGQ:
        result = "NGQ________";
        break;
    case LOCKED_STATUS::NGA:
        result = "NGA________";
        break;
    case LOCKED_STATUS::NG_MCB:
        result = "NG_________";
        break;
    case LOCKED_STATUS::NG_OK:
        result = "NG_________";
        break;
    case LOCKED_STATUS::NGR:
        result = "NGR________";
        break;
    case LOCKED_STATUS::NGT:
        result = "NGT________";
        break;
    default:
        break;
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetScrewDriverRunTimeStatus(GtcsScrewSequenceHandler &screwhandler)
 *
 *  @brief   Get real time tool status.
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDriverRunTimeStatus(GtcsScrewSequenceHandler &screwhandler,StatusTelegram &mcbstatus)
{
    // Check flags.
    std::array<bool, 16> current_status_flags = BitArray::To16BiteArray(mcbstatus.current_status.u16Statusflags);
    std::array<bool, 16> last_status_lags = BitArray::To16BiteArray(mcbstatus.last_status.u16Statusflags);
    std::array<bool, 16> tmd_flags = BitArray::To16BiteArray(mcbstatus.current_status.u16TMDFlags);

    // Check start signal.
    bool start_signal = tmd_flags[TMD_INPUT::LEVER_SW];   

    // Check flag status.
    bool last_proc_status = last_status_lags[STATUS_FlAGS_IDX::PROC_STATUS];
    bool tool_run = current_status_flags[STATUS_FlAGS_IDX::TOOL_START];
    bool tool_reverse = current_status_flags[STATUS_FlAGS_IDX::TOOL_REVERSE];
    bool tool_NG = current_status_flags[STATUS_FlAGS_IDX::ERROR_OCCURED];
    bool proc_status = current_status_flags[STATUS_FlAGS_IDX::PROC_STATUS];
    int lc_statusnum = screwhandler.statusnum;

    #pragma region Check current RT Status.
    if (tool_NG == true)
    {
        lc_statusnum = (int)LOCKED_STATUS::NG_MCB;
    }
    else
    {
        if (tool_run == true)
        {
            screwhandler.screwok = false;
            if (tool_reverse == false)
            {
                lc_statusnum = (int)LOCKED_STATUS::RUNNING;
            }
            else
            {
                lc_statusnum = (int)LOCKED_STATUS::REVERSE;
            }
        }
        else
        {
            if (last_proc_status != proc_status)
            {
                screwhandler.screwok = true;
                lc_statusnum = (int)LOCKED_STATUS::OK;
            }
            else if (start_signal == false)
            {
                if (screwhandler.screwok == false)
                {
                    lc_statusnum = (int)LOCKED_STATUS::NG_OK;
                }
            }
        }
    }
    #pragma endregion

    // Get current step id
    if (mcbstatus.current_status.u16ActStepNr !=0)
    {
        screwhandler.currentstepid = (mcbstatus.current_status.u16ActStepNr-3000)+1;
        if ((tool_run==true)&&(start_signal==true))
        {
            screwhandler.laststepid = screwhandler.currentstepid;
        }
    }
    // Assign data to last locked status num.
    screwhandler.statusnum = lc_statusnum;
    screwhandler.currentstatusmessage = GetRtLockedStatusMessage(lc_statusnum);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/20
 *
 *  @fn      GtcsManager::GetScrewDriverRunTimeStatusMessage(GtcsScrewSequenceHandler &screwhandler,StatusTelegram &mcbstatus)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 * 
 *  @param   StatusTelegram &mcbstatus
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDriverRunTimeStatusMessage(GtcsScrewSequenceHandler &screwhandler,StatusTelegram &mcbstatus)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/02
 *
 *  @fn      GtcsManager::GetStartSignalStatus(uint16_t &tmdflags)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint16_t &tmdflags
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetStartSignalStatus(uint16_t &tmdflags)
{
    // Convert flags to 16-bits arrays.
    std::array<bool, 16> tmd_flags = BitArray::To16BiteArray(tmdflags);
    // Check sensor.
    bool startsignal = tmd_flags[TMD_INPUT::LEVER_SW];
    return startsignal;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/02
 *
 *  @fn      GtcsManager::GetSystemErrorStatus(uint16_t &statusflags)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint16_t &statusflags
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetSystemErrorStatus(uint16_t &statusflags)
{
    // Convert flags to 16-bits arrays.
    std::array<bool, 16> status_flags = BitArray::To16BiteArray(statusflags);
    // Check sensor.
    bool erroroccured = status_flags[STATUS_FlAGS_IDX::ERROR_OCCURED];
    return erroroccured;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetMCBErrMessageString(uint32_t errorflagindex)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint32_t errorflagindex
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetMCBErrMessageString(uint32_t errorflagindex)
{
    std::string result = "NO-ERR______________";
    switch (errorflagindex)
    {
    case MCB_ERR_FLAG::ERR_CONT_TEMP: // 0
        result = "ERR-CONT-TEMP_______";
        break;
    case MCB_ERR_FLAG::ERR_MOT_TEMP: // 1
        result = "ERR-MOT-TEMP________";
        break;
    case MCB_ERR_FLAG::ERR_MOT_CURR: // 2
        result = "ERR-MOT-CURR________";
        break;
    case MCB_ERR_FLAG::ERR_MOT_PEAK_CURR: // 3
        result = "ERR-MOT-PEAK-CURR___";
        break;
    case MCB_ERR_FLAG::ERR_MOT_TORQUE: // 4
        result = "ERR-MOT-TORQUE______";
        break;
    case MCB_ERR_FLAG::ERR_DEADLOCK: // 5
        result = "ERR-DEADLOCK________";
        break;
    case MCB_ERR_FLAG::ERR_PROC_MINTIME: // 6
        result = "ERR-PROC_MINTIME____";
        break;
    case MCB_ERR_FLAG::ERR_PROC_MAXTIME: // 7
        result = "ERR-PROC-MAXTIME____";
        break;
    case MCB_ERR_FLAG::ERR_ENCODER: // 8
        result = "ERR-ENCODER_________";
        break;
    case MCB_ERR_FLAG::ERR_HALL: // 9
        result = "ERR-HALL____________";
        break;
    case MCB_ERR_FLAG::ERR_BUSVOLT_HIGH: // 10
        result = "ERR-BUSVOLT-HIGH____";
        break;
    case MCB_ERR_FLAG::ERR_BUSVOLT_LOW: // 11
        result = "ERR-BUSVOLT-LOW_____";
        break;
    case MCB_ERR_FLAG::ERR_PROG_NA: // 12
        result = "ERR-PROG-NA_________";
        break;
    case MCB_ERR_FLAG::ERR_PROC_NA: // 13
        result = "ERR-PROC-NA_________";
        break;
    case MCB_ERR_FLAG::ERR_STEP_NA: // 14
        result = "ERR-STEP-NA_________";
        break;
    case MCB_ERR_FLAG::ERR_DMS_COMM: // 15
        result = "ERR-DMS-COMM________";
        break;
    case MCB_ERR_FLAG::ERR_FLASH: // 16
        result = "ERR-FLASH___________";
        break;
    case MCB_ERR_FLAG::ERR_FRAM: // 17
        result = "ERR-FRAM____________";
        break;
    case MCB_ERR_FLAG::ERR_ANGLE: // 18
        result = "ERR-ANGLE___________";
        break;
    case MCB_ERR_FLAG::ERR_REVOLUTIONS: // 19
        result = "ERR-REVOLUTIONS_____";
        break;
    case MCB_ERR_FLAG::ERR_PROTECT_CIRCUIT: // 20
        result = "ERR-PROTECT-CIRCUIT_";
        break;
    case MCB_ERR_FLAG::ERR_SWITCH_CONFIG: // 21
        result = "ERR-SWITCH-CONFIG___";
        break;
    case MCB_ERR_FLAG::ERR_DISPLAY_COMM_TO: // 22
        // result = "ERR-DISPLAY-COMM-TO_";
        break;
    case MCB_ERR_FLAG::ERR_STEP_NOT_REC: // 23
        result = "ERR-STEP-NOT-REC____";
        break;
    case MCB_ERR_FLAG::ERR_TMD_FRAM: // 24
        result = "ERR-TMD-FRAM________";
        break;
    default:
        result = "NO-ERR______________";
        break;
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetCurrentSystemStatusMessage(uint16_t &currentstatusnum,uint32_t errorflags)
 *
 *  @brief   ( Constructivist )
 * 
 *  @param   uint16_t &currentstatusnum
 *
 *  @param   uint32_t errorflags
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetCurrentSystemStatusMessage(uint16_t &currentstatusnum,uint32_t errorflags)
{
    std::string result = "";
    std::array<bool, 32> errorflagarray = BitArray::To32BiteArray(errorflags);
    int count = 24;

    switch(currentstatusnum)
    {
        case 1:
            for (uint32_t i = 0; i < count; i++)
            {
                if (errorflagarray[i] == 1)
                {
                    if (i != 22)
                    {
                        result = GetMCBErrMessageString(i);
                    }
                }
            }
            break;
        case 2:
            result = "SETTING_____________";
            break;
        default:
            result = "NO-ERR______________";
            break;
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetRealTimeActuralValue(AmsDATA300Struct &data300,GtcsScrewSequenceHandler &screwhandler,GtcsStatusTelegramStrcut &mcbstatus)
 *
 *  @brief   Get MCB realy time status string.
 *
 *  @param   AmsDATA300Struct &data300
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @param   GtcsStatusTelegramStrcut &mcbstatus
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetRealTimeActuralValue(AmsDATA300Struct &data300,GtcsScrewSequenceHandler &screwhandler,GtcsStatusTelegramStrcut &mcbstatus)
{
    // Get status.
    float toolmaxtorque = 5;
    std::array<bool, 16> current_status_flags = BitArray::To16BiteArray(mcb->telegram.status.current_status.u16Statusflags);
    std::array<bool, 16> last_status_lags = BitArray::To16BiteArray(mcb->telegram.status.last_status.u16Statusflags);
    std::array<bool, 32> error_flags = BitArray::To32BiteArray(mcb->telegram.status.current_status.u32ActError);

    // Calcuate angle & revalution.
    float gear = (float)screwhandler.u16GearBoxRatio / 100;                 // Get gear box.
    // Calculate Act torque.
    std::string acttorque = DataSorter::GetFloatScaleSortString(((float)mcbstatus.u16ActTorque / 1862) * toolmaxtorque, 4); 
    // Calculate angle.
    std::string angle = "0.0";
    if (mcbstatus.u32Angle<0x0fffffff)
    {
        angle = DataSorter::GetFloatScaleSortString((float)mcbstatus.u32Angle / (gear * 200) * 360, 1); 
    }
    else
    {
        angle = DataSorter::GetFloatScaleSortString(-(float)(0xffffffff-mcbstatus.u32Angle)/ (gear * 200) * 360, 1); // Complement
    }
    // Calculate max torque.
    std::string maxtorque = DataSorter::GetFloatScaleSortString(((float)mcbstatus.u16MaxTorque / 1862) * toolmaxtorque, 4); 
    // Calculate revalution.
    std::string revolution = DataSorter::GetFloatScaleSortString((float)mcbstatus.u32Revolutions / (gear * 200) * 360, 4);  
    std::string current_rt_status = screwhandler.currentstatusmessage;
    std::string current_status_msg = GetCurrentSystemStatusMessage(screwhandler.currentstatusnum,mcbstatus.u32ActError);
    // std::string fasteningtime = DataSorter::GetFloatScaleSortString((float)screwhandler.fasteningtime / CLOCKS_PER_SEC, 2);
    std::string fasteningtime = DataSorter::GetFloatScaleSortString((float)screwhandler.fasteningtime/10000, 2);

    // time.
    data300.datetime =  DateTime::GetCurrentSystemDateTime(); // str2:yyyyMMdd HH:mm:ss
    data300.checksum = std::to_string(0);                     // str3:check sum ,4 chars
    data300.cmdsn = std::to_string(0);                        // str4:Command_sn
    data300.dervicetype = std::to_string(0);                  // str5:Device type
    data300.toolsn = std::to_string(0);                       // str6:Tool SN
    data300.dervicesn = std::to_string(0);                    // str7:Device SN
    data300.jobid = std::to_string(screwhandler.GtcsJob.jobid);      // str8:Job ID
    data300.seqid = std::to_string(screwhandler.currentsequenceid);  // str9:Sequence ID.
    // str10:Program name.
    data300.progid = screwhandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].program_name;
    data300.stepid = std::to_string(screwhandler.laststepid);  // str11:Step ID
    data300.dircetion = std::to_string(0);                                  // str12:Direction
    data300.torqueuint = std::to_string(screwhandler.torqueunit);           // str13:Torque unit
    data300.inc_dec = std::to_string(screwhandler.batchmode);               // str14:INC/DEC
    // package data300 last screw by batch mode setting. 
    if (screwhandler.batchmode==(int)GTCS_BATCH_MODE::DEC)
    {
        data300.last_screwcnt = std::to_string(screwhandler.screwcounter);      // str15:Last_screw_count
    }
    else
    {
        if(CheckScrewDriverCountingFinished(screwhandler)==true)
        {
            data300.last_screwcnt = std::to_string(screwhandler.screwcounter-1); // str15:Last_screw_count
        }
        else
        {
            data300.last_screwcnt = std::to_string(screwhandler.screwcounter); // str15:Last_screw_count
        }
    }
    data300.max_screwcnd = std::to_string(screwhandler.maxscrewcounter);    // str16:Max_screw_count
    data300.fasteningtime = fasteningtime;                  // str17:Fastening time
    data300.acttorque = acttorque;                            // str18:Torque
    data300.actangle = angle;                                 // str19:Angle
    data300.maxtorque = maxtorque;                            // str20:Max Torque
    data300.revolutions = revolution;                         // str21:Revolutions
    data300.status = current_rt_status;                       // str22:Status // data300.status        = std::to_string(0); // str22:Status
    data300.inputio = std::to_string(0);                      // str23:Inputio
    data300.outputio = std::to_string(0);                     // str24:Outputio
    data300.errmsg = current_status_msg;                      // str25:Error Masseage
    data300.toolcnt = std::to_string(0);                      // str26:Tool Count
    data300.actrpm = std::to_string(mcbstatus.u16ActRPM);     // str27:RPM
    if(bulletin->ScrewHandler.IsEnable == true)
    {
        data300.toolstatus = std::to_string(0);                // sxtr28:enable
    }
    else
    {
        data300.toolstatus = std::to_string(1);                // sxtr28:disable
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsManager::ClearRamdiskTxtFile()
 *
 *  @brief   CMD340->ANS340
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::ClearRamdiskTxtFile()
{
    // Get AMS data300 command string.
    if (txthandler.ClearTxtFile(txt_ramdisk_Path) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "txthandler.ClearTxtFile path = "<< txt_ramdisk_Path << std::endl;
        std::cout << "txthandler.ClearTxtFile fail." << std::endl;
        #endif
        return false;
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsManager::WriteRealTimeActuralValueToRamdisk(AmsDATA300Struct &data300)
 *
 *  @brief   CMD340->ANS340
 *
 *  @param   AmsDATA300Struct &data300
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::WriteRealTimeActuralValueToRamdisk(AmsDATA300Struct &data300)
{
    // Get AMS data300 command string.
    std::string data300_string = ams->GetAmsBulletin(AMSCMD::DATA300);
    // Get AMS data300 command string.
    if (txthandler.WriteToTxtFile(txt_ramdisk_Path,data300_string) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "txthandler.WriteToTxtFile fail." << std::endl;
        #endif
        return false;
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   AmsCMD340Struct &amscmd
 *
 *  @param   McbID2Struct &mcb_basic
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetAmsBasicToMcbStruct(AmsCMD340Struct &amscmd, McbID2Struct &mcb_basic)
{
    //
    mcb_basic.s16MinTemp = (uint16_t)(std::stof(amscmd.str5) * 10);       // SID = 1,Minimal Temperature of the motor and the motorcontroller.
                                                                          // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
    mcb_basic.s16MaxTemp = (uint16_t)(std::stof(amscmd.str6) * 10);       // SID = 2,Maximal Temperature of the motor and the motorcontroller.
                                                                          // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
    mcb_basic.u16MaxCurrent = (uint16_t)std::stoi(amscmd.str7);           // SID = 3,This is the absolute maximum motor current value.
                                                                          // There are also maximum current values for the specific process steps
                                                                          // (see….), but this parameter is the limit. Unit is [mA].
    mcb_basic.u16MaxPeakCurrent = (uint16_t)std::stoi(amscmd.str8);       // SID = 4,This value controls the threshold of the peak current comparator.
                                                                          // The peak current detection is built in for the protection of the device
                                                                          // when a latch up situation occurs. Unit is [A].
    mcb_basic.u16TorqueSensorType = (uint16_t)std::stoi(amscmd.str9);     // SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
    mcb_basic.u16MaxDutyCycle = (uint16_t)(std::stof(amscmd.str10) * 10); // SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]

    //mcb_basic.u16MaxTorque
    //   = (uint16_t)std::stoi(amscmd.str11);   // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
    mcb_basic.u16MaxTorque = 1862; // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"

    mcb_basic.u16PWMFreq = (uint16_t)std::stoi(amscmd.str12);               // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
    mcb_basic.u16MaxRPM = (uint16_t)std::stoi(amscmd.str13);                // SID = 9,This is the absolute maximum motor rpm value.
                                                                            // There are also rpm values for the specific process steps (see….) ,
                                                                            // but this parameter is the limit. Unit is [rpm].
    mcb_basic.u16MaxSlope = (uint16_t)std::stoi(amscmd.str14);              // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                                                            // There are also rpm slope values for the specific process steps (see….) ,
                                                                            // but this parameter is the limit. Unit is [rpm/s].
    mcb_basic.u16MinBusVolt = (uint16_t)(std::stof(amscmd.str15) * 10);     // SID = 11,Minimal Bus Voltage of the Power Supply.
                                                                            // Underneath this Voltage the tool doesn’t work.
                                                                            // Unit is [0,1V] (600 = 60V).
    mcb_basic.u16MaxBusVolt = (uint16_t)(std::stof(amscmd.str16) * 10);     // SID = 12,Maximal Bus Voltage of the Power Supply.
                                                                            // Above this Voltage the tool doesn’t work. Unit is [0,1V].
    mcb_basic.u16StartDutyCycle = (uint16_t)(std::stof(amscmd.str17) * 10); // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].(20 = 10%)

    mcb_basic.u16GearBoxRatio = (uint16_t)(std::stof(amscmd.str18) * 100); // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    mcb_basic.u32StartInp = (uint16_t)std::stoi(amscmd.str19);             // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    mcb_basic.u32RevInp = (uint16_t)std::stoi(amscmd.str20);               // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
    mcb_basic.u16RevRpm = (uint16_t)std::stoi(amscmd.str21);               // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
    mcb_basic.u16RevSlope = (uint16_t)std::stoi(amscmd.str22);             // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
    mcb_basic.u16RevMaxCurrent = (uint16_t)std::stoi(amscmd.str23);        // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."

    //mcb_basic.u16RevMaxTorque
    // = (uint16_t)std::stoi(amscmd.str24);   // SID = 20,"Maximum Torque of reverse drive.
    //                                         // Value is 0- 1862 (max TMD Raw Value)"
    mcb_basic.u16RevMaxTorque = 1862;

    mcb_basic.u16ErrorIdleTime = (uint16_t)std::stoi(amscmd.str25); // SID = 21,"Idle time of the motor controller after a Error condition.Unit is [ms]."
    mcb_basic.u16BackLash = (uint16_t)std::stoi(amscmd.str26);      // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or
                                                                    // Hallsensors are used for angle Positioning. Unit is [Increments]."
    mcb_basic.u16PGain = (uint16_t)std::stoi(amscmd.str27);         // SID = 23,Proportional Gain for the RPM Regulator.
    mcb_basic.u16IGain = (uint16_t)std::stoi(amscmd.str28);         // SID = 24,Integral Gain for the RPM Regulator.
    mcb_basic.u16Encoder = (uint16_t)std::stoi(amscmd.str29);       // SID = 25,"0 = No Encoder (positioning with Hallsensors).

    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   AmsCMD340Struct &amscmd
 *
 *  @param   McbID2Struct &mcb_basic
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::InsertRealTimeActuralValueToDatabase(AmsDATA300Struct &data300)
{
    // Initial object.
    GtcsScrewStatusbaseInfo screwstatus;
    GtcsScrewStatusDatabase dbscrewstatus(db_screw_ramdisk_Path);

    // Assign data to screw status information struct.
    screwstatus.data["data_time"]         = data300.datetime;            // colunm_index = 0
    screwstatus.data["device_type"]       = data300.dervicetype;         // colunm_index = 1
    screwstatus.data["device_sn"]         = data300.dervicesn;           // colunm_index = 2
    screwstatus.data["tool_sn"]           = data300.toolsn;              // colunm_index = 3
    screwstatus.data["selected_job"]      = data300.jobid;               // colunm_index = 4
    screwstatus.data["selected_sequence"] = data300.seqid;               // colunm_index = 5
    screwstatus.data["program_name"]      = data300.progid;               // colunm_index = 6
    screwstatus.data["selected_step"]     = data300.stepid;              // colunm_index = 7
    screwstatus.data["direction"]         = data300.dircetion;           // colunm_index = 8
    screwstatus.data["fasten_unit"]       = data300.torqueuint;          // colunm_index = 9
    screwstatus.data["count_direction"]   = data300.inc_dec;             // colunm_index = 10
    screwstatus.data["last_screw_count"]  = data300.last_screwcnt;       // colunm_index = 11
    screwstatus.data["max_screw_count"]   = data300.max_screwcnd;        // colunm_index = 12
    screwstatus.data["fasten_time"]       = data300.fasteningtime;       // colunm_index = 13
    screwstatus.data["fasten_torque"]     = data300.acttorque;           // colunm_index = 14
    screwstatus.data["fasten_angle"]      = data300.actangle;            // colunm_index = 15
    screwstatus.data["max_torque"]        = data300.maxtorque;           // colunm_index = 16
    screwstatus.data["revolutions"]       = data300.revolutions;         // colunm_index = 17
    screwstatus.data["fasten_status"]     = data300.status;              // colunm_index = 18
    screwstatus.data["input_io"]          = data300.inputio;             // colunm_index = 19
    screwstatus.data["output_io"]         = data300.outputio;            // colunm_index = 20
    screwstatus.data["error_masseage"]    = data300.errmsg;              // colunm_index = 21
    screwstatus.data["tool_count"]        = data300.toolcnt;             // colunm_index = 22
    screwstatus.data["rpm"]               = data300.actrpm;              // colunm_index = 23
    screwstatus.data["tool_status"]       = data300.toolstatus;          // colunm_index = 24

    if (dbscrewstatus.InsertScrewStatusToDatabaseTable(screwstatus) == false)
    {
        #if defined(_DEBUG_MODE_)
        std::cout << "Error to insert ScrewStatusToDatabaseTable" <<std::endl;
        #endif
        return false;
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetAmsCmdBaiscParaToAns(AmsANS340Struct &amsans,AmsCMD340Struct &amscmd)
 *
 *  @brief   CMD340->ANS340
 *
 *  @param   AmsANS340Struct &amsans
 *
 *  @param   AmsCMD340Struct &amscmd
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsManager::SetAmsCmdBaiscParaToAns(AmsANS340Struct &amsans, AmsCMD340Struct &amscmd)
{
    amsans.str5 = amscmd.str5;
    amsans.str6 = amscmd.str6;
    amsans.str7 = amscmd.str7;
    amsans.str8 = amscmd.str8;
    amsans.str9 = amscmd.str9;
    amsans.str10 = amscmd.str10;
    amsans.str11 = amscmd.str11;
    amsans.str12 = amscmd.str12;
    amsans.str13 = amscmd.str13;
    amsans.str14 = amscmd.str14;
    amsans.str15 = amscmd.str15;
    amsans.str16 = amscmd.str16;
    amsans.str17 = amscmd.str17;
    amsans.str18 = amscmd.str18;
    amsans.str19 = amscmd.str19;
    amsans.str20 = amscmd.str20;
    amsans.str21 = amscmd.str21;
    amsans.str22 = amscmd.str22;
    amsans.str23 = amscmd.str23;
    amsans.str24 = amscmd.str24;
    amsans.str25 = amscmd.str25;
    amsans.str26 = amscmd.str26;
    amsans.str27 = amscmd.str27;
    amsans.str28 = amscmd.str28;
    amsans.str29 = amscmd.str29;

    amsans.str30 = amscmd.str30;    // cl,rf
    // New
    // amsans.str30 = amscmd.str30;
    // amsans.str31 = amscmd.str31;
    // amsans.str32 = amscmd.str32;
    // amsans.str33 = amscmd.str33;
    // amsans.str34 = amscmd.str34;
    // amsans.str35 = amscmd.str35;
    // amsans.str36 = amscmd.str36;
    // amsans.str37 = amscmd.str37;
    // amsans.str38 = amscmd.str38;
    // amsans.str39 = amscmd.str39;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetSystemBasicParameter(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   AmsCMD340Struct &amscmd
 *
 *  @param   McbID2Struct &mcb_basic
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetSystemBasicParameter(AmsCMD340Struct &amscmd, McbID2Struct &mcb_basic)
{
    // Initial value.
    GtcsDatabase db_emmc(db_emmc_Path);
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    GtcsDatabaseBasicInfo basic_emmc;
    GtcsDatabaseBasicInfo basic_ramdisk;

    // step 1 : Convert AMS cmd340 to mcb struct and update MCB basic paramater.
    SetAmsBasicToMcbStruct(amscmd, mcb_basic);
    if (mcb->WriteBasicParameter(mcb_basic) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Step 1 : Error to Write basic data to MCB." << std::endl;
        #endif
        return false;
    }

    // Step 2 : Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicData(basic_emmc) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Step 2 : Read basic data from emmc database to dbstruct." << std::endl;
        #endif
        return false;
    }

    // step 3 : Update Mcb struct to emmc basic datatbase.
    if (UpdateMcbBasicParaToDB(db_emmc, basic_emmc, bulletin->McbBulletin.BasicPara) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Step 3 : Update MCB basic parameter to database." << std::endl;
        #endif
        return false;
    }

    // step 4 : Cpoy emmc database to ramdisk.
    CopyDatabase(db_ramdisk_Path, db_emmc_Path);

    // step 5 : Set ANS340
    SetAmsCmdBaiscParaToAns(bulletin->AmsBulletin.ANS340Struct, bulletin->AmsBulletin.CMD340Struct);

    // Check OK!!
    bulletin->checksysok = true;
    return bulletin->checksysok;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CheckUiSettingFSM(int uicmd)
 *
 *  @brief   Check Ui Setting FSM.
 *
 *  @param   int uicmd
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CheckUiSettingFSM(int uicmd)
{
    // Initial value 
    int count = 0;
    switch (uicmd)
    {
    case AMSCMD::CMD301:
        #pragma region CMD301 Setting sequence.
        DisableMcbScrewStatus();
        bulletin->ScrewHandler.GtcsJob.jobid = std::stoi(bulletin->AmsBulletin.CMD301Struct.str5);
        if (ScrewDriverSwitchJobHandler(bulletin->ScrewHandler.GtcsJob.jobid)==false)
        {
            #if defined(_DEBUG_MODE_)    
            std::cout << "Error to use CMD301 set ScrewDriverSwitchJobHandler." <<std::endl;
            #endif
            return false;
        }
        else
        {           
            bulletin->ScrewHandler.currentsequenceindex  = 0;
            if(ScrewDriverSwitchSequenceHandler(bulletin->ScrewHandler.GtcsJob.jobid,
                            bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id)==false)
            {
                #if defined(_DEBUG_MODE_)    
                std::cout << "Error to use CMD301 set ScrewDriverSwitchSequecneHandler." <<std::endl;
                #endif
                return false;
            }
            #if defined(_DEBUG_MODE_)    
            count = bulletin->ScrewHandler.GtcsJob.sequencelist.size();
            std::cout <<"bulletin->ScrewHandler.GtcsJob.sequencelist = " <<std::to_string(count) <<std::endl;
            for (int i = 0; i < count; i++)
            {
                std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[i].seq_id) <<std::endl;
            }
            std::cout << "bulletin->ScrewHandler.GtcsJob.job_id = ";
            std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.jobid)<<std::endl;
            std::cout << "bulletin->ScrewHandler.GtcsJob.sequencelist seq_id value = ";
            std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id)<<std::endl;
            std::cout << "bulletin->ScrewHandler.currentsequenceindex = " << std::to_string(bulletin->ScrewHandler.currentsequenceindex)<<std::endl;
            #endif
            // Get TighteningCounter form database.
            GetScrewDriverTighteningCounter(bulletin->ScrewHandler,
                                    bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].tr);
            // Setting list index.
            SetCurrentScrewDriverTighteningCounter(bulletin->ScrewHandler);
            
            #if defined(_DEBUG_MODE_)
            std::cout << "bulletin->ScrewHandler.maxscrewcounter = "<< std::to_string(bulletin->ScrewHandler.maxscrewcounter)<<std::endl;
            std::cout << "bulletin->ScrewHandler.screwcounter = "<< std::to_string(bulletin->ScrewHandler.screwcounter)<<std::endl;
            #endif
            // Setting last sequence index = current seqeuce index.
            bulletin->ScrewHandler.lastsequenceindex = bulletin->ScrewHandler.currentsequenceindex;
            bulletin->ScrewHandler.currentsequenceid 
                = bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id;
            bulletin->ScrewHandler.laststepid = 1;
            // Setting MCB to fasten status.
            mcb->telegram.status.loosen_status = false;
        }
        EnableMcbScrewStatus();
        #pragma endregion
        break;
    case AMSCMD::CMD302:
        #pragma region cmd302 sequence
        if (bulletin->AmsBulletin.CMD302Struct.str5 == "0")
        {
            bulletin->ScrewHandler.IsEnable = true;
            bulletin->AmsBulletin.ANS302Struct.str5 = "0";
            bulletin->AmsBulletin.DATA300Struct.toolstatus = "0";
        }
        else if (bulletin->AmsBulletin.CMD302Struct.str5 == "1")
        {
            bulletin->ScrewHandler.IsEnable = false;
            bulletin->AmsBulletin.ANS302Struct.str5 = "1";
            bulletin->AmsBulletin.DATA300Struct.toolstatus = "1";
        }
        else if (bulletin->AmsBulletin.CMD302Struct.str5 == "2")
        {
            bulletin->AmsBulletin.ANS302Struct.str5 = "2";
        }
        else if (bulletin->AmsBulletin.CMD302Struct.str5 == "3")
        {
            bulletin->AmsBulletin.ANS302Struct.str5 = "3";
        }
        else
        {
            return false;
        }
        #pragma endregion
        break;
    case AMSCMD::CMD303:
        #pragma region cmd303 sequence
        // Disable screw.
        DisableMcbScrewStatus();
        // Increase current index.
        if (bulletin->ScrewHandler.currentsequenceindex == (bulletin->ScrewHandler.GtcsJob.sequencelist.size()-1))
        {
            bulletin->ScrewHandler.currentsequenceindex = 0;
        }
        else
        {
            bulletin->ScrewHandler.currentsequenceindex += 1;
        }
        // Excute switch sequence . 
        if(ScrewDriverSwitchSequenceHandler(bulletin->ScrewHandler.GtcsJob.jobid,
                        bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id)==false)
        {
            #if defined(_DEBUG_MODE_)    
            std::cout << "Error to use CMD301 set ScrewDriverSwitchSequecneHandler." <<std::endl;
            #endif
            return false;
        }
        #if defined(_DEBUG_MODE_)    
        count = bulletin->ScrewHandler.GtcsJob.sequencelist.size();
        std::cout <<"bulletin->ScrewHandler.GtcsJob.sequencelist = " <<std::to_string(count) <<std::endl;
        for (int i = 0; i < count; i++)
        {
            std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[i].seq_id) <<std::endl;
        }
        std::cout << "bulletin->ScrewHandler.GtcsJob.job_id = ";
        std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.jobid)<<std::endl;
        std::cout << "bulletin->ScrewHandler.GtcsJob.sequencelist seq_id value = ";
        std::cout << std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id)<<std::endl;
        std::cout << "bulletin->ScrewHandler.currentsequenceindex = " << std::to_string(bulletin->ScrewHandler.currentsequenceindex)<<std::endl;
        #endif
        // Get TighteningCounter form database.
        GetScrewDriverTighteningCounter(bulletin->ScrewHandler,
                                bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].tr);
        // Setting list index.
        SetCurrentScrewDriverTighteningCounter(bulletin->ScrewHandler);
        
        #if defined(_DEBUG_MODE_)
        std::cout << "bulletin->ScrewHandler.maxscrewcounter = "<< std::to_string(bulletin->ScrewHandler.maxscrewcounter)<<std::endl;
        std::cout << "bulletin->ScrewHandler.screwcounter = "<< std::to_string(bulletin->ScrewHandler.screwcounter)<<std::endl;
        #endif
        // Setting last sequence index = current seqeuce index.
        bulletin->ScrewHandler.lastsequenceindex = bulletin->ScrewHandler.currentsequenceindex;
        bulletin->ScrewHandler.currentsequenceid 
            = bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id;
        bulletin->ScrewHandler.laststepid = 1;
        // Setting MCB to fasten status.
        mcb->telegram.status.loosen_status = false;
        // Enable screw 
        EnableMcbScrewStatus();
        #pragma endregion
        break;
    case AMSCMD::CMD340:
        if (SetSystemBasicParameter(bulletin->AmsBulletin.CMD340Struct, bulletin->McbBulletin.BasicPara) == false)
        {
            #if defined(_DEBUG_MODE_)
            std::cout << "Error to use CMD340 set SetSystemBasicParameter." <<std::endl;
            #endif
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetUiResponseCmd(std::string uicmd_string)
 *
 *  @brief   Get Ui Cmd Response.
 *
 *  @param   std::string uicmd_string
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetUiResponseCmd(std::string uicmd_string)
{
    int uiresponsecmd = 0;

    if (uicmd_string == "REQ300") // Polling
    {
        if (bulletin->checksysok == true)
        {
            uiresponsecmd = AMSCMD::DATA300;
        }
        else
        {
            uiresponsecmd = AMSCMD::REQ301;
        }
    }
    else // Setting
    {
        // std::cout << "ams->GetAmsCmdNum(uicmd_string) = "  << std::to_string(ams->GetAmsCmdNum(uicmd_string))<<std::endl;
        std::cout << "Ui setting cmd = " << uicmd_string << std::endl;
        uiresponsecmd = ams->GetAmsSymmetryCmdNumber(ams->GetAmsCmdNum(uicmd_string));
        // std::cout << "std::to_string(uiresponsecmd) = " << std::to_string(uiresponsecmd) << std::endl;
    }
    return ams->GetAmsBulletin(uiresponsecmd);
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetUiSettingStatus()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::GetUiSettingStatus()
{
    return bulletin->settingstatus;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetUiSettingStatus(bool status)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   bool status
 *
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
void GtcsManager::SetUiSettingStatus(bool status)
{
    bulletin->settingstatus = status;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsManager::GetGtcsGPIOTConfigure()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::GetGtcsGPIOTConfigure()
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsManager::SetGtcsGPIOTConfigure()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::SetGtcsGPIOTConfigure()
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/04/01
 *
 *  @fn      GtcsManager::StartGtcsGPIOInputThread()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::StartGtcsGPIOThread()
{
    // Initial thread.
    thread_gpio = std::thread(GtcsGPIOHandler::GtcsGPIOProcessHandler);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/04/02
 *
 *  @fn      GtcsManager::StopGtcsGPIOThread()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::StopGtcsGPIOThread()
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetUiSettingStatus(bool status)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   bool status
 *
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
std::string GtcsManager::GetCurrentDateYear()
{
    return DateTime::GetCurrentSystemDateYear();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetUiSettingStatus(bool status)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   bool status
 *
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
std::string  GtcsManager::GetCurrentDateime()
{
    return DateTime::GetCurrentSystemDateTime();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CopyDatabase(std::string destination ,std::string source)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string destination
 *
 *  @param   std::string source
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::GetGtcsScrewSequenceHandlerStatus(GtcsScrewSequenceHandler &screwhandler)
{
    screwhandler = bulletin->ScrewHandler;
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CopyDatabase(std::string destination ,std::string source)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string destination
 *
 *  @param   std::string source
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::SetGtcsScrewSequenceHandlerStatus(GtcsScrewSequenceHandler &screwhandler)
{
    bulletin->ScrewHandler = screwhandler;
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CopyDatabase(std::string destination ,std::string source)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string destination
 *
 *  @param   std::string source
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
uint16_t GtcsManager::GetGtcsGpioOutputStatus()
{
    return bulletin->ScrewHandler.outputstatus;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetGtcsGpioInputStatus(uint16_t &gpiostatus)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint16_t &gpiostatus
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::SetGtcsGpioInputStatus(uint16_t &gpiostatus)
{
    bulletin->ScrewHandler.inputstatus = gpiostatus;
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/04/12
 *
 *  @fn      GtcsManager::SetGtcsGpioOutputPinStatus(uint16_t &gpiostatus,const uint16_t &checkpinnum,bool status)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint16_t &gpiostatus
 * 
 *  @param   const uint16_t &pinnum
 * 
 *  @param   bool status
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::SetGtcsGpioOutputPinStatus(uint16_t &gpiostatus,const uint16_t &pinnum,bool status)
{
    if (status==true)
    {
        gpiostatus |= (1<<pinnum);
    }
    else
    {
        gpiostatus ^= (1<<pinnum);
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/04/12
 *
 *  @fn      GtcsManager::GetGtcsGpioInputPinStatus(const uint16_t &gpiostatus,const uint16_t &pinnum)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint16_t &gpiostatus
 *
 *  @param   uint16_t &checkpinnum
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::GetGtcsGpioInputPinStatus(const uint16_t &gpiostatus,const uint16_t &pinnum)
{
    uint16_t checknum =  gpiostatus;
    if(((checknum >> pinnum)&0x01)==0)
    {
        return false; 
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CopyDatabase(std::string destination ,std::string source)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string destination
 *
 *  @param   std::string source
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool GtcsManager::CopyDatabase(std::string destination, std::string source)
{
    std::string systemcmd = "sudo cp " + source + " " + destination;
    system(systemcmd.c_str());
    systemcmd = "sudo chmod -R 777 " + destination;
    system(systemcmd.c_str());
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::UpdateMcbBasicParaToDB(GtcsDatabase &db,GtcsDatabaseBasicInfo &db_basic,McbID2Struct &mcb_basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsDatabase &db
 *
 *  @param   GtcsDatabaseBasicInfo &db_basic
 *
 *  @param   McbID2Struct &mcb_basic
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::UpdateMcbBasicParaToDB(GtcsDatabase &db, GtcsDatabaseBasicInfo &db_basic, McbID2Struct &mcb_basic)
{
    // Update data.
    db_basic.data["mintemp"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MinTemp / 10, 1);               // Min temperature       (REAL)
    db_basic.data["maxtemp"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MaxTemp / 10, 1);               // Max temperature       (REAL)
    db_basic.data["maxcurrent"] = std::to_string(mcb_basic.u16MaxCurrent);                                             // Max current           (REAL)
    db_basic.data["maxpeakcurrent"] = std::to_string(mcb_basic.u16MaxPeakCurrent);                                     // Max peak current      (INTEGER)
    db_basic.data["torquesensortype"] = std::to_string(mcb_basic.u16TorqueSensorType);                                 // torque sensor type    (INTEGER)
    db_basic.data["maxdutycycle"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MaxDutyCycle / 10, 1);     // Max duty cycle        (REAL)
    db_basic.data["maxtorque"] = db_basic.data["maxtorque"];                                                           // Max torque            (REAL)
    db_basic.data["pwmfreq"] = std::to_string(mcb_basic.u16PWMFreq);                                                   // PWM frequency         (INTEGER)
    db_basic.data["maxrpm"] = std::to_string(mcb_basic.u16MaxRPM);                                                     // Max rpm               (INTEGER)
    db_basic.data["maxslope"] = std::to_string(mcb_basic.u16MaxSlope);                                                 // Max slope             (INTEGER)
    db_basic.data["minbusvolt"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MinBusVolt / 10, 1);         // Min bus voltage       (REAL)
    db_basic.data["maxbusvolt"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MaxBusVolt / 10, 1);         // Max bus voltage       (REAL)
    db_basic.data["startdutycycle"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16StartDutyCycle / 10, 1); // Start duty cycle      (REAL)
    db_basic.data["gearboxratio"] = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16GearBoxRatio / 100, 2);    // Gear box ratio        (REAL)
    db_basic.data["startinp"] = std::to_string(mcb_basic.u32StartInp);                                                 // Start input source    (INTEGER)
    db_basic.data["revinp"] = std::to_string(mcb_basic.u32RevInp);                                                     // Reverse ipnut source  (INTEGER)
    db_basic.data["revrpm"] = std::to_string(mcb_basic.u16RevRpm);                                                     // Reverse rpm           (INTEGER)
    db_basic.data["revslope"] = std::to_string(mcb_basic.u16RevSlope);                                                 // Reverse slope         (INTEGER)
    db_basic.data["revmaxcurrent"] = std::to_string(mcb_basic.u16RevMaxCurrent);                                       // Reverse max current   (INTEGER)
    db_basic.data["revmaxtorque"] = db_basic.data["revmaxtorque"];                                                     // Reverse max torque    (REAL)
    db_basic.data["erroridletime"] = std::to_string(mcb_basic.u16ErrorIdleTime);                                       // Error idle time       (INTEGER)
    db_basic.data["backlash"] = std::to_string(mcb_basic.u16BackLash);                                                 // Bachlash              (INTEGER)
    db_basic.data["pgain"] = std::to_string(mcb_basic.u16PGain);                                                       // Proportional gain     (INTEGER)
    db_basic.data["igain"] = std::to_string(mcb_basic.u16IGain);                                                       // Integral gain         (INTEGER)
    db_basic.data["encoder"] = std::to_string(mcb_basic.u16Encoder);                                                   // Encoder               (INTEGER)

    // Update database basic table.
    db.UpdateDatabaseBasicData(db_basic);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CompareBasicStruct(GtcsDatabaseBasicInfo &emmc,GtcsDatabaseBasicInfo &ramdisk)
 *
 *  @brief   Compare Basic Struct.
 *
 *  @param   GtcsDatabaseBasicInfo &emmc
 *
 *  @param   GtcsDatabaseBasicInfo &ramdisk
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CompareBasicStruct(GtcsDatabaseBasicInfo &emmc, GtcsDatabaseBasicInfo &ramdisk)
{
    bool result = true;

    for (int i = 0; i < emmc.columnnames.size(); i++)
    {
        if (emmc.data[emmc.columnnames[i]] != ramdisk.data[ramdisk.columnnames[i]])
        {
            result = false;
            std::cout << " key = " << emmc.columnnames[i] << " is not the same.";
            std::cout << " emmc_value = " << emmc.data[emmc.columnnames[i]];
            std::cout << " ramdisk_value = " << ramdisk.data[ramdisk.columnnames[i]] << std::endl;
        }
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetDatabaseBasicParaToAns(AmsANS340Struct &amsans,GtcsDatabaseBasicInfo &db_basic)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   AmsANS340Struct &amsans
 *
 *  @param   GtcsDatabaseBasicInfo &db_basic
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetDatabaseBasicParaToAns(AmsANS340Struct &amsans, GtcsDatabaseBasicInfo &db_basic)
{
    // GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // Setting REQ301 value.
    amsans.str5 = db_basic.data["mintemp"];          // Min temperature
    amsans.str6 = db_basic.data["maxtemp"];          // Max temperature
    amsans.str7 = db_basic.data["maxcurrent"];       // Max current
    amsans.str8 = db_basic.data["maxpeakcurrent"];   // Max peak current
    amsans.str9 = db_basic.data["torquesensortype"]; // torque sensor type
    amsans.str10 = db_basic.data["maxdutycycle"];    // Max duty cycle
    amsans.str11 = db_basic.data["maxtorque"];       // Max torque
    amsans.str12 = db_basic.data["pwmfreq"];         // PWM frequency
    amsans.str13 = db_basic.data["maxrpm"];          // Max rpm
    amsans.str14 = db_basic.data["maxslope"];        // Max slope
    amsans.str15 = db_basic.data["minbusvolt"];      // Min bus voltage
    amsans.str16 = db_basic.data["maxbusvolt"];      // Max bus voltage
    amsans.str17 = db_basic.data["startdutycycle"];  // Start duty cycle
    amsans.str18 = db_basic.data["gearboxratio"];    // Gear box ratio
    amsans.str19 = db_basic.data["startinp"];        // Start input source
    amsans.str20 = db_basic.data["revinp"];          // Reverse ipnut source
    amsans.str21 = db_basic.data["revrpm"];          // Reverse rpm
    amsans.str22 = db_basic.data["revslope"];        // Reverse slope
    amsans.str23 = db_basic.data["revmaxcurrent"];   // Reverse max current
    amsans.str24 = db_basic.data["revmaxtorque"];    // Reverse max torque
    amsans.str25 = db_basic.data["erroridletime"];   // Error idle time
    amsans.str26 = db_basic.data["backlash"];        // Bachlash
    amsans.str27 = db_basic.data["pgain"];           // Proportional gain
    amsans.str28 = db_basic.data["igain"];           // Integral gain
    amsans.str29 = db_basic.data["encoder"];         // Encoder
    // New
    // amsans.str30 = db_basic.data["mintorque"];         // Min Torque
    // amsans.str31 = db_basic.data["minrpm"];            // Min RPM
    // amsans.str32 = db_basic.data["revminrpm"];         // Reverse Min RPM
    // amsans.str33 = db_basic.data["dmsswver"];          // DmsSWVer
    // amsans.str34 = db_basic.data["dmscoreid"];         // DmsCoreID
    // amsans.str35 = db_basic.data["dmssernr"];          // DmsSerNr
    // amsans.str36 = db_basic.data["led"];               // Led
    // amsans.str37 = db_basic.data["lever_sensitivity"]; // Lever Sensitivity
    // amsans.str38 = db_basic.data["push_sensitivity"];  // Push Sensitivity
    // amsans.str39 = db_basic.data["motswver"];          // MotSWVer
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetDatabaseUnscrewData(GtcsCtrlTelegramStrcut &telegram,int jobid)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   GtcsCtrlTelegramStrcut &telegram
 *
 *  @param   int jobid
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetDatabaseUnscrewData(GtcsCtrlTelegramStrcut &telegram, int jobid)
{
    // Initial object.
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    GtcsDatabaseJobInfo jobinfo;
    float unscrew_forcerate = 0;

    // Get data from database
    if (db_ramdisk.ReadDatabaseJobData(jobinfo, jobid) == true)
    {
        #if defined(_DEBUG_MODE_)
        std::cout << "--------------------------------- " << std::endl;
        std::cout << "id                   = " << jobinfo.data["id"] << std::endl;
        std::cout << "job_id               = " << jobinfo.data["job_id"] << std::endl;
        std::cout << "job_name             = " << jobinfo.data["job_name"] << std::endl;
        std::cout << "unscrew_direction    = " << jobinfo.data["unscrew_direction"] << std::endl;
        std::cout << "force                = " << jobinfo.data["force"] << std::endl;
        std::cout << "rpm                  = " << jobinfo.data["rpm"] << std::endl;
        std::cout << "enable_unscrew_force = " << jobinfo.data["enable_unscrew_force"] << std::endl;
        std::cout << "--------------------------------- " << std::endl;
        #endif

        telegram.u16ManRpm = (uint16_t)(std::stoi(jobinfo.data["rpm"]));
        if (std::stoi(jobinfo.data["enable_unscrew_force"]) == 1)
        {
            unscrew_forcerate = std::stof(jobinfo.data["force"]) / 100;
            telegram.u16ManMaxTorque = (uint16_t)(1862 * unscrew_forcerate);
        }
        else
        {
            #if defined(_DEBUG_MODE_)
            std::cout << "Fuck unscrew_forcerate!!!" << std::endl;
            #endif
        }
    }
    else
    {
        return false;
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetDatabaseScrewSequenceListData(std::vector<GtcsSequenceDataStruct> &seqlist,int jobid)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   std::vector<GtcsSequenceDataStruct> &seqlist
 *
 *  @param   int jobid
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetDatabaseScrewSequenceListData(std::vector<GtcsSequenceDataStruct> &seqlist, int jobid)
{
    // Initial object.
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    std::vector<GtcsDatabaseSequenceInfo> db_seqlist; //
    GtcsSequenceDataStruct seq;                          //
    std::string::size_type sz;                           // alias of size_t

    // Read sequence List from database. 
    if (db_ramdisk.ReadDataBaseSequenceList(db_seqlist, jobid) == false)
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Read DataBase Sequence List error!" << std::endl;
        #endif
        return false;
    }
    // Get data from database struct.
    seqlist.clear();
    int row_size = db_seqlist.size();
    for (int i = 0; i < row_size; i++)
    {
        // Package to  ScrewHandler seqlist.
        seq.job_id          = std::stoi(db_seqlist[i].data["job_id"]);               // colunm index = 0
        seq.seq_id          = std::stoi(db_seqlist[i].data["seq_id"]);               // colunm index = 1
        seq.program_name    = db_seqlist[i].data["program_name"];                    // colunm index = 2
        seq.ok_time         = std::stoi(db_seqlist[i].data["ok_time"]);              // colunm index = 3
        seq.ng_stop         = std::stoi(db_seqlist[i].data["ng_stop"]);              // colunm index = 4
        seq.tr              = std::stoi(db_seqlist[i].data["tr"]);                   // colunm index = 5
        seq.ok_seq          = std::stoi(db_seqlist[i].data["ok_seq"]);               // colunm index = 6
        seq.ok_seq_time     = std::stof(db_seqlist[i].data["ok_seq_time"], &sz);     // colunm index = 7
        seq.seq_stop        = std::stoi(db_seqlist[i].data["seq_stop"]);             // colunm index = 8
        seq.program_mintime = std::stof(db_seqlist[i].data["program_mintime"], &sz); // colunm index = 9
        seq.program_maxtime = std::stof(db_seqlist[i].data["program_maxtime"], &sz); // colunm index = 10

        seqlist.push_back(seq);
        #ifdef _DEBUG_MODE_
        std::cout << "--------------------------------- " << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.job_id) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.seq_id) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << seq.program_name << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.ok_time) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.ng_stop) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.tr) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.ok_seq) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.ok_seq_time) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.seq_stop) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.program_mintime) << std::endl;
        std::cout << "db_seqlist[" << std::to_string(i) << "] = " << std::to_string(seq.program_maxtime) << std::endl;
        std::cout << "--------------------------------- " << std::endl;
        #endif
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetDatabaseScrewStepListData(std::vector<GtcsStepDataStruct> &steplist,int jobid, int seqid)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   std::vector<GtcsStepDataStruct> &steplist
 *
 *  @param   int jobid
 *
 *  @param   int seqid
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetDatabaseScrewStepListData(std::vector<GtcsStepDataStruct> &steplist, int jobid, int seqid)
{
    // Initial object.
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    std::vector<GtcsDatabaseStepInfo> db_steplist;
    GtcsStepDataStruct step;
    std::string::size_type sz; // alias of size_t

    // Read step data from tcs.db step table.
    if (db_ramdisk.ReadDatabaseStepList(db_steplist, jobid, seqid) == false)
    {
        std::cout << "Fuck ReadDatabaseStepList!!" << std::endl;
        return false;
    }
    // Check data aval


    // Get data from database struct.
    steplist.clear();
    int row_size = db_steplist.size();
    for (size_t i = 0; i < row_size; i++)
    {
        // Package to  ScrewHandler seqlist.
        step.job_id              = std::stoi(db_steplist[i].data["job_id"]);                   // column index = 0,  type = INTEGER
        step.seq_id              = std::stoi(db_steplist[i].data["seq_id"]);                   // column index = 1,  type = INTEGER
        step.target_type         = std::stoi(db_steplist[i].data["target_type"]);              // column index = 2,  type = INTEGER
        step.program_name        = db_steplist[i].data["program_name"];                        // column index = 3,  type = TEXT
        step.ScrewStepID         = std::stoi(db_steplist[i].data["ScrewStepID"]);              // column index = 4,  type = INTEGER
        step.u8StepName          = db_steplist[i].data["u8StepName"];                          // column index = 5,  type = TEXT
        step.u16StepRpm          = std::stoi(db_steplist[i].data["u16StepRpm"]);               // column index = 6,  type = INTEGER
        step.u16StepMaxTorque    = std::stof(db_steplist[i].data["u16StepMaxTorque"], &sz);    // column index = 7,  type = REAL
        step.u16StepMaxRevol     = std::stof(db_steplist[i].data["u16StepMaxRevol"], &sz);     // column index = 8,  type = REAL
        step.u16StepTime         = std::stof(db_steplist[i].data["u16StepTime"], &sz);         // column index = 9,  type = REAL
        step.u16StepAngle        = std::stof(db_steplist[i].data["u16StepAngle"], &sz);        // column index = 10, type = REAL
        step.u16StepAngleWindow  = std::stof(db_steplist[i].data["u16StepAngleWindow"], &sz);  // column index = 11, type = REAL
        step.u16StepTorqueWindow = std::stof(db_steplist[i].data["u16StepTorqueWindow"], &sz); // column index = 12, type = REAL
        step.ScrewHiTorque       = std::stof(db_steplist[i].data["ScrewHiTorque"], &sz);       // column index = 13, type = REAL
        step.ScrewLoTorque       = std::stof(db_steplist[i].data["ScrewLoTorque"], &sz);       // column index = 14, type = REAL
        step.ScrewHiAngle        = std::stof(db_steplist[i].data["ScrewHiAngle"], &sz);        // column index = 15, type = REAL
        step.ScrewLoAngle        = std::stof(db_steplist[i].data["ScrewLoAngle"], &sz);        // column index = 16, type = REAL
        step.TorqueThreshold     = std::stof(db_steplist[i].data["TorqueThreshold"], &sz);     // column index = 17, type = REAL
        step.TorqueJointOffset   = std::stof(db_steplist[i].data["TorqueJointOffset"], &sz);   // column index = 18, type = REAL
        step.StepDelaytime       = std::stof(db_steplist[i].data["StepDelaytime"], &sz);       // column index = 19, type = REAL
        step.ScrewStepDirection  = std::stoi(db_steplist[i].data["ScrewStepDirection"]);       // column index = 20, type = INTEGER
        step.StepMonitoringMode  = std::stoi(db_steplist[i].data["StepMonitoringMode"]);       // column index = 21, type = INTEGER
        step.off_set             = std::stoi(db_steplist[i].data["off_set"]);                  // column index = 22, type = INTEGER
        step.enable_downshift    = std::stoi(db_steplist[i].data["enable_downshift"]);         // column index = 23, type = INTEGER
        step.downshift_torque    = std::stof(db_steplist[i].data["downshift_torque"], &sz);    // column index = 24, type = REAL
        step.downshift_speed     = std::stoi(db_steplist[i].data["downshift_speed"]);          // column index = 25, type = INTEGER
        steplist.push_back(step);

        #if defined(_DEBUG_MODE_)
        // std::cout << "--------------------------------- " << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.job_id) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.seq_id) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.target_type) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << step.program_name << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewStepID) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << step.u8StepName << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepRpm) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepMaxTorque) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepMaxRevol) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepTime) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepAngle) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepAngleWindow) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.u16StepTorqueWindow) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewHiTorque) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewLoTorque) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewHiAngle) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewLoAngle) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.TorqueThreshold) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.TorqueJointOffset) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.StepDelaytime) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.ScrewStepDirection) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.StepMonitoringMode) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.off_set) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.enable_downshift) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.downshift_torque) << std::endl;
        // std::cout << "db_steplist[" << std::to_string(i) << "] = " << std::to_string(step.downshift_speed) << std::endl;
        // std::cout << "--------------------------------- " << std::endl;
        #endif
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetMcbProcessFromDatabase(McbID4Struct &mcbprocess,int jobid,int seqid)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   McbID4Struct &mcbprocess
 *
 *  @param   McbID2Struct &basic
 *
 *  @param   std::vector<GtcsStepDataStruct> &steplist
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbProcessTelegramFromDBData(McbID4Struct &mcbprocess, McbID2Struct &mcbbasic, std::vector<GtcsStepDataStruct> &steplist)
{
    // Package data to mcb telegram.
    mcbprocess.u8ProcID       = 4000;
    mcbprocess.u8ProcName     = "****";
    mcbprocess.u16ProcPGain   = mcbbasic.u16PGain;
    mcbprocess.u16ProcIGain   = mcbbasic.u16IGain;
    mcbprocess.u16ProcMaxTime = 0;
    mcbprocess.u16ProcMinTime = 0;
    mcbprocess.u16ProcRevFunc = 0;
    mcbprocess.u16NbrSteps = steplist.size();

    int stepid_max_size =  mcbprocess.stepid_list.size();

    for (int i = 0; i < stepid_max_size; i++)
    {
        if(i<mcbprocess.u16NbrSteps)
        {
            mcbprocess.stepid_list[i] = 3000+i;
        }
        else
        {
            mcbprocess.stepid_list[i] = 0;
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "i = " << std::to_string(i) << " = "<<std::to_string(mcbprocess.stepid_list[i])<<std::endl;
        #endif
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetStepListFromDatabase(std::vector<McbID3Struct> &mcbsteplist,int jobid,int seqid)
 *
 *  @brief   Set AMS Bulletin Basic Parameter.
 *
 *  @param   std::vector<McbID3Struct> &mcbsteplist
 *
 *  @param   McbID2Struct &mcbbasic
 *
 *  @param   std::vector<GtcsStepDataStruct> &steplist
 *
 *  @param   bool stopmotorflag
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbStepTelegramFromDBData( McbID3Struct &mcbstep, 
                                                McbID2Struct &mcbbasic, 
                                                GtcsStepDataStruct &dbstep,
                                                int stepindex,
                                                bool endstepflag)
{
    // Package database step data to mcb telegram.
    #if defined(_DEBUG_RAM_MODE_)
    mcbstep.u8StepID            = stepindex;          // 3XXX
    #else
    mcbstep.u8StepID            = 3000 + stepindex;          // 3XXX
    #endif
    mcbstep.u8StepName          = dbstep.u8StepName;         // SID = 1,32 byte long string which contains the name of the step.
    // property.
    mcbstep.u16StepRpm          = dbstep.u16StepRpm;         // SID = 2,Rpm of this screwing step. Unit is [rpm] (after the Gearbox)
    mcbstep.u16StepSlope        = mcbbasic.u16MaxSlope;      // SID = 3,Start slope of this screwing step. Unit is [rpm/s] (after the Gearbox).
    mcbstep.u16StepMaxCurrent   = mcbbasic.u16MaxCurrent;    // SID = 4,Maximum current of this step. Unit is [mA].

    // SID = 5,Maximum Torque Value is 0- 1862 (max Raw TMD Value)
    if (dbstep.target_type == (int)SCREW_TARGET_TYPE::TARGET_TORQUE)
    {
        mcbstep.u16StepMaxTorque    = (uint16_t)((dbstep.u16StepMaxTorque/5)*1862);   
    }
    else
    {
        if (dbstep.ScrewHiTorque!=0)
        {
            mcbstep.u16StepMaxTorque = (uint16_t)((dbstep.ScrewHiTorque/5)*1862);
        }   
        else
        {
            mcbstep.u16StepMaxTorque = 0; // 1862?  
        }    
    }    
    mcbstep.u16StepMaxRevol     = dbstep.u16StepMaxRevol;    // SID = 6,Maximum Revolutions (after the Gearbox) of this step.
                                                             // Unit is [0,01] (1000 = 10,00 Revolutions)
    mcbstep.u16StepTime         = (uint16_t)(dbstep.u16StepTime * 1000);        // SID = 7,Execution Time- time. Unit is [ms].
    mcbstep.s32StepAngle        = dbstep.u16StepAngle;       // SID = 8,Position to Angle. Unit is [0,1 °] (10 = 1°)
    mcbstep.u16StepAngleWindow  = dbstep.u16StepAngleWindow; // SID = 9,Window of the Angle Monitoring. Unit is [0,1°] (10 = 1°)
    mcbstep.u16StepTorqueWindow = dbstep.u16StepTorqueWindow;// SID = 10,Window of the torque monitoring.
                                                             // Unit is digits related to maximum Torque Value 1862 (max Raw TMD Value).
    mcbstep.u16MinDutyCycle     = mcbbasic.u16StartDutyCycle;// SID = 11,Minimum Duty Cycle Unit is [0,1%]. (10 = 1%)

    // New.
    mcbstep.u16AngleWindow2     = dbstep.ScrewLoAngle;       // SID = 14
    mcbstep.u16TorqueWindow2    = dbstep.ScrewLoTorque;      // SID = 15

    // Get Mcb step flags.
    SetMcbScrewStepFlags(mcbstep,dbstep,stepindex,endstepflag);           // SID = 12,See description of step flags.

    // Get Mcb windows mode flags.
    SetMcbStepWindowModeFlags(mcbstep,dbstep);                  // SID = 13

    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/04
 *
 *  @fn      GtcsManager::EnableMcbScrewStatus()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::EnableMcbScrewStatus()
{
    // Initial object.
    GtcsCtrlTelegramStrcut ctrltelegram;
    // Configure fasten ctrl telegram.
    ctrltelegram = mcb->telegram.ctrl.config; 
    mcb->telegram.ctrl.InitialCtrlFlags(ctrltelegram);
    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SC_ENABLE);
    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SHORT_UVW);
    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::EN_TIMEOUT_200MS);

    // Polling to MCB.
    if (mcb->GetMcbPollingStatus(ctrltelegram)==false)
    {
        return false;
    }    
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/04
 *
 *  @fn      GtcsManager::DisableMcbScrewStatus()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::DisableMcbScrewStatus()
{
    // Initial object.
    GtcsCtrlTelegramStrcut ctrltelegram;
    // Configure fasten ctrl telegram.
    ctrltelegram = mcb->telegram.ctrl.config; 
    mcb->telegram.ctrl.InitialCtrlFlags(ctrltelegram);
    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SHORT_UVW);
    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::EN_TIMEOUT_200MS);

    // Polling to MCB.
    if (mcb->GetMcbPollingStatus(ctrltelegram)==false)
    {
        return false;
    }    
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/17
 *
 *  @fn      GtcsManager::GetMcbProcessParameter(McbID4Struct &mcbprocess)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   AmsREQ301Struct &amsreq
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbProcessParameter(McbID4Struct &mcbprocess)
{
    mcb->ReadProcessParameter(mcbprocess,4000);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/17
 *
 *  @fn      GtcsManager::SetMcbProcessParameter(McbID4Struct &mcbprocess)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID4Struct &mcbprocess
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetMcbProcessParameter(McbID4Struct &mcbprocess)
{
    #if defined(_DEBUG_RAM_MODE_)
    // Write process data to MCB ram.
    mcb->WriteProcessRamData(mcbprocess);
    #else
    // Write process data to MCB flash.
    mcb->WriteProcessParameter(mcbprocess,4000);
    #endif
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/17
 *
 *  @fn      GtcsManager::GetMcbStepParameter(McbID3Struct &mcbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbStepParameter(McbID3Struct &mcbstep)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/17
 *
 *  @fn      GtcsManager::SetMcbStepParameter(McbID3Struct &mcbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetMcbStepParameter(McbID3Struct &mcbstep)
{
    #if defined(_DEBUG_RAM_MODE_)
    // Write step data to MCB flash.
    mcb->WriteStepRamData(mcbstep,mcbstep.u8StepID);
    #else
    // Write step data to MCB flash.
    mcb->WriteStepParameter(mcbstep,mcbstep.u8StepID);
    #endif

    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::GetMcbScrewStepFlags(McbID3Struct &mcbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbScrewStepFlags(McbID3Struct &mcbstep)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::SetMcbScrewStepFlags(McbID3Struct &mcbstep,GtcsStepDataStruct &dbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @param   GtcsStepDataStruct &dbstep
 *
 *  @param   bool stopmotor
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetMcbScrewStepFlags(McbID3Struct &mcbstep,GtcsStepDataStruct &dbstep,int stepindex,bool stopmotor)
{
    // Initial step flags.
    mcbstep.u16StepFlags = 0;
    // Check
    switch ((uint16_t)dbstep.target_type)
    {
        case (uint16_t)TARGET_TYPE::TORQUE:
            mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::NEXT_TORQUE;
            break;
        case (uint16_t)TARGET_TYPE::ANGLE:
            mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::NEXT_ANGLE;
            break;
        case (uint16_t)TARGET_TYPE::REVOLUTION:
            mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::NEXT_REVOLUTION;
            break;
        case (uint16_t)TARGET_TYPE::TIME:
            mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::NEXT_TIME;
            break;
    }

    // Setting step tp reset angle & revolution.
    mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::RESET_ANGLE_STA;     //
    mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::RESET_REV_STA;       //
    
    // If database ScrewStepDirection  = 1,step dircetion is CCW.
    if (dbstep.ScrewStepDirection == 1)  // CW = 0,CCW = 1 
    {
        mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::STEP_REVERSE;       //
    }
    
    // If stopmotor is true,send stop flag.
    if (stopmotor==true)
    {
        mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::STOP_MOTOR_END_STEP; //
        mcbstep.u16StepFlags |= 1<<(uint16_t)SCREW_STEP_FLAG::TIGHTENING_STEP;     //
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::GetMcbStepWindowModeFlags(McbID3Struct &mcbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetMcbStepWindowModeFlags(McbID3Struct &mcbstep)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::SetMcbStepWindowModeFlags(uint16_t &windowmodeflags,GtcsStepDataStruct &dbstep)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID3Struct &mcbstep
 *
 *  @param   GtcsStepDataStruct &dbstep
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetMcbStepWindowModeFlags(McbID3Struct &mcbstep,GtcsStepDataStruct &dbstep)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::ScrewDriverSwitchJobHandler(int jobid)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   int jobid
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::ScrewDriverSwitchJobHandler(int jobid)
{
    // Get normal unscrew data form database.
    if (GetDatabaseUnscrewData(mcb->telegram.ctrl.loosen, jobid) == false)
    {
        std::cout << " Error to get unscrew data form database." << std::endl;
        return false;
    }

    #if defined(_DEBUG_MODE_)
    // std::cout << "--------------------------------- " << std::endl;
    // std::cout << "normal.unscrew.u16Ctrlflags     = " << mcb->telegram.ctrl.loosen.u16Ctrlflags<< std::endl;
    // std::cout << "normal.unscrew.u16ControlMode   = " << mcb->telegram.ctrl.loosen.u16ControlMode<< std::endl;
    // std::cout << "normal.unscrew.u16WorkProc      = " << mcb->telegram.ctrl.loosen.u16WorkProc<< std::endl;
    // std::cout << "normal.unscrew.u16CtrlProgram   = " << mcb->telegram.ctrl.loosen.u16CtrlProgram<< std::endl;
    // std::cout << "normal.unscrew.u16ManRpm        = " << mcb->telegram.ctrl.loosen.u16ManRpm<< std::endl;
    // std::cout << "normal.unscrew.u16ManSlope      = " << mcb->telegram.ctrl.loosen.u16ManSlope<< std::endl;
    // std::cout << "normal.unscrew.u16ManMaxTorque  = " << mcb->telegram.ctrl.loosen.u16ManMaxTorque<< std::endl;
    // std::cout << "normal.unscrew.u16ManMaxCurrent = " << mcb->telegram.ctrl.loosen.u16ManMaxCurrent<< std::endl;
    // std::cout << "normal.unscrew.u16ManRpmMode    = " << mcb->telegram.ctrl.loosen.u16ManRpmMode<< std::endl;
    // std::cout << "normal.unscrew.u8TMDControl     = " << mcb->telegram.ctrl.loosen.u8TMDControl<< std::endl;
    // std::cout << "--------------------------------- " << std::endl;
    #endif

    // Get screw handler sequence list form database.
    if (GetDatabaseScrewSequenceListData(bulletin->ScrewHandler.GtcsJob.sequencelist, jobid) == false)
    {
        std::cout << "Error to get screw handler sequence list form database." << std::endl;
        return false;
    }
    else
    {
        #if defined(_DEBUG_MODE_)
        int seqlist_size = bulletin->ScrewHandler.GtcsJob.sequencelist.size();
        for (int i = 0; i < seqlist_size; i++)
        {
            std::cout << "normal.screw.jobid = " << std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[i].job_id);
            std::cout << "normal.screw.sequence = "<< std::to_string(bulletin->ScrewHandler.GtcsJob.sequencelist[i].seq_id) << std::endl;
        }
        #endif
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/18
 *
 *  @fn      GtcsManager::ScrewDriverSwitchSequecneHandler(int jobid,int seqid)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   int jobid
 *
 *  @param   int seqid
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::ScrewDriverSwitchSequenceHandler(int jobid,int seqid)
{
    // Get step data list from tcs.db step table by JobID & seqID.
    int seqindex = bulletin->ScrewHandler.currentsequenceindex;
    if (GetDatabaseScrewStepListData(bulletin->ScrewHandler.GtcsJob.sequencelist[seqindex].steplist, jobid, seqid) == false)
    {
        std::cout << "Error to get Database Screw Step List Data! "<< std::endl;
        return false;
    }
    // Get step list size.
    int steplist_size = bulletin->ScrewHandler.GtcsJob.sequencelist[seqindex].steplist.size();
    #if defined(_DEBUG_MODE_)
    std::cout << "steplist_size = "<<std::to_string(steplist_size) << std::endl;
    #endif

    // Package step data list to MCB Process telegram & send to MCB.
    GetMcbProcessTelegramFromDBData(bulletin->McbBulletin.ProcessPara,
                                    bulletin->McbBulletin.BasicPara,
                                    bulletin->ScrewHandler.GtcsJob.sequencelist[seqindex].steplist);
    // Check process data 有笑性 for mcb telegram.

    // Send process telegram to MCB.
    SetMcbProcessParameter(bulletin->McbBulletin.ProcessPara);
    bool endstepflag = false;
    // Package step data list to MCB Step telegram & write to MCB.
    for (int i = 0; i < steplist_size; i++)
    {
        if (i==(steplist_size-1))
        {
            endstepflag = true;
        }

        GetMcbStepTelegramFromDBData(bulletin->McbBulletin.StepPara,
                                    bulletin->McbBulletin.BasicPara,
                                    bulletin->ScrewHandler.GtcsJob.sequencelist[seqindex].steplist[i],
                                    i,
                                    endstepflag);
        #if defined(_DEBUG_MODE_)
        std::cout << "=============================================================================== "  << std::endl;
        std::cout << "u8StepID            = "<<std::to_string(bulletin->McbBulletin.StepPara.u8StepID)   << std::endl;
        std::cout << "u8StepName          = "<<bulletin->McbBulletin.StepPara.u8StepName                 << std::endl;
        std::cout << "u16StepRpm          = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepRpm) << std::endl;
        std::cout << "u16StepSlope        = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepSlope) << std::endl;
        std::cout << "u16StepMaxCurrent   = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepMaxCurrent) << std::endl;
        std::cout << "u16StepMaxTorque    = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepMaxTorque) << std::endl;
        std::cout << "u16StepMaxRevol     = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepMaxRevol) << std::endl;
        std::cout << "u16StepTime         = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepTime) << std::endl;
        std::cout << "s32StepAngle        = "<<std::to_string(bulletin->McbBulletin.StepPara.s32StepAngle) << std::endl;
        std::cout << "u16StepAngleWindow  = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepAngleWindow) << std::endl;
        std::cout << "u16StepTorqueWindow = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepTorqueWindow) << std::endl;
        std::cout << "u16MinDutyCycle     = "<<std::to_string(bulletin->McbBulletin.StepPara.u16MinDutyCycle) << std::endl;
        std::cout << "u16StepFlags        = "<<std::to_string(bulletin->McbBulletin.StepPara.u16StepFlags) << std::endl;
        std::cout << "u16WindowMode       = "<<std::to_string(bulletin->McbBulletin.StepPara.u16WindowMode) << std::endl;
        std::cout << "u16AngleWindow2     = "<<std::to_string(bulletin->McbBulletin.StepPara.u16AngleWindow2) << std::endl;
        std::cout << "u16TorqueWindow2    = "<<std::to_string(bulletin->McbBulletin.StepPara.u16TorqueWindow2) << std::endl;
        #endif

        // Check step data 有效性

        // Write to MCB.
        SetMcbStepParameter(bulletin->McbBulletin.StepPara);
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/15
 *
 *  @fn      GtcsManager::GetScrewDeviceInfoFormDB(GtcsScrewSequenceHandler &screwhandler,std::string &dbPath)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 * 
 *  @param   std::string &dbPath
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDeviceInfoFormDB(GtcsScrewSequenceHandler &screwhandler,std::string &dbPath)
{   
    // Intial object.
    GtcsDatabase dbDevice(dbPath);
    GtcsDatabaseDeviceInfo dbdeviceInfo;
    // Read data form tcs database Device table.
    if(dbDevice.ReadDatabaseDeviceData(dbdeviceInfo)==false)
    {
        #if defined(_DEBUG_MODE_)
        std::cout << "dbDevice.ReadDatabaseDeviceData Error!! " << std::endl;
        #endif
        return false;
    }    
    screwhandler.batchmode = (uint16_t)std::stoi(dbdeviceInfo.data["batch_mode"]); 
    screwhandler.torqueunit = (uint16_t)std::stoi(dbdeviceInfo.data["torque_unit"]);
    #if defined(_DEBUG_MODE_)
    std::cout << "screwhandler.batchmode = "<< std::to_string(screwhandler.batchmode) << std::endl;
    std::cout << "screwhandler.torqueunit = "<< std::to_string(screwhandler.torqueunit) << std::endl;
    #endif        
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/09
 *
 *  @fn      GtcsManager::CheckScrewDriverCountingFinished (GtcsScrewSequenceHandler &screwhandler)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CheckScrewDriverCountingFinished (GtcsScrewSequenceHandler &screwhandler)
{
    if (screwhandler.batchmode == (uint16_t)GTCS_BATCH_MODE::DEC)
    {
        if (screwhandler.screwcounter != 0)
        {
            return false;
        }
    }
    else
    {
        if ((screwhandler.screwcounter-1) != screwhandler.maxscrewcounter)
        {
            return false;
        }
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/09
 *
 *  @fn      GtcsManager::GetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler,int &tighteningcounter)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @param   int &tighteningcounte
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler,int &tighteningcounter)
{
    return true;
} 
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/09
 *
 *  @fn      GtcsManager::SetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @param   int &tighteningcounte
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetCurrentScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler)
{
    if (screwhandler.batchmode==(int)GTCS_BATCH_MODE::DEC)
    {
        screwhandler.screwcounter = screwhandler.maxscrewcounter;
    }
    if (screwhandler.batchmode==(int)GTCS_BATCH_MODE::INC)
    {
        screwhandler.screwcounter = 1;                          // 
    }    
    return true;
} 
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwsequenceandler)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @param   int &tighteningcounte
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler,int &tighteningcounter)
{
    // Set max cpunter.
    screwhandler.maxscrewcounter = tighteningcounter;
    #if defined(_DEBUG_MODE_)
    std::cout << "screwhandler.screwcounter = " <<std::to_string(screwhandler.screwcounter)<<std::endl;
    #endif
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &handler)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetScrewDriverTighteningCounter(GtcsScrewSequenceHandler &screwhandler)
{
    // Check screw driver counting is finished. // if (screwhandler.screwcounter != 0)
    if (CheckScrewDriverCountingFinished(screwhandler) == true)
    {
        return false;
    }
    else
    {
        if (screwhandler.screwcounterlocked==false)
        {
            if (screwhandler.batchmode == (uint16_t)GTCS_BATCH_MODE::INC)
            {
                screwhandler.screwcounter++;
            }
            else
            {
                screwhandler.screwcounter--;
            }
            screwhandler.screwcounterlocked = true;
        }
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/19
 *
 *  @fn      GtcsManager::SetScrewDriverFasteningStartClock(clock_t &scclock)
 *
 *  @brief   ( Constructivist )
 * 
 *  @param   clock_t &scclock
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetScrewDriverFasteningStartClock(clock_t &scclock)
{
    DateTime::GetCurrentSystemClock(scclock);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/19
 *
 *  @fn      GtcsManager::GetScrewDriverFasteningTime(uint16_t &fastentime,clock_t &startclock)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   uint32_t &fastentime
 * 
 *  @param   clock_t &startclock
 *  
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDriverFasteningTime(uint32_t &fastentime,clock_t &startclock)
{
    fastentime = clock()-startclock;
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/12
 *
 *  @fn      GtcsManager::GetScrewDriverNextSeqindex(GtcsScrewSequenceHandler  &screwhandler)
 *
 *  @brief   ( Constructivist )
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::GetScrewDriverNextSeqindex(GtcsScrewSequenceHandler  &screwhandler)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/12
 *
 *  @fn      GtcsManager::SetScrewDriverNextSeqindex(GtcsScrewSequenceHandler  &screwhandler)
 * 
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewSequenceHandler &screwhandler
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetScrewDriverNextSeqindex(GtcsScrewSequenceHandler  &screwhandler)
{
    if (screwhandler.GtcsJob.jobid == 0) //normal
    {
        screwhandler.currentsequenceindex = screwhandler.maxscrewcounter; // 
    }
    else
    {            
        screwhandler.currentsequenceindex +=1;   // Move to next sequence id.
    }    
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetDatabaseBasicParaToReq(AmsREQ301Struct &amsreq,GtcsDatabaseBasicInfo &db_basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   AmsREQ301Struct &amsreq
 *
 *  @param   GtcsDatabaseBasicInfo &db_basic
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetDatabaseBasicParaToReq(AmsREQ301Struct &amsreq, GtcsDatabaseBasicInfo &db_basic)
{
    // GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // Setting REQ301 value.
    amsreq.str5 = db_basic.data["mintemp"];          // Min temperature
    amsreq.str6 = db_basic.data["maxtemp"];          // Max temperature
    amsreq.str7 = db_basic.data["maxcurrent"];       // Max current
    amsreq.str8 = db_basic.data["maxpeakcurrent"];   // Max peak current
    amsreq.str9 = db_basic.data["torquesensortype"]; // torque sensor type
    amsreq.str10 = db_basic.data["maxdutycycle"];    // Max duty cycle
    amsreq.str11 = db_basic.data["maxtorque"];       // Max torque
    amsreq.str12 = db_basic.data["pwmfreq"];         // PWM frequency
    amsreq.str13 = db_basic.data["maxrpm"];          // Max rpm
    amsreq.str14 = db_basic.data["maxslope"];        // Max slope
    amsreq.str15 = db_basic.data["minbusvolt"];      // Min bus voltage
    amsreq.str16 = db_basic.data["maxbusvolt"];      // Max bus voltage
    amsreq.str17 = db_basic.data["startdutycycle"];  // Start duty cycle
    amsreq.str18 = db_basic.data["gearboxratio"];    // Gear box ratio
    amsreq.str19 = db_basic.data["startinp"];        // Start input source
    amsreq.str20 = db_basic.data["revinp"];          // Reverse ipnut source
    amsreq.str21 = db_basic.data["revrpm"];          // Reverse rpm
    amsreq.str22 = db_basic.data["revslope"];        // Reverse slope
    amsreq.str23 = db_basic.data["revmaxcurrent"];   // Reverse max current
    amsreq.str24 = db_basic.data["revmaxtorque"];    // Reverse max torque
    amsreq.str25 = db_basic.data["erroridletime"];   // Error idle time
    amsreq.str26 = db_basic.data["backlash"];        // Bachlash
    amsreq.str27 = db_basic.data["pgain"];           // Proportional gain
    amsreq.str28 = db_basic.data["igain"];           // Integral gain
    amsreq.str29 = db_basic.data["encoder"];         // Encoder
    // New
    // amsreq.str30 = db_basic.data["mintorque"];         // Min Torque
    // amsreq.str31 = db_basic.data["minrpm"];            // Min RPM
    // amsreq.str32 = db_basic.data["revminrpm"];         // Reverse Min RPM
    // amsreq.str33 = db_basic.data["dmsswver"];          // DmsSWVer
    // amsreq.str34 = db_basic.data["dmscoreid"];         // DmsCoreID
    // amsreq.str35 = db_basic.data["dmssernr"];          // DmsSerNr
    // amsreq.str36 = db_basic.data["led"];               // Led
    // amsreq.str37 = db_basic.data["lever_sensitivity"]; // Lever Sensitivity
    // amsreq.str38 = db_basic.data["push_sensitivity"];  // Push Sensitivity
    // amsreq.str39 = db_basic.data["motswver"];          // MotSWVer
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CheckUiRequestCmd(std::string reqest_string)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string reqest_string
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CheckUiRequestCmd(std::string reqest_string)
{
    // Set AMS bulletin.
    if (ams->SetAmsBulletin(reqest_string) == false)
    {
        #if defined(_DEBUG_MODE_)
        std::cout << "CheckUiRequestCmd = error!! " << std::endl;
        #endif
        return false;
    }
    else
    {
        bulletin->uisockrevcmd = ams->GetAmsCmd(reqest_string);
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetUiRequestCmd()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetUiRequestCmd()
{
    return bulletin->uisockrevcmd;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetMcbPortName(std::string com_name)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string com_name
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsManager::SetMcbPortName(std::string com_name)
{
    comport_name = com_name;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetEmmcDatabasePath(std::string Path)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsManager::SetEmmcDatabasePath(std::string Path)
{
    db_emmc_Path = Path;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SetRamdiskDatabasePath(std::string Path)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsManager::SetRamdiskDatabasePath(std::string Path)
{
    db_ramdisk_Path = Path;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsManager::SetRamdiskTxtPath(std::string Path)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsManager::SetRamdiskTxtPath(std::string Path)
{
    txt_ramdisk_Path = Path;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsManager::SetScrewStatusDatabase(std::string Path)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SetScrewStatusDatabase(std::string Path)
{
    db_screw_ramdisk_Path = Path;
    //
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/25
 *
 *  @fn      GtcsManager::CreatScrewStatusDatabase(std::string Path)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CreatScrewStatusDatabase()
{
    GtcsScrewStatusDatabase screwstatusdatabase(db_screw_ramdisk_Path);
    #if defined(_DEBUG_MODE_)
    std::cout << "db_screw_ramdisk_Path = " << db_screw_ramdisk_Path <<std::endl;
    #endif
    screwstatusdatabase.CreatScrewStatusDatabaseTable();
    // InsertRealTimeActuralValueToDatabase(bulletin->AmsBulletin.DATA300Struct);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetGtcsTcpSocketServerIP()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string GtcsManager::GetGtcsTcpSocketServerIP()
{
    return bulletin->TcpServer.GetIpAddress();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::GetGtcsTcpSocketServerPort()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsManager::GetGtcsTcpSocketServerPort()
{
    return bulletin->TcpServer.GetPort();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::StartGtcsTcpSocketServerThread(std::string ip ,int port)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string ip
 *
 *  @param   int port
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::StartGtcsTcpSocketServerThread(std::string ip, int port)
{
    bulletin->TcpServer.SetIpAddress(ip);
    bulletin->TcpServer.SetPortNum(port);
    thread_tcpserver = std::thread(GtcsTcpSocket::GtcsTcpSocketServerHandler);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::StopAllThread()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::StopAllThread()
{
    thread_tcpserver.join();
    thread_gpio.join();
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::InitialGtcsSystem()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::InitialGtcsSystem()
{
    // Initial MCB Com.
    mcb->InitialMcbComPort(comport_name);
    // Copy database from emmc to ramdisk.
    CopyDatabase(db_ramdisk_Path, db_emmc_Path);
    // Get batchmod from database.
    GetScrewDeviceInfoFormDB(bulletin->ScrewHandler,db_ramdisk_Path);
    // Switch job to normal mode.
    ScrewDriverSwitchJobHandler(bulletin->ScrewHandler.GtcsJob.jobid);
    // Switch sequence list to normal mode.
    ScrewDriverSwitchSequenceHandler(bulletin->ScrewHandler.GtcsJob.jobid,
                        bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id);
    // Polling 5 time to trace ttymxc3.
    for (int index = 0; index < 5; index++)
    {
        // Get MCB polling stataus after polling to MCB .
        mcb->GetMcbPollingStatus(mcb->telegram.ctrl.fasten);
        // Calaulate RT actural value.
        GetRealTimeActuralValue(bulletin->AmsBulletin.DATA300Struct,
                                bulletin->ScrewHandler,
                                mcb->telegram.status.current_status);
        mcb->telegram.status.last_status = mcb->telegram.status.current_status;  // Storage last telegram status.
    }
    bulletin->ScrewHandler.IsEnable = false;

    // Min fsm jump to Check system status.
    SetMainFSM(MAIN_FSM::CHECK_SYS);
    return false;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::CheckGtcsSystem()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::CheckGtcsSystem()
{
    // Initial value.
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    GtcsDatabase db_emmc(db_emmc_Path);
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    GtcsDatabaseBasicInfo basic_emmc;
    GtcsDatabaseBasicInfo basic_ramdisk;
    SetMainFSM(MAIN_FSM::SETTING); // Default MAIN_FSM = SETTING.

    #pragma region check system sequence.
    // Step 1 : Read data from mcb basice parameter.
    if (mcb->ReadBasicParameter(bulletin->McbBulletin.BasicPara) == false)
    {
        std::cout << "Step 1 : Read data from mcb basice parameter error." << std::endl;
        return false;
    }

    // Step 2 : Copy tcs.db from emmc to ramdisk.
    CopyDatabase(db_ramdisk_Path, db_emmc_Path);

    // Step 3 : Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicData(basic_emmc) == false)
    {
        std::cout << "Step 3 : Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }
    else
    {
        basic_ramdisk.SetDataValue(basic_emmc.data);
    }

    // Step 4 : Update MCB basic parameter to ramdisk DB.
    if (UpdateMcbBasicParaToDB(db_ramdisk, basic_ramdisk, bulletin->McbBulletin.BasicPara) == false)
    {
        std::cout << "Step 4 : Update MCB basic parameter to database." << std::endl;
        return false;
    }

    // Step 5 = Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicData(basic_emmc) == false)
    {
        std::cout << "Step 5 = Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }

    // Step 6 : Read basic data from ramdisk database to dbstruct.
    if (db_ramdisk.ReadDatabaseBasicData(basic_ramdisk) == false)
    {
        std::cout << "Step 6 : Read basic data from ramdisk database to dbstruct." << std::endl;
        return false;
    }
    #pragma endregion

    // Step 7 = Jump to selected MAIN_FSM.
    bulletin->checksysok = CompareBasicStruct(basic_emmc, basic_ramdisk);

    #ifdef _DEBUG_MODE_
    std::cout << "bulletin->checksysok = " << bulletin->checksysok << std::endl;
    #endif

    if (bulletin->checksysok == true)
    {
        // Set Gear rate data.
        bulletin->ScrewHandler.u16GearBoxRatio = bulletin->McbBulletin.BasicPara.u16GearBoxRatio;
        bulletin->ScrewHandler.u32StartInp     = bulletin->McbBulletin.BasicPara.u32StartInp;
        // Switch job to normal mode.
        ScrewDriverSwitchJobHandler(bulletin->ScrewHandler.GtcsJob.jobid);

        // Switch sequence list to normal mode.
        ScrewDriverSwitchSequenceHandler(bulletin->ScrewHandler.GtcsJob.jobid,
                            bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id);
                            
        // Get TighteningCounter form database.
        GetScrewDriverTighteningCounter(bulletin->ScrewHandler,
                            bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].tr);
        // Display some informaiton.
        #if defined(_DEBUG_MODE_)
        std::cout << "Gear Ratio = " << std::to_string(bulletin->McbBulletin.BasicPara.u16GearBoxRatio) << std::endl;
        #endif
        SetCurrentScrewDriverTighteningCounter(bulletin->ScrewHandler);
        bulletin->ScrewHandler.IsEnable = true;
        SetMainFSM(MAIN_FSM::READY);
    }
    else
    {
        SetDatabaseBasicParaToReq(bulletin->AmsBulletin.REQ301Struct, basic_ramdisk); // DB_basic -> ams_ans340.
        SetMainFSM(MAIN_FSM::SETTING);
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::RunGtcsSystem()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::RunGtcsSystem()
{
    // Initial object.
    GtcsCtrlTelegramStrcut ctrltelegram;

    // Check settingstatus status.
    if(bulletin->settingstatus == true)
    {
        SetMainFSM(MAIN_FSM::SETTING);  // MAIN_FSM Jump to setting mode.
    }
    else if(bulletin->ScrewHandler.statusnum == (int)LOCKED_STATUS::NG_MCB)
    {
        SetMainFSM(MAIN_FSM::ALARM);    // MAIN_FSM Jump to setting mode.
    }
    else
    {
        // step 1 =  Compare last sequence index and current sequence index.
        if (bulletin->ScrewHandler.currentsequenceindex < bulletin->ScrewHandler.GtcsJob.sequencelist.size())
        {
            // Send data to mcb.
            if (bulletin->ScrewHandler.lastsequenceindex!=bulletin->ScrewHandler.currentsequenceindex)
            {
                bulletin->ScrewHandler.IsEnable = false;
                bulletin->ScrewHandler.currentstatusnum = 2;
                // Get current system status.
                GetCurrentSystemStatusMessage(bulletin->ScrewHandler.currentstatusnum,mcb->telegram.status.current_status.u32ActError);
                // Calaulate RT actural value.
                GetRealTimeActuralValue(bulletin->AmsBulletin.DATA300Struct,bulletin->ScrewHandler,mcb->telegram.status.current_status);
                // Disable Screwdriver.
                DisableMcbScrewStatus();
                // Get MCB program data from database.
                ScrewDriverSwitchSequenceHandler(bulletin->ScrewHandler.GtcsJob.jobid,
                                    bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id);
                // Get TighteningCounter form database.
                GetScrewDriverTighteningCounter(bulletin->ScrewHandler,
                                    bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].tr);
                // Setting list index.
                SetCurrentScrewDriverTighteningCounter(bulletin->ScrewHandler);
                bulletin->ScrewHandler.lastsequenceindex = bulletin->ScrewHandler.currentsequenceindex;
                bulletin->ScrewHandler.currentsequenceid 
                    = bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].seq_id;
                // Setting MCB to fasten status.
                mcb->telegram.status.loosen_status      = false; 
                bulletin->ScrewHandler.currentstatusnum = 0;
                bulletin->ScrewHandler.IsEnable         = true;
                bulletin->ScrewHandler.laststepid       = 1;
            }
        }
        else
        {
            bulletin->ScrewHandler.currentsequenceindex = 0;    // Repeat sequence list operation.
            return false;
        }        

        // step 2 = Config ctrl telegram.
        if (mcb->telegram.status.loosen_status == false)
        {
            ctrltelegram = mcb->telegram.ctrl.fasten;          // Configure fasten ctrl telegram.
            mcb->telegram.ctrl.InitialCtrlFlags(ctrltelegram); 
        }
        else
        {
            #if defined(_DEBUG_MODE_111_)
            ctrltelegram = mcb->telegram.ctrl.fasten;           // Config fasten ctrl telegram.
            #else
            ctrltelegram = mcb->telegram.ctrl.loosen;           // Config loosen ctrl telegram.
            #endif

            mcb->telegram.ctrl.InitialCtrlFlags(ctrltelegram);
            #if defined(_DEBUG_MODE_111_)
            #else
            mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SC_REVERSE); // Reverse
            #endif
        }
        // Set ctrltelegram = SHORT_UW.
        mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SHORT_UVW);
        mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::EN_TIMEOUT_200MS);

        // Enabale | Disable ?
        if(GetGtcsGpioInputPinStatus(bulletin->ScrewHandler.inputstatus,(uint16_t)GTCS_GPIO_IN::DISABLE_IN)==true)
        {
            ;
        }
        else if (bulletin->ScrewHandler.IsEnable == true)
        {
            mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::SC_ENABLE);
        }

        #if defined(_DEBUG_MODE_)
        std::cout << "telegram.ctrl.IsEnable status = " << std::to_string(bulletin->ScrewHandler.IsEnable) << std::endl;
        std::cout << "bulletin->ScrewHandler.currentsequenceindex = " << std::to_string(bulletin->ScrewHandler.currentsequenceindex) << std::endl;
        #endif

        // Step 3 = Polling to MCB & get MCB status.
        if (mcb->GetMcbPollingStatus(ctrltelegram))
        {
            // Get MCB Process excute run time status.
            GetScrewDriverRunTimeStatus(bulletin->ScrewHandler,mcb->telegram.status);

            // Get runtime status message.
            GetScrewDriverRunTimeStatusMessage(bulletin->ScrewHandler,mcb->telegram.status);
            
            // Calaulate tigthtening repeat times.
            if (bulletin->ScrewHandler.statusnum == (int)LOCKED_STATUS::RUNNING)
            {
                bulletin->ScrewHandler.screwcounterlocked = false;
                if (bulletin->ScrewHandler.screwrunning ==false)
                {
                    ClearRamdiskTxtFile();
                    bulletin->ScrewHandler.screwrunning = true;
                    SetScrewDriverFasteningStartClock(bulletin->ScrewHandler.fastenstartclock);
                    SetGtcsGpioOutputPinStatus(bulletin->ScrewHandler.outputstatus,(uint16_t)GTCS_GPIO_OUT::START_OUT,false);                  
                }
                // Calculate fasten time.
                GetScrewDriverFasteningTime(bulletin->ScrewHandler.fasteningtime,bulletin->ScrewHandler.fastenstartclock);                
            }
            else
            {                
                // Onlay to process OK!
                if ((bulletin->ScrewHandler.statusnum == (int)LOCKED_STATUS::OK)&&(bulletin->ScrewHandler.GtcsJob.jobid!=0))
                {
                    SetScrewDriverTighteningCounter(bulletin->ScrewHandler);
                }
                // Insert real time data to database. 
                if (bulletin->ScrewHandler.screwrunning ==true)
                {
                    InsertRealTimeActuralValueToDatabase(bulletin->AmsBulletin.DATA300Struct);
                    // Calaulate RT actural value.
                    GetRealTimeActuralValue(bulletin->AmsBulletin.DATA300Struct,bulletin->ScrewHandler,mcb->telegram.status.current_status);
                    // Write data to ramdisk
                    WriteRealTimeActuralValueToRamdisk(bulletin->AmsBulletin.DATA300Struct);
                    bulletin->ScrewHandler.screwrunning = false;
                    SetGtcsGpioOutputPinStatus(bulletin->ScrewHandler.outputstatus,(uint16_t)GTCS_GPIO_OUT::START_OUT,true);
                }
            }

            // Calaulate RT actural value.
            GetRealTimeActuralValue(bulletin->AmsBulletin.DATA300Struct,bulletin->ScrewHandler,mcb->telegram.status.current_status);
            
            // Write RealTime Actural Value to ramdisk.
            if (bulletin->ScrewHandler.screwrunning ==true)
            {
                WriteRealTimeActuralValueToRamdisk(bulletin->AmsBulletin.DATA300Struct);
            }
            
            // Switch to next sequence index.
            if(CheckScrewDriverCountingFinished(bulletin->ScrewHandler) == true)
            {
                SetScrewDriverNextSeqindex(bulletin->ScrewHandler);             // 
            }
        }
        // Step 4 = Package system status to bulletin.
        mcb->telegram.status.last_status = mcb->telegram.status.current_status; //
    }
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::SettingGtcsSystem()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::SettingGtcsSystem()
{
    if (bulletin->settingstatus == true)
    {
        if (CheckUiSettingFSM(ams->GetAmsCmdNum(bulletin->uisockrevcmd)) == true)
        {
            bulletin->settingstatus = false;
            if (bulletin->checksysok == true)
            {
                SetMainFSM(MAIN_FSM::READY);
            }
        }
    }
    else
    {;}
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Thread sleep 1s.
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsManager::ClearGtcsSystemAlarm()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsManager::ClearGtcsSystemAlarm()
{
    // Initial object.
    GtcsCtrlTelegramStrcut ctrltelegram;

    // Check settingstatus status.
    if(bulletin->settingstatus == true)
    {
        SetMainFSM(MAIN_FSM::SETTING); // MAIN_FSM Jump to setting mode.
    }
    else
    {
        // Check alarm message.
        if (mcb->telegram.status.loosen_status == false)
        {
            ctrltelegram = mcb->telegram.ctrl.fasten;       // Configure fasten ctrl telegram.
        }
        else
        {
            ctrltelegram = mcb->telegram.ctrl.loosen;       // Config loosen ctrl telegram.
        }        
        // Check start signal.
        if (GetSystemErrorStatus(mcb->telegram.status.current_status.u16Statusflags)==true)
        {
            if (bulletin->ScrewHandler.GtcsJob.sequencelist[bulletin->ScrewHandler.currentsequenceindex].ng_stop==0)
            {
                if (GetStartSignalStatus(mcb->telegram.status.current_status.u16TMDFlags)==true)
                {
                    mcb->telegram.ctrl.InitialCtrlFlags(ctrltelegram);
                    mcb->telegram.ctrl.SetCtrlFlags(ctrltelegram, CTRL_FLAGS_IDX::ERR_ACK);  // Ack mcb error. 
                }
                // Polling Ack signal to mcb.
                if(mcb->GetMcbPollingStatus(ctrltelegram))
                {
                    #if defined(_DEBUG_MODE_)
                    std::cout << "Polling to clear Gtcs system alarm statua!" <<std::endl;
                    #endif
                }         
                else
                {
                    #if defined(_DEBUG_MODE_)
                    std::cout << "Error to polling Gtcs system alarm statua!" <<std::endl;
                    #endif
                }  
            }
            else
            {
                #if defined(_DEBUG_MODE_)
                std::cout << "Gtcs system alarm statua NG stop = 1 !" <<std::endl;
                #endif
            }
        }
        else
        {
            bulletin->ScrewHandler.statusnum = (int)LOCKED_STATUS::NG_OK;
            SetMainFSM(MAIN_FSM::READY); // MAIN_FSM Jump to ready mode.    
        }

        #if defined(_DEBUG_MODE_)
        std::cout << "Erro to clear Gtcs system alarm statua!" <<std::endl;
        #endif
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Thread sleep 1s.
    return true;
}
