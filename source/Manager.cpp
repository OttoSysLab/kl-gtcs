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
int GtcsManager::ConvertActuralData300()
{
    int result = 0;
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
    // data300->toolstatus    = std::to_string(0); // str28:Tool status
    return result;
}
// Check Ui Setting FSM.
int GtcsManager::CheckUiSettingFSM(int uicmd)
{
    int result = 0;
    switch (uicmd)
    {
    case AMSCMD::CMD302:
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
        result = 1;
        break;
    case AMSCMD::CMD340:
        ams->GetAmsBulletin(AMSCMD::ANS340);
        result = 1;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
// Get Ui Cmd Response.
std::string GtcsManager::GetUiCmdResponse(std::string uicmd_string)
{
    int uiresponsecmd = 0;
    if(uicmd_string == "REQ300")
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
    else{
        uiresponsecmd = ams->GetAmsSymmetryCmdNumver(ams->GetAmsCmdNum(uicmd_string));
    }
    return ams->GetAmsBulletin(uiresponsecmd);
}
// Initial Gtcs System.
int GtcsManager::InitialGtcsSystem()
{
    int result = 0;
    // Initial MCB Com.
    mcb->InitialMcbComPort("/dev/ttymxc3");
    for(int index=0;index<5;index++)
    {
        mcb->CheckMcbFSM((int)MCB_FSM::NORMAL_POLLING);
        ConvertActuralData300();
    }
    mcb->telegram.ctrl.IsEnable = false;

    SetMainFSM(MAIN_FSM::CHECK_SYS);
    return result;
}
// Initial Gtcs System.
int GtcsManager::CheckGtcsSystem()
{
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    int result = 0;
    // Step 1 = Read data from mcb basice parameter.
    if (mcb->CheckMcbFSM((int)MCB_FSM::READ_MCB_BASIC)!=0)
    {
        std::uint16_t *ptr = &bulletin->McbBulletin.BasicPara.s16MinTemp;
        std::cout << std::to_string(bulletin->McbBulletin.BasicPara.s16MaxTemp) <<std::endl;
    }
    // Step 2 = Copy tcs.db to ramdisk.
    std::string db_emmc_Path = "/var/www/html/database/tcs.db";   // Initial database path.
    std::string db_ramdisk_Path = "/mnt/ramdisk/tcs.db";          // Initial database path.
    std::string systemcmd = "sudo cp " + db_emmc_Path + " " + db_ramdisk_Path;
    system(systemcmd.c_str());
    systemcmd = "sudo chmod -R 777 " + db_ramdisk_Path;
    system(systemcmd.c_str());

    // Step 3 = Write basic parameter to tcs.db which is in ramdisk.
    GtcsDatabase database(db_ramdisk_Path,db_emmc_Path);
    // database.CheckDatabaseFSM((int)DB_FSM::R_RAM_BAIIC_PARA);
    database.ReadFromSqliteDatabase(database.db_ramdisk,"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp);    
    std::cout << database.db_ramdisk.GetDatabasePath() << std::endl;
    std::cout << database.db_emmc.GetDatabasePath() << std::endl;
    GtcsDatabaseBasicInfo ram_basic;
    ram_basic.SetDataValue(&bulletin->DbBulletin.basic.mintemp);  // Convert data to basic parameter buffer.
    
    // Sort out basic data.
    ram_basic.data["mintemp"]          = std::to_string(bulletin->McbBulletin.BasicPara.s16MinTemp);         // Min temperature       (REAL)
    ram_basic.data["maxtemp"]          = std::to_string(bulletin->McbBulletin.BasicPara.s16MaxTemp);         // Max temperature       (REAL) 
    ram_basic.data["maxcurrent"]       = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxCurrent);      // Max current           (REAL) 
    ram_basic.data["maxpeakcurrent"]   = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxPeakCurrent);  // Max peak current      (INTEGER) 
    ram_basic.data["torquesensortype"] = std::to_string(bulletin->McbBulletin.BasicPara.u16TorqueSensorType);// torque sensor type    (INTEGER) 
    ram_basic.data["maxdutycycle"]     = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxDutyCycle);    // Max duty cycle        (REAL) 
    // ram_basic.data["maxtorque"]        = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxTorque);       // Max torque            (REAL) 
    ram_basic.data["maxtorque"]        = ram_basic.data["maxtorque"];                                            // Max torque            (REAL) 
    ram_basic.data["pwmfreq"]          = std::to_string(bulletin->McbBulletin.BasicPara.u16PWMFreq);         // PWM frequency         (INTEGER) 
    ram_basic.data["maxrpm"]           = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxRPM);          // Max rpm               (INTEGER) 
    ram_basic.data["maxslope"]         = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxSlope);        // Max slope             (INTEGER) 
    ram_basic.data["minbusvolt"]       = std::to_string(bulletin->McbBulletin.BasicPara.u16MinBusVolt);      // Min bus voltage       (REAL) 
    ram_basic.data["maxbusvolt"]       = std::to_string(bulletin->McbBulletin.BasicPara.u16MaxBusVolt);      // Max bus voltage       (REAL) 
    ram_basic.data["startdutycycle"]   = std::to_string(bulletin->McbBulletin.BasicPara.u16StartDutyCycle);  // Start duty cycle      (REAL) 
    ram_basic.data["gearboxratio"]     = std::to_string((float)bulletin->McbBulletin.BasicPara.u16GearBoxRatio/100);    // Gear box ratio        (REAL) 
    ram_basic.data["startinp"]         = std::to_string(bulletin->McbBulletin.BasicPara.u32StartInp);        // Start input source    (INTEGER) 
    ram_basic.data["revinp"]           = std::to_string(bulletin->McbBulletin.BasicPara.u32RevInp);          // Reverse ipnut source  (INTEGER) 
    ram_basic.data["revrpm"]           = std::to_string(bulletin->McbBulletin.BasicPara.u16RevRpm);          // Reverse rpm           (INTEGER) 
    ram_basic.data["revslope"]         = std::to_string(bulletin->McbBulletin.BasicPara.u16RevSlope);        // Reverse slope         (INTEGER) 
    ram_basic.data["revmaxcurrent"]    = std::to_string(bulletin->McbBulletin.BasicPara.u16RevMaxCurrent);   // Reverse max current   (INTEGER) 
    // ram_basic.data["revmaxtorque"]     = std::to_string(bulletin->McbBulletin.BasicPara.u16RevMaxTorque);    // Reverse max torque    (REAL) 
    ram_basic.data["revmaxtorque"]     = ram_basic.data["revmaxtorque"];                                         // Reverse max torque    (REAL) 

    ram_basic.data["erroridletime"]    = std::to_string(bulletin->McbBulletin.BasicPara.u16ErrorIdleTime);   // Error idle time       (INTEGER) 
    ram_basic.data["backlash"]         = std::to_string(bulletin->McbBulletin.BasicPara.u16BackLash);        // Bachlash              (INTEGER) 
    ram_basic.data["pgain"]            = std::to_string(bulletin->McbBulletin.BasicPara.u16PGain);           // Proportional gain     (INTEGER) 
    ram_basic.data["igain"]            = std::to_string(bulletin->McbBulletin.BasicPara.u16IGain);           // Integral gain         (INTEGER) 
    ram_basic.data["encoder"]          = std::to_string(bulletin->McbBulletin.BasicPara.u16Encoder);         // Encoder               (INTEGER) 

    database.UpdateSqliteDatabase(database.db_ramdisk,"basic",ram_basic.data);    //

    // Step 4 = Compare data bwtweem ramdisk and emmc database basic table.
    GtcsDatabaseBasicInfo emmc_basic;
    database.ReadFromSqliteDatabase(database.db_emmc,"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp);    
    emmc_basic.SetDataValue(&bulletin->DbBulletin.basic.mintemp);
    database.ReadFromSqliteDatabase(database.db_ramdisk,"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp);    
    ram_basic.SetDataValue(&bulletin->DbBulletin.basic.mintemp);
    
    // Step 5 = Jump to selected MAIN_FSM.    
    bulletin->checksysok = true;
    for (int i = 0; i < ram_basic.columnnames.size(); i++)
    {
        if (ram_basic.data[ram_basic.columnnames[i]] != emmc_basic.data[emmc_basic.columnnames[i]])
        {
            bulletin->checksysok = false;
        }
        // std::cout << "check key = " << ram_basic.columnnames[i] ;
        // std::cout << " ram_value = " << ram_basic.data[ram_basic.columnnames[i]];
        // std::cout << " basic_value = " << emmc_basic.data[emmc_basic.columnnames[i]] << std::endl;
    }    
    std::cout << " bulletin->checksysok = " << bulletin->checksysok << std::endl;     
    if (bulletin->checksysok == true)
    {
        // Display some informaiton.
        std::cout <<"Gear Ratio = " << std::to_string(bulletin->McbBulletin.BasicPara.u16GearBoxRatio)<<std::endl;
        SetMainFSM(MAIN_FSM::READY);
    }
    else
    {
        // Setting REQ301 value.
        bulletin->AmsBulletin.REQ301Struct.str5  = ram_basic.data["mintemp"],          // Min temperature 
        bulletin->AmsBulletin.REQ301Struct.str6  = ram_basic.data["maxtemp"],          // Max temperature
        bulletin->AmsBulletin.REQ301Struct.str7  = ram_basic.data["maxcurrent"],       // Max current
        bulletin->AmsBulletin.REQ301Struct.str8  = ram_basic.data["maxpeakcurrent"],   // Max peak current
        bulletin->AmsBulletin.REQ301Struct.str9  = ram_basic.data["torquesensortype"]; // torque sensor type
        bulletin->AmsBulletin.REQ301Struct.str10 = ram_basic.data["maxdutycycle"];     // Max duty cycle
        bulletin->AmsBulletin.REQ301Struct.str11 = ram_basic.data["maxtorque"];        // Max torque
        bulletin->AmsBulletin.REQ301Struct.str12 = ram_basic.data["pwmfreq"];          // PWM frequency 
        bulletin->AmsBulletin.REQ301Struct.str13 = ram_basic.data["maxrpm"];           // Max rpm
        bulletin->AmsBulletin.REQ301Struct.str14 = ram_basic.data["maxslope"];         // Max slope
        bulletin->AmsBulletin.REQ301Struct.str15 = ram_basic.data["minbusvolt"];       // Min bus voltage
        bulletin->AmsBulletin.REQ301Struct.str16 = ram_basic.data["maxbusvolt"];       // Max bus voltage
        bulletin->AmsBulletin.REQ301Struct.str17 = ram_basic.data["startdutycycle"];   // Start duty cycle
        bulletin->AmsBulletin.REQ301Struct.str18 = ram_basic.data["gearboxratio"];     // Gear box ratio
        bulletin->AmsBulletin.REQ301Struct.str19 = ram_basic.data["startinp"];         // Start input source
        bulletin->AmsBulletin.REQ301Struct.str20 = ram_basic.data["revinp"];           // Reverse ipnut source
        bulletin->AmsBulletin.REQ301Struct.str21 = ram_basic.data["revrpm"];           // Reverse rpm
        bulletin->AmsBulletin.REQ301Struct.str22 = ram_basic.data["revslope"];         // Reverse slope
        bulletin->AmsBulletin.REQ301Struct.str23 = ram_basic.data["revmaxcurrent"];    // Reverse max current
        bulletin->AmsBulletin.REQ301Struct.str24 = ram_basic.data["revmaxtorque"];     // Reverse max torque
        bulletin->AmsBulletin.REQ301Struct.str25 = ram_basic.data["erroridletime"];    // Error idle time
        bulletin->AmsBulletin.REQ301Struct.str26 = ram_basic.data["backlash"];         // Bachlash 
        bulletin->AmsBulletin.REQ301Struct.str27 = ram_basic.data["pgain"];            // Proportional gain
        bulletin->AmsBulletin.REQ301Struct.str28 = ram_basic.data["igain"];            // Integral gain
        bulletin->AmsBulletin.REQ301Struct.str29 = ram_basic.data["encoder"];          // Encoder
        // 
        bulletin->AmsBulletin.REQ301Struct.str30 = ram_basic.data["mintorque"];        // Min Torque
        bulletin->AmsBulletin.REQ301Struct.str31 = ram_basic.data["minrpm"];           // Min RPM
        bulletin->AmsBulletin.REQ301Struct.str32 = ram_basic.data["revminrpm"];        // Reverse Min RPM
        bulletin->AmsBulletin.REQ301Struct.str33 = ram_basic.data["dmsswver"];         // DmsSWVer
        bulletin->AmsBulletin.REQ301Struct.str34 = ram_basic.data["dmscoreid"];        // DmsCoreID
        bulletin->AmsBulletin.REQ301Struct.str35 = ram_basic.data["dmssernr"];         // DmsSerNr
        bulletin->AmsBulletin.REQ301Struct.str36 = ram_basic.data["led"];              // Led
        bulletin->AmsBulletin.REQ301Struct.str37 = ram_basic.data["lever_sensitivity"];// Lever Sensitivity
        bulletin->AmsBulletin.REQ301Struct.str38 = ram_basic.data["push_sensitivity"]; // Push Sensitivity
        bulletin->AmsBulletin.REQ301Struct.str39 = ram_basic.data["motswver"];         // MotSWVer 
        SetMainFSM(MAIN_FSM::SETTING);
    }
    return result;
}
// Initial Gtcs System.
int GtcsManager::RunGtcsSystem()
{
    int result = 0;
    if (bulletin->uisetting==false)
    {
        mcb->CheckMcbFSM((int)MCB_FSM::NORMAL_POLLING);
        ConvertActuralData300();
    }
    else
    {
        SetMainFSM(MAIN_FSM::SETTING); // MAIN_FSM Jump to setting mode.
    }
    return result;
}
// Clear Gtcs System Alarm.
int GtcsManager::ClearGtcsSystemAlarm()
{
    int result = 0;
    return result;
}
// Setting Gtcs System.
int GtcsManager::SettingGtcsSystem()
{
    int result = 0;
    if (bulletin->uisetting==true)
    {
        if (CheckUiSettingFSM(ams->GetAmsCmdNum(bulletin->uisockrevcmd)))
        {
            bulletin->uisetting = false;
            SetMainFSM(MAIN_FSM::READY);
        }
    }
    else
    {
        // CheckUiSettingFSM(ams->GetAmsCmdNum(bulletin->sockrevcmd));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return result;
}
#pragma region UI
// Check Request Status from UI.
std::string GtcsManager::CheckUiCmdRequest(std::string reqest_string)
{
    return ams->SetAmsBulletin(reqest_string);
}
#pragma endregion

// Check ui request status.
int GtcsManager::CheckMainFSM(int main_fsm)
{
    int result = 0;
    // Check gtcs Ams Protocol.
    switch (main_fsm)
    {
    case MAIN_FSM::READY:
        result = RunGtcsSystem();
        break;
    case MAIN_FSM::SETTING:
        std::cout << "CheckMainFSM = SETTING" << std::endl;
        result = SettingGtcsSystem();
        break;
    case MAIN_FSM::ALARM:
        result = ClearGtcsSystemAlarm();
        break;
    case MAIN_FSM::CHECK_SYS:
        result = CheckGtcsSystem();
        break;
    case MAIN_FSM::INITIAL:
        result = InitialGtcsSystem();
        break;
    }
    return result;
}