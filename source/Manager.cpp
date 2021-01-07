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
bool GtcsManager::ConvertAmsBasicToMcbBulletin(AmsCMD340Struct &amscmd)
{
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // 
    bulletin->McbBulletin.BasicPara.s16MinTemp
        = (uint16_t)(std::stof(amscmd.str5)*10);// SID = 1,Minimal Temperature of the motor and the motorcontroller. 
                                                // Underneath this temperature the tool doesn’t work. Unit is [0,1 °C]."
    bulletin->McbBulletin.BasicPara.s16MaxTemp
        = (uint16_t)(std::stof(amscmd.str6)*10);// SID = 2,Maximal Temperature of the motor and the motorcontroller. 
                                                // Above this temperature the tool doesn’t work. Unit is [0,1 °C ].
    bulletin->McbBulletin.BasicPara.u16MaxCurrent
        = (uint16_t)std::stoi(amscmd.str7);     // SID = 3,This is the absolute maximum motor current value. 
                                                // There are also maximum current values for the specific process steps 
                                                // (see….), but this parameter is the limit. Unit is [mA].
    bulletin->McbBulletin.BasicPara.u16MaxPeakCurrent
        = (uint16_t)std::stoi(amscmd.str8);     // SID = 4,This value controls the threshold of the peak current comparator.
                                                // The peak current detection is built in for the protection of the device 
                                                // when a latch up situation occurs. Unit is [A].
    bulletin->McbBulletin.BasicPara.u16TorqueSensorType
        = (uint16_t)std::stoi(amscmd.str9);    // SID = 5, 0 = Torquesensor 5Nm, 1 = Torquesensor 4Nm,2 = No Torquesensor"
    bulletin->McbBulletin.BasicPara.u16MaxDutyCycle     
        = (uint16_t)(std::stof(amscmd.str10)*10);// SID = 6,Maximal Duty Cycle of the Motor- PWM 1 – 100%. Unit is [0.1 %]

    //bulletin->McbBulletin.BasicPara.u16MaxTorque        
    //   = (uint16_t)std::stoi(amscmd.str11);   // SID = 7,"Maximum Torque Value is 0- 1862 (maxRaw TMD Value)"
    
    bulletin->McbBulletin.BasicPara.u16PWMFreq
        = (uint16_t)std::stoi(amscmd.str12);   // SID = 8,0 = 16kHz,1 = 24kHz,2 = 32kHz,3 = 40kHz,4 = 48kHz"
    bulletin->McbBulletin.BasicPara.u16MaxRPM
        = (uint16_t)std::stoi(amscmd.str13);    // SID = 9,This is the absolute maximum motor rpm value. 
                                                // There are also rpm values for the specific process steps (see….) , 
                                                // but this parameter is the limit. Unit is [rpm].
    bulletin->McbBulletin.BasicPara.u16MaxSlope 
        = (uint16_t)std::stoi(amscmd.str14);    // SID = 10,This is the absolute maximum motor rpm ramp (up/down) value.
                                                // There are also rpm slope values for the specific process steps (see….) , 
                                                // but this parameter is the limit. Unit is [rpm/s].
    bulletin->McbBulletin.BasicPara.u16MinBusVolt       
        = (uint16_t)(std::stof(amscmd.str15)*10);// SID = 11,Minimal Bus Voltage of the Power Supply. 
                                                // Underneath this Voltage the tool doesn’t work. 
                                                // Unit is [0,1V] (600 = 60V).
    bulletin->McbBulletin.BasicPara.u16MaxBusVolt
        = (uint16_t)(std::stof(amscmd.str16)*10);    // SID = 12,Maximal Bus Voltage of the Power Supply. 
                                                // Above this Voltage the tool doesn’t work. Unit is [0,1V].
    bulletin->McbBulletin.BasicPara.u16StartDutyCycle   
        = (uint16_t)std::stoi(amscmd.str17);    // SID = 13,Start Duty Cycle for the RPM regulator Unit is [0,1%].(20 = 10%)

    bulletin->McbBulletin.BasicPara.u16GearBoxRatio     
        = (uint16_t)(std::stof(amscmd.str18)*100);    // SID = 14,Gear box ratio. Unit [0,01] (1600 = 16:1)
    bulletin->McbBulletin.BasicPara.u32StartInp 
        = (uint16_t)std::stoi(amscmd.str19);    // SID = 15,0 = Start Switch,1 = From Displaycontroller Telegram"
    bulletin->McbBulletin.BasicPara.u32RevInp           
        = (uint16_t)std::stoi(amscmd.str20);   // SID = 16,0 = Reverse Switch,1 = From Displaycontroller"
    bulletin->McbBulletin.BasicPara.u16RevRpm  
        = (uint16_t)std::stoi(amscmd.str21);   // SID = 17,"Rpm of reverse drive. Unit is [rpm](after the Gearbox)"
    bulletin->McbBulletin.BasicPara.u16RevSlope      
        = (uint16_t)std::stoi(amscmd.str22);   // SID = 18,"Slope of reverse drive. Unit is [rpm/s](after the Gearbox)."
    bulletin->McbBulletin.BasicPara.u16RevMaxCurrent    
        = (uint16_t)std::stoi(amscmd.str23);   // SID = 19,"Maximum Current of reverse drive. Unit is [mA]."

    //bulletin->McbBulletin.BasicPara.u16RevMaxTorque     
    // = (uint16_t)std::stoi(amscmd.str24);   // SID = 20,"Maximum Torque of reverse drive. 
    //                                         // Value is 0- 1862 (max TMD Raw Value)"
    bulletin->McbBulletin.BasicPara.u16ErrorIdleTime
        = (uint16_t)std::stoi(amscmd.str25);   // SID = 21,"Idle time of the motor controller after a Error condition.Unit is [ms]."
    bulletin->McbBulletin.BasicPara.u16BackLash         
        = (uint16_t)std::stoi(amscmd.str26);   // SID = 22,"Backlash of the gearbox. The value depends if a Encoder or 
                                                // Hallsensors are used for angle Positioning. Unit is [Increments]."
    bulletin->McbBulletin.BasicPara.u16PGain            
        = (uint16_t)std::stoi(amscmd.str27);   // SID = 23,Proportional Gain for the RPM Regulator.
    bulletin->McbBulletin.BasicPara.u16IGain            
        = (uint16_t)std::stoi(amscmd.str28);   // SID = 24,Integral Gain for the RPM Regulator.
    bulletin->McbBulletin.BasicPara.u16Encoder          
        = (uint16_t)std::stoi(amscmd.str29);   // SID = 25,"0 = No Encoder (positioning with Hallsensors).
    
    return true;
}

