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
#include "../include/Manager.h"

#pragma region manager
// Constructor.
Manager::Manager(/* args */)
{}
// Distructor.
Manager::~Manager()
{}
// Get main FSM
int Manager::GetMainFSM()
{
    return MainFSM;
}
// Set Miain fsm.
void Manager::SetMainFSM(int main_fsm)
{
    MainFSM = main_fsm;
}
#pragma endregion

#pragma region NTCS manager
NtcsManager::NtcsManager()
{}
NtcsManager::~NtcsManager()
{}
#pragma endregion

#pragma region GTCS manager
// Constructor
GtcsManager::GtcsManager()
{}
// Distructor
GtcsManager::~GtcsManager()
{}
// Get MCB realy time status string.
std::string GtcsManager::GetMcbRtStatusString(MCB_RT_STATUS status)
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
std::string GtcsManager::GetToolRunTimeStatus()
{
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
bool GtcsManager::ConvertReadlTimeActuralValue()
{
    // GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // GtcsMcbComm *mcb = GtcsMcbComm::GetInstance();
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
    double acttorque  = ((double)mcb_status->u16ActTorque/1862) * toolmaxtorque;   // Calculate Act torque.
    double angle      = (double)mcb_status->u32Angle/(gear*200) * 360;             // Calculate revalution.
    double maxtorque  = ((double)mcb_status->u16MaxTorque/1862) * toolmaxtorque;   // Calculate Act torque.
    double revolution = (double)mcb_status->u32Revolutions/(gear * 200) * 360;     // Calculate revalution.
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
    // data300->toolstatus    = std::to_string(0); // str28:Tool status

    return true;
}
bool GtcsManager::ConvertAmsBasicToMcbStruct(AmsCMD340Struct &amscmd,McbID2Struct &mcb_basic)
{
    //
    mcb_basic.s16MinTemp
        = (uint16_t)(std::stof(amscmd.str5)*10);// SID = 1,Minimal Temperature of the motor and the motorcontroller.
                                                // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
    mcb_basic.s16MaxTemp
        = (uint16_t)(std::stof(amscmd.str6)*10);// SID = 2,Maximal Temperature of the motor and the motorcontroller.
                                                // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
    mcb_basic.u16MaxCurrent
        = (uint16_t)std::stoi(amscmd.str7);     // SID = 3,This is the absolute maximum motor current value.
                                                // There are also maximum current values for the specific process steps
                                                // (see….), but this parameter is the limit. Unit is [mA].
    mcb_basic.u16MaxPeakCurrent
        = (uint16_t)std::stoi(amscmd.str8);     // SID = 4,This value controls the threshold of the peak current comparator.
                                                // The peak current detection is built in for the protection of the device
                                                // when a latch up situation occurs. Unit is [A].
    mcb_basic.u16TorqueSensorType
        = (uint16_t)std::stoi(amscmd.str9);    // SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
    mcb_basic.u16MaxDutyCycle
        = (uint16_t)(std::stof(amscmd.str10)*10);// SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]

    //mcb_basic.u16MaxTorque
    //   = (uint16_t)std::stoi(amscmd.str11);   // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
    mcb_basic.u16MaxTorque = 1862;   // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"

    mcb_basic.u16PWMFreq
        = (uint16_t)std::stoi(amscmd.str12);   // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
    mcb_basic.u16MaxRPM
        = (uint16_t)std::stoi(amscmd.str13);    // SID = 9,This is the absolute maximum motor rpm value.
                                                // There are also rpm values for the specific process steps (see….) ,
                                                // but this parameter is the limit. Unit is [rpm].
    mcb_basic.u16MaxSlope
        = (uint16_t)std::stoi(amscmd.str14);    // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                                // There are also rpm slope values for the specific process steps (see….) ,
                                                // but this parameter is the limit. Unit is [rpm/s].
    mcb_basic.u16MinBusVolt
        = (uint16_t)(std::stof(amscmd.str15)*10);// SID = 11,Minimal Bus Voltage of the Power Supply.
                                                // Underneath this Voltage the tool doesn’t work.
                                                // Unit is [0,1V] (600 = 60V).
    mcb_basic.u16MaxBusVolt
        = (uint16_t)(std::stof(amscmd.str16)*10);    // SID = 12,Maximal Bus Voltage of the Power Supply.
                                                // Above this Voltage the tool doesn’t work. Unit is [0,1V].
    mcb_basic.u16StartDutyCycle
        = (uint16_t)(std::stof(amscmd.str17)*10);    // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].(20 = 10%)

    mcb_basic.u16GearBoxRatio
        = (uint16_t)(std::stof(amscmd.str18)*100);    // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    mcb_basic.u32StartInp
        = (uint16_t)std::stoi(amscmd.str19);    // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    mcb_basic.u32RevInp
        = (uint16_t)std::stoi(amscmd.str20);   // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
    mcb_basic.u16RevRpm
        = (uint16_t)std::stoi(amscmd.str21);   // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
    mcb_basic.u16RevSlope
        = (uint16_t)std::stoi(amscmd.str22);   // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
    mcb_basic.u16RevMaxCurrent
        = (uint16_t)std::stoi(amscmd.str23);   // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."

    //mcb_basic.u16RevMaxTorque
    // = (uint16_t)std::stoi(amscmd.str24);   // SID = 20,"Maximum Torque of reverse drive.
    //                                         // Value is 0- 1862 (max TMD Raw Value)"
    mcb_basic.u16RevMaxTorque = 1862;

    mcb_basic.u16ErrorIdleTime
        = (uint16_t)std::stoi(amscmd.str25);   // SID = 21,"Idle time of the motor controller after a Error condition.Unit is [ms]."
    mcb_basic.u16BackLash
        = (uint16_t)std::stoi(amscmd.str26);   // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or
                                                // Hallsensors are used for angle Positioning. Unit is [Increments]."
    mcb_basic.u16PGain
        = (uint16_t)std::stoi(amscmd.str27);   // SID = 23,Proportional Gain for the RPM Regulator.
    mcb_basic.u16IGain
        = (uint16_t)std::stoi(amscmd.str28);   // SID = 24,Integral Gain for the RPM Regulator.
    mcb_basic.u16Encoder
        = (uint16_t)std::stoi(amscmd.str29);   // SID = 25,"0 = No Encoder (positioning with Hallsensors).

    return true;
}
bool GtcsManager::SetAmsCmdBaiscParaToAns(AmsANS340Struct &amsans,AmsCMD340Struct &amscmd)   // CMD340->ANS340
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
    // New
    amsans.str30 = amscmd.str30;
    amsans.str31 = amscmd.str31;
    amsans.str32 = amscmd.str32;
    amsans.str33 = amscmd.str33;
    amsans.str34 = amscmd.str34;
    amsans.str35 = amscmd.str35;
    amsans.str36 = amscmd.str36;
    amsans.str37 = amscmd.str37;
    amsans.str38 = amscmd.str38;
    amsans.str39 = amscmd.str39;
}
bool GtcsManager::SetSystemBasicParameter(AmsCMD340Struct &amscmd,GtcsDatabaseBasicStruct &db_basic,McbID2Struct &mcb_basic)
{
    // Initial value.
    GtcsDatabase db_emmc(db_emmc_Path);
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    GtcsDatabaseBasicInfo basic_emmc;
    GtcsDatabaseBasicInfo basic_ramdisk;

    // step 1 : Convert AMS cmd340 to mcb struct and update MCB basic paramater.
    ConvertAmsBasicToMcbStruct(amscmd,mcb_basic);
    if (mcb->WriteBasicParameter(mcb_basic)==false)
    {
        return false;
    }

    // Step 2 : Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicTable(basic_emmc)==false)
    {
        std::cout << "Step 2 : Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }

    // step 3 : Update Mcb struct to emmc basic datatbase.
    if (UpdateMcbBasicParaToDB(db_emmc,basic_emmc,bulletin->McbBulletin.BasicPara)==false)
    {
        std::cout << "Step 3 : Update MCB basic parameter to database." << std::endl;
        return false;
    }

    // step 4 : Cpoy emmc database to ramdisk.
    CopyDatabase(db_ramdisk_Path,db_emmc_Path);

    // step 5 : Set ANS340
    SetAmsCmdBaiscParaToAns(bulletin->AmsBulletin.ANS340Struct,bulletin->AmsBulletin.CMD340Struct);

    return true;
}
// Check Ui Setting FSM.
bool GtcsManager::CheckUiSettingFSM(int uicmd)
{
    switch (uicmd)
    {
    case AMSCMD::CMD302:
        #pragma region cmd302 sequence
        if(bulletin->AmsBulletin.CMD302Struct.str5 == "0")
        {
            mcb->telegram.ctrl.IsEnable = true;
            bulletin->AmsBulletin.ANS302Struct.str5 = "0";
            bulletin->AmsBulletin.DATA300Struct.toolstatus = "0";
        }
        else if (bulletin->AmsBulletin.CMD302Struct.str5 == "1")
        {
            mcb->telegram.ctrl.IsEnable = false;
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
        #pragma endregion
        break;
    case AMSCMD::CMD340:
        if(SetSystemBasicParameter(bulletin->AmsBulletin.CMD340Struct,bulletin->DbBulletin.basic,bulletin->McbBulletin.BasicPara)==false)
        {
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}
// Get Ui Cmd Response.
std::string GtcsManager::GetUiCmdResponse(std::string uicmd_string)
{
    int uiresponsecmd = 0;

    if(uicmd_string == "REQ300") // Polling
    {
        if(bulletin->checksysok==true)
        {
            uiresponsecmd = AMSCMD::DATA300;
        }
        else
        {
            uiresponsecmd = AMSCMD::REQ301;
        }
    }
    else                          // Setting
    {
        std::cout << "ams->GetAmsCmdNum(uicmd_string) = "  << std::to_string(ams->GetAmsCmdNum(uicmd_string))<<std::endl;
        uiresponsecmd = ams->GetAmsSymmetryCmdNumver(ams->GetAmsCmdNum(uicmd_string));
        std::cout << "std::to_string(uiresponsecmd) = " << std::to_string(uiresponsecmd) << std::endl;
    }
    return ams->GetAmsBulletin(uiresponsecmd);
}
// Copy database.
bool GtcsManager::CopyDatabase(std::string destination ,std::string source)
{
    std::string systemcmd = "sudo cp " + source + " " + destination;
    system(systemcmd.c_str());
    systemcmd = "sudo chmod -R 777 " + destination;
    system(systemcmd.c_str());
    return true;
}
//
bool GtcsManager::UpdateMcbBasicParaToDB(GtcsDatabase &db,GtcsDatabaseBasicInfo &db_basic,McbID2Struct &mcb_basic)
{
    // Update data.
    db_basic.data["mintemp"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MinTemp/10,1);      // Min temperature       (REAL)
    db_basic.data["maxtemp"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MaxTemp/10,1);      // Max temperature       (REAL)
    db_basic.data["maxcurrent"]       = std::to_string(mcb_basic.u16MaxCurrent);      // Max current           (REAL)
    db_basic.data["maxpeakcurrent"]   = std::to_string(mcb_basic.u16MaxPeakCurrent);  // Max peak current      (INTEGER)
    db_basic.data["torquesensortype"] = std::to_string(mcb_basic.u16TorqueSensorType);// torque sensor type    (INTEGER)
    db_basic.data["maxdutycycle"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MaxDutyCycle/10,1);  // Max duty cycle        (REAL)
    db_basic.data["maxtorque"]        = db_basic.data["maxtorque"];                                        // Max torque            (REAL)
    db_basic.data["pwmfreq"]          = std::to_string(mcb_basic.u16PWMFreq);         // PWM frequency         (INTEGER)
    db_basic.data["maxrpm"]           = std::to_string(mcb_basic.u16MaxRPM);          // Max rpm               (INTEGER)
    db_basic.data["maxslope"]         = std::to_string(mcb_basic.u16MaxSlope);        // Max slope             (INTEGER)
    db_basic.data["minbusvolt"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MinBusVolt/10,1);    // Min bus voltage       (REAL)
    db_basic.data["maxbusvolt"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16MaxBusVolt/10,1);    // Max bus voltage       (REAL)
    db_basic.data["startdutycycle"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16StartDutyCycle/10,1);  // Start duty cycle      (REAL)
    db_basic.data["gearboxratio"]
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.u16GearBoxRatio/100,2); // Gear box ratio        (REAL)
    db_basic.data["startinp"]         = std::to_string(mcb_basic.u32StartInp);        // Start input source    (INTEGER)
    db_basic.data["revinp"]           = std::to_string(mcb_basic.u32RevInp);          // Reverse ipnut source  (INTEGER)
    db_basic.data["revrpm"]           = std::to_string(mcb_basic.u16RevRpm);          // Reverse rpm           (INTEGER)
    db_basic.data["revslope"]         = std::to_string(mcb_basic.u16RevSlope);        // Reverse slope         (INTEGER)
    db_basic.data["revmaxcurrent"]    = std::to_string(mcb_basic.u16RevMaxCurrent);   // Reverse max current   (INTEGER)
    db_basic.data["revmaxtorque"]     = db_basic.data["revmaxtorque"];                                         // Reverse max torque    (REAL)
    db_basic.data["erroridletime"]    = std::to_string(mcb_basic.u16ErrorIdleTime);   // Error idle time       (INTEGER)
    db_basic.data["backlash"]         = std::to_string(mcb_basic.u16BackLash);        // Bachlash              (INTEGER)
    db_basic.data["pgain"]            = std::to_string(mcb_basic.u16PGain);           // Proportional gain     (INTEGER)
    db_basic.data["igain"]            = std::to_string(mcb_basic.u16IGain);           // Integral gain         (INTEGER)
    db_basic.data["encoder"]          = std::to_string(mcb_basic.u16Encoder);         // Encoder               (INTEGER)

    // Update database basic table.
    db.UpdateDatabaseBasicTable(db_basic);
    return true;
}
// Compare Basic Struct.
bool GtcsManager::CompareBasicStruct(GtcsDatabaseBasicInfo &emmc,GtcsDatabaseBasicInfo &ramdisk)
{
    bool result = true;

    for (int i = 0; i < emmc.columnnames.size(); i++)
    {
        if (emmc.data[emmc.columnnames[i]] != ramdisk.data[ramdisk.columnnames[i]])
        {
            result = false;
        }
        std::cout << "check key = " << emmc.columnnames[i] ;
        std::cout << " emmc_value = " << emmc.data[emmc.columnnames[i]];
        std::cout << " ramdisk_value = " << ramdisk.data[ramdisk.columnnames[i]] << std::endl;
    }
    return result;
}
// Set AMS Bulletin Basic Parameter.
bool GtcsManager::SetDatabaseBasicParaToAns(AmsANS340Struct &amsans,GtcsDatabaseBasicInfo &db_basic)
{
    // GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // Setting REQ301 value.
    amsans.str5  = db_basic.data["mintemp"];          // Min temperature
    amsans.str6  = db_basic.data["maxtemp"];          // Max temperature
    amsans.str7  = db_basic.data["maxcurrent"];       // Max current
    amsans.str8  = db_basic.data["maxpeakcurrent"];   // Max peak current
    amsans.str9  = db_basic.data["torquesensortype"]; // torque sensor type
    amsans.str10 = db_basic.data["maxdutycycle"];     // Max duty cycle
    amsans.str11 = db_basic.data["maxtorque"];        // Max torque
    amsans.str12 = db_basic.data["pwmfreq"];          // PWM frequency
    amsans.str13 = db_basic.data["maxrpm"];           // Max rpm
    amsans.str14 = db_basic.data["maxslope"];         // Max slope
    amsans.str15 = db_basic.data["minbusvolt"];       // Min bus voltage
    amsans.str16 = db_basic.data["maxbusvolt"];       // Max bus voltage
    amsans.str17 = db_basic.data["startdutycycle"];   // Start duty cycle
    amsans.str18 = db_basic.data["gearboxratio"];     // Gear box ratio
    amsans.str19 = db_basic.data["startinp"];         // Start input source
    amsans.str20 = db_basic.data["revinp"];           // Reverse ipnut source
    amsans.str21 = db_basic.data["revrpm"];           // Reverse rpm
    amsans.str22 = db_basic.data["revslope"];         // Reverse slope
    amsans.str23 = db_basic.data["revmaxcurrent"];    // Reverse max current
    amsans.str24 = db_basic.data["revmaxtorque"];     // Reverse max torque
    amsans.str25 = db_basic.data["erroridletime"];    // Error idle time
    amsans.str26 = db_basic.data["backlash"];         // Bachlash
    amsans.str27 = db_basic.data["pgain"];            // Proportional gain
    amsans.str28 = db_basic.data["igain"];            // Integral gain
    amsans.str29 = db_basic.data["encoder"];          // Encoder
    // New
    amsans.str30 = db_basic.data["mintorque"];        // Min Torque
    amsans.str31 = db_basic.data["minrpm"];           // Min RPM
    amsans.str32 = db_basic.data["revminrpm"];        // Reverse Min RPM
    amsans.str33 = db_basic.data["dmsswver"];         // DmsSWVer
    amsans.str34 = db_basic.data["dmscoreid"];        // DmsCoreID
    amsans.str35 = db_basic.data["dmssernr"];         // DmsSerNr
    amsans.str36 = db_basic.data["led"];              // Led
    amsans.str37 = db_basic.data["lever_sensitivity"];// Lever Sensitivity
    amsans.str38 = db_basic.data["push_sensitivity"]; // Push Sensitivity
    amsans.str39 = db_basic.data["motswver"];         // MotSWVer
    return true;
}
// Check Request Status from UI.
std::string GtcsManager::CheckUiCmdRequest(std::string reqest_string)
{
    return ams->SetAmsBulletin(reqest_string);
}
// Set Initial value.
void GtcsManager::SetMcbPortName(std::string com_name)
{
    comport_name = com_name;
}
void GtcsManager::SetEmmcDatabasePath(std::string Path)
{
    db_emmc_Path = Path;
}
void GtcsManager::SetRamdiskDatabasePath(std::string Path)
{
    db_ramdisk_Path = Path;
}
// Initial Gtcs System.
bool GtcsManager::InitialGtcsSystem()
{
    // Initial MCB Com.
    mcb->InitialMcbComPort(comport_name);
    for(int index=0;index<5;index++)
    {
        mcb->NormalPollingToMcb();
        ConvertReadlTimeActuralValue();
    }
    mcb->telegram.ctrl.IsEnable = false;
    // Min fsm jump to Check system status.
    SetMainFSM(MAIN_FSM::CHECK_SYS);
    return false;
}
// Check Gtcs System.
bool GtcsManager::CheckGtcsSystem()
{
    // Initial value.
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    GtcsDatabase db_emmc(db_emmc_Path);
    GtcsDatabase db_ramdisk(db_ramdisk_Path);
    GtcsDatabaseBasicInfo basic_emmc;
    GtcsDatabaseBasicInfo basic_ramdisk;

    // Step 1 : Read data from mcb basice parameter.
    if (mcb->ReadBasicParameter(bulletin->McbBulletin.BasicPara) == false)
    {
        std::cout << "Step 1 : Read data from mcb basice parameter." << std::endl;
        return false;
    }

    // Step 2 : Copy tcs.db from emmc to ramdisk.
    CopyDatabase(db_ramdisk_Path,db_emmc_Path);

    // Step 3 : Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicTable(basic_emmc)==false)
    {
        std::cout << "Step 3 : Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }
    else
    {
        basic_ramdisk.SetDataValue(basic_emmc.data);
    }

    // Step 4 : Update MCB basic parameter to database.
    if (UpdateMcbBasicParaToDB(db_ramdisk,basic_ramdisk,bulletin->McbBulletin.BasicPara)==false)
    {
        std::cout << "Step 4 : Update MCB basic parameter to database." << std::endl;
        return false;
    }

    // Step 5 = Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabaseBasicTable(basic_emmc)==false)
    {
        std::cout << "Step 5 = Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }

    // Step 6 : Read basic data from ramdisk database to dbstruct.
    if (db_ramdisk.ReadDatabaseBasicTable(basic_ramdisk)==false)
    {
        std::cout << "Step 6 : Read basic data from ramdisk database to dbstruct." << std::endl;
        return false;
    }

    // Step 7 = Jump to selected MAIN_FSM.
    bulletin->checksysok = CompareBasicStruct(basic_emmc,basic_ramdisk);
    std::cout << " bulletin->checksysok = " << bulletin->checksysok << std::endl;
    if (bulletin->checksysok == true)
    {
        // Display some informaiton.
        std::cout <<"Gear Ratio = " << std::to_string(bulletin->McbBulletin.BasicPara.u16GearBoxRatio)<<std::endl;
        mcb->telegram.ctrl.IsEnable = true;
        SetMainFSM(MAIN_FSM::READY);
    }
    else
    {
        SetDatabaseBasicParaToAns(bulletin->AmsBulletin.ANS340Struct,basic_ramdisk); // DB_basic -> ams_ans340.
        SetMainFSM(MAIN_FSM::SETTING);
    }

    return true;
}
// Initial Gtcs System.
bool GtcsManager::RunGtcsSystem()
{
    if (bulletin->uisetting==false)
    {
        mcb->NormalPollingToMcb();
        ConvertReadlTimeActuralValue();
    }
    else
    {
        SetMainFSM(MAIN_FSM::SETTING); // MAIN_FSM Jump to setting mode.
    }
    return true;
}
// Clear Gtcs System Alarm.
bool GtcsManager::ClearGtcsSystemAlarm()
{
    return true;
}
// Setting Gtcs System.
bool GtcsManager::SettingGtcsSystem()
{
    if (bulletin->uisetting==true)
    {
        if (CheckUiSettingFSM(ams->GetAmsCmdNum(bulletin->uisockrevcmd))==true)
        {
            bulletin->uisetting = false;
            SetMainFSM(MAIN_FSM::READY);
        }
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Thread sleep 1s.
    }
    return true;
}