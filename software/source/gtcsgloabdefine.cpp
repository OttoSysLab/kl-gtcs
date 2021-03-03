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
#include "gtcsgloabdefine.h"

/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsScrewSequenceHandler::GtcsScrewSequenceHandler()
 *
 *  @brief   GtcsScrewSequenceHandler constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsScrewSequenceHandler::GtcsScrewSequenceHandler()
{
    GtcsJob = {
        .jobid = 0,
    };
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsScrewSequenceHandler::~GtcsScrewSequenceHandler()
 *
 *  @brief   GtcsScrewSequenceHandler distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsScrewSequenceHandler::~GtcsScrewSequenceHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBaseInfo::GtcsDatabaseBaseInfo()
 *
 *  @brief   GtcsDatabaseBaseInfo constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseBaseInfo::GtcsDatabaseBaseInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBaseInfo::~GtcsDatabaseBaseInfo()
 *
 *  @brief   GtcsDatabaseBaseInfo distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseBaseInfo::~GtcsDatabaseBaseInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBaseInfo::InitialDataStruct()
 *
 *  @brief   Initial Gtcsdata structor.
 * 
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseBaseInfo::InitialDataStruct()
{ 
    int columnnames_size = columnnames.size();
    for (int i = 0; i < columnnames_size; i++)
    {
        data.insert(std::pair<std::string,std::string>(columnnames[i],"-"));
    }        
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBaseInfo::SetDataValue(std::map<std::string,std::string> &pDataValue)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::map<std::string,std::string> &pDataValue
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseBaseInfo::SetDataValue(std::map<std::string,std::string> &pDataValue)
{
    // std::cout << "======================SetDataValue====================" << std::endl;
    int columnnames_size = columnnames.size();
    for (int i = 0; i < columnnames_size; i++)
    {
        data[columnnames[i]] = pDataValue[columnnames[i]];
        // std::cout <<"Index = "<<i<<" "<< columnnames[i] << " = " <<*ptr<< std::endl;
    }
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseBasicInfo::GtcsTcsDatabaseBasicInfo()
 *
 *  @brief   GtcsTcsDatabaseBasicInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseBasicInfo::GtcsTcsDatabaseBasicInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseBasicInfo::~GtcsTcsDatabaseBasicInfo()
 *
 *  @brief   GtcsTcsDatabaseBasicInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseBasicInfo::~GtcsTcsDatabaseBasicInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseBasicInfo::InitialColumnType()
 *
 *  @brief   Initial Gtcs database basice table columntype list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseBasicInfo::InitialColumnType()
{    
    type.insert(std::pair<std::string,std::string>("mintemp","REAL"));            // Min temperature       (REAL)
    type.insert(std::pair<std::string,std::string>("maxtemp","REAL"));            // Max temperature       (REAL)
    type.insert(std::pair<std::string,std::string>("maxcurrent","INTEGER"));      // Max current           (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxpeakcurrent","INTEGER"));  // Max peak current      (INTEGER)
    type.insert(std::pair<std::string,std::string>("torquesensortype","INTEGER"));// torque sensor type    (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxdutycycle","REAL"));       // Max duty cycle        (REAL)
    type.insert(std::pair<std::string,std::string>("maxtorque","REAL"));          // Max torque            (REAL)
    type.insert(std::pair<std::string,std::string>("pwmfreq","INTEGER"));         // PWM frequency         (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxrpm","INTEGER"));          // Max rpm               (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxslope","INTEGER"));        // Max slope             (INTEGER)
    type.insert(std::pair<std::string,std::string>("minbusvolt","REAL"));         // Min bus voltage       (REAL)
    type.insert(std::pair<std::string,std::string>("maxbusvolt","REAL"));         // Max bus voltage       (REAL)
    type.insert(std::pair<std::string,std::string>("startdutycycle","REAL"));     // Start duty cycle      (REAL)
    type.insert(std::pair<std::string,std::string>("gearboxratio","REAL"));       // Gear box ratio        (REAL)
    type.insert(std::pair<std::string,std::string>("startinp","INTEGER"));        // Start input source    (INTEGER)
    type.insert(std::pair<std::string,std::string>("revinp","INTEGER"));          // Reverse ipnut source  (INTEGER)
    type.insert(std::pair<std::string,std::string>("revrpm","INTEGER"));          // Reverse rpm           (INTEGER)
    type.insert(std::pair<std::string,std::string>("revslope","INTEGER"));        // Reverse slope         (INTEGER)
    type.insert(std::pair<std::string,std::string>("revmaxcurrent","INTEGER"));   // Reverse max current   (INTEGER)
    type.insert(std::pair<std::string,std::string>("revmaxtorque","REAL"));       // Reverse max torque    (REAL)
    type.insert(std::pair<std::string,std::string>("erroridletime","INTEGER"));   // Error idle time       (INTEGER)
    type.insert(std::pair<std::string,std::string>("backlash","INTEGER"));        // Bachlash              (INTEGER)
    type.insert(std::pair<std::string,std::string>("pgain","INTEGER"));           // Proportional gain     (INTEGER)
    type.insert(std::pair<std::string,std::string>("igain","INTEGER"));           // Integral gain         (INTEGER)
    type.insert(std::pair<std::string,std::string>("encoder","INTEGER"));         // Encoder               (INTEGER)
    // New add
    type.insert(std::pair<std::string,std::string>("mintorque","REAL"));          // (REAL)
    type.insert(std::pair<std::string,std::string>("minrpm","INTEGER"));           // (INTEGER)
    type.insert(std::pair<std::string,std::string>("revminrpm","INTEGER"));        // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmsswver","INTEGER"));         // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmscoreid","INTEGER"));        // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmssernr","INTEGER"));         // (INTEGER)
    type.insert(std::pair<std::string,std::string>("led","INTEGER"));              // (INTEGER)
    type.insert(std::pair<std::string,std::string>("lever_sensitivity","INTEGER"));// (INTEGER)
    type.insert(std::pair<std::string,std::string>("push_sensitivity","INTEGER")); // (INTEGER) 
    type.insert(std::pair<std::string,std::string>("motswver","TEXT"));         // (TEXT)
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseBasicInfo::InitialColumnName()
 *
 *  @brief   Initial Gtcs database basice table columnname list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseBasicInfo::InitialColumnName()
{
    columnnames.push_back("mintemp");         // Min temperature       (REAL)
    columnnames.push_back("maxtemp");         // Max temperature       (REAL)
    columnnames.push_back("maxcurrent");      // Max current           (INTEGER)
    columnnames.push_back("maxpeakcurrent");  // Max peak current      (INTEGER)
    columnnames.push_back("torquesensortype");// torque sensor type    (INTEGER)
    columnnames.push_back("maxdutycycle");    // Max duty cycle        (REAL)
    columnnames.push_back("maxtorque");       // Max torque            (REAL)
    columnnames.push_back("pwmfreq");         // PWM frequency         (INTEGER)
    columnnames.push_back("maxrpm");          // Max rpm               (INTEGER)
    columnnames.push_back("maxslope");        // Max slope             (INTEGER)
    columnnames.push_back("minbusvolt");      // Min bus voltage       (REAL)
    columnnames.push_back("maxbusvolt");      // Max bus voltage       (REAL)
    columnnames.push_back("startdutycycle");  // Start duty cycle      (REAL)
    columnnames.push_back("gearboxratio");    // Gear box ratio        (REAL)
    columnnames.push_back("startinp");        // Start input source    (INTEGER)
    columnnames.push_back("revinp");          // Reverse ipnut source  (INTEGER)
    columnnames.push_back("revrpm");          // Reverse rpm           (INTEGER)
    columnnames.push_back("revslope");        // Reverse slope         (INTEGER)
    columnnames.push_back("revmaxcurrent");   // Reverse max current   (INTEGER)
    columnnames.push_back("revmaxtorque");    // Reverse max torque    (REAL)
    columnnames.push_back("erroridletime");   // Error idle time       (INTEGER)
    columnnames.push_back("backlash");        // Bachlash              (INTEGER)
    columnnames.push_back("pgain");           // Proportional gain     (INTEGER)
    columnnames.push_back("igain");           // Integral gain         (INTEGER)
    columnnames.push_back("encoder");         // Encoder               (INTEGER)
    // New add
    columnnames.push_back("mintorque");        // (REAL)
    columnnames.push_back("minrpm");           // (INTEGER)
    columnnames.push_back("revminrpm");        // (INTEGER)
    columnnames.push_back("dmsswver");         // (INTEGER)
    columnnames.push_back("dmscoreid");        // (INTEGER)
    columnnames.push_back("dmssernr");         // (INTEGER)
    columnnames.push_back("led");              // (INTEGER)
    columnnames.push_back("lever_sensitivity");// (INTEGER)
    columnnames.push_back("push_sensitivity"); // (INTEGER) 
    columnnames.push_back("motswver");         // (TEXT)
    // columnnames.push_back("end");           // CL,RF  
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseJobInfo::GtcsTcsDatabaseJobInfo()
 *
 *  @brief   GtcsTcsDatabaseJobInfo constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseJobInfo::GtcsTcsDatabaseJobInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseJobInfo::~GtcsTcsDatabaseJobInfo()
 *
 *  @brief   GtcsTcsDatabaseJobInfo distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseJobInfo::~GtcsTcsDatabaseJobInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseJobInfo::InitialColumnType()
 *
 *  @brief   Initial Gtcs database Jobsequence table columntype list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseJobInfo::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("id","INTEGER"));                  // column_index  = 0
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));              // column_index  = 1
    type.insert(std::pair<std::string,std::string>("job_name","TEXT"));               // column_index  = 2
    type.insert(std::pair<std::string,std::string>("unscrew_direction","INTEGER"));   // column_index  = 3
    type.insert(std::pair<std::string,std::string>("force","INTEGER"));               // column_index  = 4
    type.insert(std::pair<std::string,std::string>("rpm","INTEGER"));                 // column_index  = 5
    type.insert(std::pair<std::string,std::string>("enable_unscrew_force","INTEGER"));// column_index  = 6            
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseJobInfo::InitialColumnName()
 *
 *  @brief   Initial Gtcs database Jobsequence table columnname list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseJobInfo::InitialColumnName()
{
    columnnames.push_back("id");                   // column_index  = 0
    columnnames.push_back("job_id");               // column_index  = 1
    columnnames.push_back("job_name");             // column_index  = 2
    columnnames.push_back("unscrew_direction");    // column_index  = 3
    columnnames.push_back("force");                // column_index  = 4
    columnnames.push_back("rpm");                  // column_index  = 5
    columnnames.push_back("enable_unscrew_force"); // column_index  = 6        
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseSequenceInfo ::GtcsTcsDatabaseSequenceInfo()
 *
 *  @brief   GtcsTcsDatabaseSequenceInfo constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseSequenceInfo ::GtcsTcsDatabaseSequenceInfo (/* args */)
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseSequenceInfo ::~GtcsTcsDatabaseSequenceInfo()
 *
 *  @brief   GtcsTcsDatabaseSequenceInfo distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseSequenceInfo ::~GtcsTcsDatabaseSequenceInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseSequenceInfo ::InitialColumnType()
 *
 *  @brief   Initial Gtcs database program table columntype list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseSequenceInfo ::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));          // column_index  = 0
    type.insert(std::pair<std::string,std::string>("seq_id","INTEGER"));          // column_index  = 1
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));       // column_index  = 2
    type.insert(std::pair<std::string,std::string>("ok_time","INTEGER"));         // column_index  = 3
    type.insert(std::pair<std::string,std::string>("ng_stop","INTEGER"));         // column_index  = 4
    type.insert(std::pair<std::string,std::string>("tr","INTEGER"));              // column_index  = 5
    type.insert(std::pair<std::string,std::string>("ok_seq","INTEGER"));          // column_index  = 6
    type.insert(std::pair<std::string,std::string>("ok_seq_time","REAL"));        // column_index  = 7
    type.insert(std::pair<std::string,std::string>("seq_stop","INTEGER"));        // column_index  = 8
    type.insert(std::pair<std::string,std::string>("program_mintime","REAL"));    // column_index  = 8
    type.insert(std::pair<std::string,std::string>("program_maxtime","REAL"));    // column_index  = 8
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseSequenceInfo ::InitialColumnName()
 *
 *  @brief   Initial Gtcs database program table columnname list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseSequenceInfo ::InitialColumnName()
{ 
    columnnames.push_back("job_id");          // column_index  = 0
    columnnames.push_back("seq_id");          // column_index  = 1
    columnnames.push_back("program_name");    // column_index  = 2
    columnnames.push_back("ok_time");         // column_index  = 3
    columnnames.push_back("ng_stop");         // column_index  = 4
    columnnames.push_back("tr");              // column_index  = 5
    columnnames.push_back("ok_seq");          // column_index  = 6
    columnnames.push_back("ok_seq_time");     // column_index  = 7
    columnnames.push_back("seq_stop");        // column_index  = 8
    columnnames.push_back("program_mintime"); // column_index  = 9
    columnnames.push_back("program_maxtime"); // column_index  = 10
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseStepInfo::GtcsTcsDatabaseStepInfo ()
 *
 *  @brief   GtcsTcsDatabaseStepInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseStepInfo::GtcsTcsDatabaseStepInfo (/* args */)
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseStepInfo::GtcsTcsDatabaseStepInfo ()
 *
 *  @brief   GtcsTcsDatabaseStepInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabaseStepInfo::~GtcsTcsDatabaseStepInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseStepInfo ::InitialColumnType()
 *
 *  @brief   Initial Gtcs database step table columntype list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseStepInfo ::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));                // colunm_index = 0
    type.insert(std::pair<std::string,std::string>("seq_id","INTEGER"));                // colunm_index = 1
    type.insert(std::pair<std::string,std::string>("target_type","INTEGER"));           // colunm_index = 2
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));             // colunm_index = 3
    type.insert(std::pair<std::string,std::string>("ScrewStepID","INTEGER"));           // colunm_index = 4
    type.insert(std::pair<std::string,std::string>("u8StepName","TEXT"));               // colunm_index = 5
    type.insert(std::pair<std::string,std::string>("u16StepRpm","INTEGER"));            // colunm_index = 6
    type.insert(std::pair<std::string,std::string>("u16StepMaxTorque","REAL"));         // colunm_index = 7 
    type.insert(std::pair<std::string,std::string>("u16StepMaxRevol","REAL"));          // colunm_index = 8 
    type.insert(std::pair<std::string,std::string>("u16StepTime","REAL"));              // colunm_index = 9
    type.insert(std::pair<std::string,std::string>("u16StepAngle","REAL"));             // colunm_index = 10
    type.insert(std::pair<std::string,std::string>("u16StepAngleWindow","REAL"));       // colunm_index = 11
    type.insert(std::pair<std::string,std::string>("u16StepTorqueWindow","REAL"));      // colunm_index = 12
    type.insert(std::pair<std::string,std::string>("ScrewHiTorque","REAL"));            // colunm_index = 13
    type.insert(std::pair<std::string,std::string>("ScrewLoTorque","REAL"));            // colunm_index = 14
    type.insert(std::pair<std::string,std::string>("ScrewHiAngle","REAL"));             // colunm_index = 15
    type.insert(std::pair<std::string,std::string>("ScrewLoAngle","REAL"));             // colunm_index = 16
    type.insert(std::pair<std::string,std::string>("TorqueThreshold","REAL"));          // colunm_index = 17
    type.insert(std::pair<std::string,std::string>("TorqueJointOffset","REAL"));        // colunm_index = 18
    type.insert(std::pair<std::string,std::string>("StepDelaytime","REAL"));            // colunm_index = 19
    type.insert(std::pair<std::string,std::string>("ScrewStepDirection","INTEGER"));    // colunm_index = 20
    type.insert(std::pair<std::string,std::string>("StepMonitoringMode","INTEGER"));    // colunm_index = 21
    type.insert(std::pair<std::string,std::string>("off_set","INTEGER"));               // colunm_index = 22
    type.insert(std::pair<std::string,std::string>("enable_downshift","INTEGER"));      // colunm_index = 23
    type.insert(std::pair<std::string,std::string>("downshift_torque","INTEGER"));      // colunm_index = 24
    type.insert(std::pair<std::string,std::string>("downshift_speed","INTEGER"));       // colunm_index = 25
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabaseStepInfo ::InitialColumnName()
 *
 *  @brief   Initial Gtcs database step table columnname list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcsDatabaseStepInfo ::InitialColumnName()
{
    columnnames.push_back("job_id");                // colunm_index = 0
    columnnames.push_back("seq_id");                // colunm_index = 1
    columnnames.push_back("target_type");           // colunm_index = 2
    columnnames.push_back("program_name");          // colunm_index = 3
    columnnames.push_back("ScrewStepID");           // colunm_index = 4
    columnnames.push_back("u8StepName");            // colunm_index = 5
    columnnames.push_back("u16StepRpm");            // colunm_index = 6
    columnnames.push_back("u16StepMaxTorque");      // colunm_index = 7 
    columnnames.push_back("u16StepMaxRevol");       // colunm_index = 8 
    columnnames.push_back("u16StepTime");           // colunm_index = 9
    columnnames.push_back("u16StepAngle");          // colunm_index = 10
    columnnames.push_back("u16StepAngleWindow");    // colunm_index = 11
    columnnames.push_back("u16StepTorqueWindow");   // colunm_index = 12
    columnnames.push_back("ScrewHiTorque");         // colunm_index = 13
    columnnames.push_back("ScrewLoTorque");         // colunm_index = 14
    columnnames.push_back("ScrewHiAngle");          // colunm_index = 15
    columnnames.push_back("ScrewLoAngle");          // colunm_index = 16
    columnnames.push_back("TorqueThreshold");       // colunm_index = 17
    columnnames.push_back("TorqueJointOffset");     // colunm_index = 18
    columnnames.push_back("StepDelaytime");         // colunm_index = 19
    columnnames.push_back("ScrewStepDirection");    // colunm_index = 20
    columnnames.push_back("StepMonitoringMode");    // colunm_index = 21
    columnnames.push_back("off_set");               // colunm_index = 22
    columnnames.push_back("enable_downshift");      // colunm_index = 23
    columnnames.push_back("downshift_torque");      // colunm_index = 24
    columnnames.push_back("downshift_speed");       // colunm_index = 25
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusbaseInfo::GtcsScrewStatusbaseInfo()
 *
 *  @brief   GtcsScrewStatusbaseInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsScrewStatusbaseInfo::GtcsScrewStatusbaseInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusbaseInfo::~GtcsScrewStatusbaseInfo()
 *
 *  @brief   GtcsScrewStatusbaseInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsScrewStatusbaseInfo::~GtcsScrewStatusbaseInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusbaseInfo ::InitialColumnType()
 *
 *  @brief   Initial Gtcs database screw data table columntype list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsScrewStatusbaseInfo ::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("data_time","DATETIME"));            // colunm_index = 0
    type.insert(std::pair<std::string,std::string>("device_type","INTEGER"));           // colunm_index = 1
    type.insert(std::pair<std::string,std::string>("device_sn","TEXT"));                // colunm_index = 2
    type.insert(std::pair<std::string,std::string>("tool_sn","TEXT"));                  // colunm_index = 3
    type.insert(std::pair<std::string,std::string>("selected_job","INTEGER"));          // colunm_index = 4
    type.insert(std::pair<std::string,std::string>("selected_sequence","INTEGER"));     // colunm_index = 5
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));             // colunm_index = 6
    type.insert(std::pair<std::string,std::string>("selected_step","INTEGER"));         // colunm_index = 7 
    type.insert(std::pair<std::string,std::string>("direction","INTEGER"));             // colunm_index = 8 
    type.insert(std::pair<std::string,std::string>("fasten_unit","INTEGER"));           // colunm_index = 9
    type.insert(std::pair<std::string,std::string>("count_direction","INTEGER"));       // colunm_index = 10
    type.insert(std::pair<std::string,std::string>("last_screw_count","INTEGER"));      // colunm_index = 11
    type.insert(std::pair<std::string,std::string>("max_screw_count","INTEGER"));       // colunm_index = 12
    type.insert(std::pair<std::string,std::string>("fasten_time","REAL"));              // colunm_index = 13
    type.insert(std::pair<std::string,std::string>("fasten_torque","REAL"));            // colunm_index = 14
    type.insert(std::pair<std::string,std::string>("fasten_angle","REAL"));             // colunm_index = 15
    type.insert(std::pair<std::string,std::string>("max_torque","REAL"));               // colunm_index = 16
    type.insert(std::pair<std::string,std::string>("revolutions","REAL"));              // colunm_index = 17
    type.insert(std::pair<std::string,std::string>("fasten_status","TEXT"));            // colunm_index = 18
    type.insert(std::pair<std::string,std::string>("input_io","TEXT"));                 // colunm_index = 19
    type.insert(std::pair<std::string,std::string>("output_io","TEXT"));                // colunm_index = 20
    type.insert(std::pair<std::string,std::string>("error_masseage","TEXT"));           // colunm_index = 21
    type.insert(std::pair<std::string,std::string>("tool_count","INTEGER"));            // colunm_index = 22
    type.insert(std::pair<std::string,std::string>("rpm","INTEGER"));                   // colunm_index = 23
    type.insert(std::pair<std::string,std::string>("tool_status","INTEGER"));           // colunm_index = 24
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsScrewStatusbaseInfo ::InitialColumnName()
 *
 *  @brief   Initial Gtcs database step table columnname list.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsScrewStatusbaseInfo ::InitialColumnName()
{
    columnnames.push_back("data_time");                  // colunm_index = 0
    columnnames.push_back("device_type");                // colunm_index = 1
    columnnames.push_back("device_sn");                  // colunm_index = 2
    columnnames.push_back("tool_sn");                    // colunm_index = 3
    columnnames.push_back("selected_job");               // colunm_index = 4
    columnnames.push_back("selected_sequence");          // colunm_index = 5
    columnnames.push_back("program_name");               // colunm_index = 6
    columnnames.push_back("selected_step");              // colunm_index = 7 
    columnnames.push_back("direction");                  // colunm_index = 8 
    columnnames.push_back("fasten_unit");                // colunm_index = 9
    columnnames.push_back("count_direction");            // colunm_index = 10
    columnnames.push_back("last_screw_count");           // colunm_index = 11
    columnnames.push_back("max_screw_count");            // colunm_index = 12
    columnnames.push_back("fasten_time");                // colunm_index = 13
    columnnames.push_back("fasten_torque");              // colunm_index = 14
    columnnames.push_back("fasten_angle");               // colunm_index = 15
    columnnames.push_back("max_torque");                 // colunm_index = 16
    columnnames.push_back("revolutions");                // colunm_index = 17
    columnnames.push_back("fasten_status");              // colunm_index = 18
    columnnames.push_back("input_io");                   // colunm_index = 19
    columnnames.push_back("output_io");                  // colunm_index = 20
    columnnames.push_back("error_masseage");             // colunm_index = 21
    columnnames.push_back("tool_count");                 // colunm_index = 22
    columnnames.push_back("rpm");                        // colunm_index = 23
    columnnames.push_back("tool_status");                // colunm_index = 24
}
#pragma endregion