// Check Ui Setting FSM.
int GtcsManager::CheckUiSettingFSM(int uicmd)
{
    int result = 0;

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
        result = 0;
        break;
    case AMSCMD::CMD340:
        #pragma region cmd340 sequence
        // step 1 : Convert cmd340 to mcb command.
        ConvertAmsBasicToMcbBulletin(bulletin->AmsBulletin.CMD340Struct);
        // step 2 : Update MCB basic paramater.
        mcb->WriteBasicParameter(&bulletin->McbBulletin.BasicPara);
        // step 3 : Read MCB basic parameeter. 
        
        // step 4 : Update ramdisk database.
        
        // step 5 : Compare basic data btweem ramdisk database and emmc database.

        // step 6 : Update ans cmd.            
        // ams->GetAmsBulletin(AMSCMD::ANS340);     
           
        #pragma endregion
        result = 0;
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
        uiresponsecmd = ams->GetAmsSymmetryCmdNumver(ams->GetAmsCmdNum(uicmd_string));
    }
    return ams->GetAmsBulletin(uiresponsecmd);
}
// Initial Gtcs System.
bool GtcsManager::InitialGtcsSystem()
{
    // Initial MCB Com.
    // mcb->InitialMcbComPort("/dev/ttymxc3");
    mcb->InitialMcbComPort(comport_name);
    for(int index=0;index<5;index++)
    {
        mcb->CheckMcbFSM((int)MCB_FSM::NORMAL_POLLING);
        ConvertReadlTimeActuralValue();
    }
    mcb->telegram.ctrl.IsEnable = false;
    // Min fsm jump to Check system status.
    SetMainFSM(MAIN_FSM::CHECK_SYS);
    return false;
}
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
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MinTemp/10,1);// Min temperature       (REAL)
    db_basic.data["maxtemp"]          
        = DataSorter::GetFloatScaleSortString((float)mcb_basic.s16MaxTemp/10,1);// Max temperature       (REAL) 
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
    db_basic.data["startdutycycle"]   = std::to_string(mcb_basic.u16StartDutyCycle);  // Start duty cycle      (REAL) 
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
    
    // Update Sqlite 
    db.UpdateDatabase(db.GetDatabasePath(),"basic",db_basic.GetUpdateSqlCommand());
    return true;
}

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
// 
bool GtcsManager::SetAmsBulletinBasicPara(GtcsDatabaseBasicInfo &basic)
{
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // Setting REQ301 value.
    bulletin->AmsBulletin.REQ301Struct.str5  = basic.data["mintemp"];          // Min temperature
    bulletin->AmsBulletin.REQ301Struct.str6  = basic.data["maxtemp"];          // Max temperature
    bulletin->AmsBulletin.REQ301Struct.str7  = basic.data["maxcurrent"];       // Max current
    bulletin->AmsBulletin.REQ301Struct.str8  = basic.data["maxpeakcurrent"];   // Max peak current
    bulletin->AmsBulletin.REQ301Struct.str9  = basic.data["torquesensortype"]; // torque sensor type
    bulletin->AmsBulletin.REQ301Struct.str10 = basic.data["maxdutycycle"];     // Max duty cycle
    bulletin->AmsBulletin.REQ301Struct.str11 = basic.data["maxtorque"];        // Max torque
    bulletin->AmsBulletin.REQ301Struct.str12 = basic.data["pwmfreq"];          // PWM frequency 
    bulletin->AmsBulletin.REQ301Struct.str13 = basic.data["maxrpm"];           // Max rpm
    bulletin->AmsBulletin.REQ301Struct.str14 = basic.data["maxslope"];         // Max slope
    bulletin->AmsBulletin.REQ301Struct.str15 = basic.data["minbusvolt"];       // Min bus voltage
    bulletin->AmsBulletin.REQ301Struct.str16 = basic.data["maxbusvolt"];       // Max bus voltage
    bulletin->AmsBulletin.REQ301Struct.str17 = basic.data["startdutycycle"];   // Start duty cycle
    bulletin->AmsBulletin.REQ301Struct.str18 = basic.data["gearboxratio"];     // Gear box ratio
    bulletin->AmsBulletin.REQ301Struct.str19 = basic.data["startinp"];         // Start input source
    bulletin->AmsBulletin.REQ301Struct.str20 = basic.data["revinp"];           // Reverse ipnut source
    bulletin->AmsBulletin.REQ301Struct.str21 = basic.data["revrpm"];           // Reverse rpm
    bulletin->AmsBulletin.REQ301Struct.str22 = basic.data["revslope"];         // Reverse slope
    bulletin->AmsBulletin.REQ301Struct.str23 = basic.data["revmaxcurrent"];    // Reverse max current
    bulletin->AmsBulletin.REQ301Struct.str24 = basic.data["revmaxtorque"];     // Reverse max torque
    bulletin->AmsBulletin.REQ301Struct.str25 = basic.data["erroridletime"];    // Error idle time
    bulletin->AmsBulletin.REQ301Struct.str26 = basic.data["backlash"];         // Bachlash 
    bulletin->AmsBulletin.REQ301Struct.str27 = basic.data["pgain"];            // Proportional gain
    bulletin->AmsBulletin.REQ301Struct.str28 = basic.data["igain"];            // Integral gain
    bulletin->AmsBulletin.REQ301Struct.str29 = basic.data["encoder"];          // Encoder
    // 
    bulletin->AmsBulletin.REQ301Struct.str30 = basic.data["mintorque"];        // Min Torque
    bulletin->AmsBulletin.REQ301Struct.str31 = basic.data["minrpm"];           // Min RPM
    bulletin->AmsBulletin.REQ301Struct.str32 = basic.data["revminrpm"];        // Reverse Min RPM
    bulletin->AmsBulletin.REQ301Struct.str33 = basic.data["dmsswver"];         // DmsSWVer
    bulletin->AmsBulletin.REQ301Struct.str34 = basic.data["dmscoreid"];        // DmsCoreID
    bulletin->AmsBulletin.REQ301Struct.str35 = basic.data["dmssernr"];         // DmsSerNr
    bulletin->AmsBulletin.REQ301Struct.str36 = basic.data["led"];              // Led
    bulletin->AmsBulletin.REQ301Struct.str37 = basic.data["lever_sensitivity"];// Lever Sensitivity
    bulletin->AmsBulletin.REQ301Struct.str38 = basic.data["push_sensitivity"]; // Push Sensitivity
    bulletin->AmsBulletin.REQ301Struct.str39 = basic.data["motswver"];         // MotSWVer
    return true;
}
// Initial Gtcs System.
bool GtcsManager::CheckGtcsSystem()
{
    // Initial value.
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // std::string db_emmc_Path = "/var/www/html/database/tcs.db";   // Initial database path.
    // std::string db_ramdisk_Path = "/mnt/ramdisk/tcs.db";          // Initial database path.
    GtcsDatabase db_emmc(db_emmc_Path);
    GtcsDatabase db_ramdisk(db_ramdisk_Path);                     
    GtcsDatabaseBasicInfo basic_emmc;     
    GtcsDatabaseBasicInfo basic_ramdisk;

    // Step 1 : Read data from mcb basice parameter.
    mcb->ReadBasicParameter();    

    // Step 2 : Copy tcs.db from emmc to ramdisk.
    CopyDatabase(db_ramdisk_Path,db_emmc_Path);
    
    // Step 3 : Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabase(db_emmc.GetDatabasePath(),"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp)==false)
    {
        std::cout << "Step 3 : Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }
    else
    {
        basic_ramdisk.SetDataValue(&bulletin->DbBulletin.basic.mintemp);
    }
    
    // Step 4 : Update MCB basic parameter to database.
    if (UpdateMcbBasicParaToDB(db_ramdisk,basic_ramdisk,bulletin->McbBulletin.BasicPara)==false)
    {
        std::cout << "Step 4 : Update MCB basic parameter to database." << std::endl;
        return false;
    }
    
    // Step 5 = Read basic data from emmc database to dbstruct.
    if (db_emmc.ReadDatabase(db_emmc.GetDatabasePath(),"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp)==false)
    {
        std::cout << "Step 5 = Read basic data from emmc database to dbstruct." << std::endl;
        return false;
    }
    else
    {
        basic_emmc.SetDataValue(&bulletin->DbBulletin.basic.mintemp);
    }

    // Step 6 : Read basic data from ramdisk database to dbstruct.
    if (db_ramdisk.ReadDatabase(db_ramdisk.GetDatabasePath(),"basic",(std::string *)(void* )&bulletin->DbBulletin.basic.mintemp)==false)
    {
        std::cout << "Step 6 : Read basic data from ramdisk database to dbstruct." << std::endl;
        return false;
    }
    else
    {
        basic_ramdisk.SetDataValue(&bulletin->DbBulletin.basic.mintemp);
    }     

    // Step 7 = Jump to selected MAIN_FSM.    
    bulletin->checksysok = CompareBasicStruct(basic_emmc,basic_ramdisk);
    std::cout << " bulletin->checksysok = " << bulletin->checksysok << std::endl;
    if (bulletin->checksysok == true)
    {
        // Display some informaiton.
        std::cout <<"Gear Ratio = " << std::to_string(bulletin->McbBulletin.BasicPara.u16GearBoxRatio)<<std::endl;
        SetMainFSM(MAIN_FSM::READY);
    }
    else
    {
        SetAmsBulletinBasicPara(basic_ramdisk);
        SetMainFSM(MAIN_FSM::SETTING);
    }
    
    return true;
}
// Initial Gtcs System.
bool GtcsManager::RunGtcsSystem()
{
    if (bulletin->uisetting==false)
    {
        mcb->CheckMcbFSM((int)MCB_FSM::NORMAL_POLLING);
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
        // CheckUiSettingFSM(ams->GetAmsCmdNum(bulletin->sockrevcmd));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return true;
}
#pragma region UI
// Check Request Status from UI.
std::string GtcsManager::CheckUiCmdRequest(std::string reqest_string)
{
    return ams->SetAmsBulletin(reqest_string);
}
#pragma endregion

//
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
// Check ui request status.
int GtcsManager::CheckMainFSM(int main_fsm)
{
    int result = 0;
    // Check gtcs Ams Protocol.
    switch (main_fsm)
    {
    case MAIN_FSM::READY:
        std::cout << "CheckMainFSM = READY" << std::endl;
        result = RunGtcsSystem();
        break;
    case MAIN_FSM::SETTING:
        std::cout << "CheckMainFSM = SETTING" << std::endl;
        result = SettingGtcsSystem();
        break;
    case MAIN_FSM::ALARM:
        std::cout << "CheckMainFSM = ALARM" << std::endl;
        result = ClearGtcsSystemAlarm();
        break;
    case MAIN_FSM::CHECK_SYS:
        std::cout << "CheckMainFSM = CHECK_SYS" << std::endl;
        result = CheckGtcsSystem();
        break;
    case MAIN_FSM::INITIAL:
        std::cout << "CheckMainFSM = INITIAL" << std::endl;
        result = InitialGtcsSystem();
        break;
    }
    return result;
}