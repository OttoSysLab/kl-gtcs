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

#pragma region GtcsScrewSequenceHandler
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsScrewSequenceHandler::GtcsScrewSequenceHandler()
{}
GtcsScrewSequenceHandler::~GtcsScrewSequenceHandler()
{}
#pragma endregion

#pragma region database base.
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseBaseInfo::GtcsDatabaseBaseInfo()
{}
GtcsDatabaseBaseInfo::~GtcsDatabaseBaseInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
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
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
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
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Constructor.
GtcsDatabaseBasicInfo::GtcsDatabaseBasicInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
// Distructor.
GtcsDatabaseBasicInfo::~GtcsDatabaseBasicInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseBasicInfo::InitialColumnType()
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
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    Initial basic columnName
 *
 *******************************************************************************************/
void GtcsDatabaseBasicInfo::InitialColumnName()
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
 *  @date    2021/01/30
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    Initial Job sequence columnName
 *
 *******************************************************************************************/
GtcsDatabaseJobSequecne::GtcsDatabaseJobSequecne()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
GtcsDatabaseJobSequecne::~GtcsDatabaseJobSequecne()
{
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/01/30
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    Initial Job sequence columnName
 *
 *******************************************************************************************/
void GtcsDatabaseJobSequecne::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("id","INTEGER"));                  // column_index  = 0
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));              // column_index  = 1
    type.insert(std::pair<std::string,std::string>("job_name","TEXT"));               // column_index  = 2
    type.insert(std::pair<std::string,std::string>("unscrew","INTEGER"));             // column_index  = 3
    type.insert(std::pair<std::string,std::string>("force","INTEGER"));               // column_index  = 4
    type.insert(std::pair<std::string,std::string>("rpm","INTEGER"));                 // column_index  = 5
    type.insert(std::pair<std::string,std::string>("enable_unscrew_force","INTEGER"));// column_index  = 6            
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/01/30
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    Initial Job sequence columnName
 *
 *******************************************************************************************/
void GtcsDatabaseJobSequecne::InitialColumnName()
{
    columnnames.push_back("id");                   // column_index  = 0
    columnnames.push_back("job_id");               // column_index  = 1
    columnnames.push_back("job_name");             // column_index  = 2
    columnnames.push_back("unscrew");              // column_index  = 3
    columnnames.push_back("force");                // column_index  = 4
    columnnames.push_back("rpm");                  // column_index  = 5
    columnnames.push_back("enable_unscrew_force"); // column_index  = 6        
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseProgramInfo ::GtcsDatabaseProgramInfo (/* args */)
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
GtcsDatabaseProgramInfo ::~GtcsDatabaseProgramInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseProgramInfo ::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));          // column_index  = 0
    type.insert(std::pair<std::string,std::string>("seq_id","INTEGER"));          // column_index  = 1
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));       // column_index  = 2
    type.insert(std::pair<std::string,std::string>("ok_time","INTEGER"));         // column_index  = 3
    type.insert(std::pair<std::string,std::string>("ng_stop","INTEGER"));         // column_index  = 4
    type.insert(std::pair<std::string,std::string>("joint_offset","REAL"));       // column_index  = 5
    type.insert(std::pair<std::string,std::string>("offset","INTEGER"));          // column_index  = 6
    type.insert(std::pair<std::string,std::string>("tr","INTEGER"));              // column_index  = 7
    type.insert(std::pair<std::string,std::string>("ok_seq","INTEGER"));          // column_index  = 8
    type.insert(std::pair<std::string,std::string>("ok_seq_time","INTEGER"));     // column_index  = 9
    type.insert(std::pair<std::string,std::string>("seq_stop","INTEGER"));        // column_index  = 10
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseProgramInfo ::InitialColumnName()
{ 
    columnnames.push_back("job_id");          // column_index  = 0
    columnnames.push_back("seq_id");          // column_index  = 1
    columnnames.push_back("program_name");    // column_index  = 2
    columnnames.push_back("ok_time");         // column_index  = 3
    columnnames.push_back("ng_stop");         // column_index  = 4
    columnnames.push_back("joint_offset");    // column_index  = 5
    columnnames.push_back("offset");          // column_index  = 6
    columnnames.push_back("tr");              // column_index  = 7
    columnnames.push_back("ok_seq");          // column_index  = 8
    columnnames.push_back("ok_seq_time");     // column_index  = 9
    columnnames.push_back("seq_stop");        // column_index  = 10
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseStepInfo::GtcsDatabaseStepInfo (/* args */)
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
GtcsDatabaseStepInfo::~GtcsDatabaseStepInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseStepInfo ::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("job_id","INTEGER"));                // colunm_index = 0
    type.insert(std::pair<std::string,std::string>("seq_id","INTEGER"));                // colunm_index = 1
    type.insert(std::pair<std::string,std::string>("target_type","INTEGER"));           // colunm_index = 2
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));             // colunm_index = 3
    type.insert(std::pair<std::string,std::string>("ScrewStepID","INTEGER"));           // colunm_index = 4
    type.insert(std::pair<std::string,std::string>("u8StepName","TEXT"));               // colunm_index = 5
    type.insert(std::pair<std::string,std::string>("u16StepRpm","INTEGER"));            // colunm_index = 6
    type.insert(std::pair<std::string,std::string>("u16StepSlope","REAL"));             // colunm_index = 7
    type.insert(std::pair<std::string,std::string>("u16StepMaxCurrent","INTEGER"));     // colunm_index = 8
    type.insert(std::pair<std::string,std::string>("u16StepMaxTorque","REAL"));         // colunm_index = 9 
    type.insert(std::pair<std::string,std::string>("u16StepMaxRevol","REAL"));          // colunm_index = 10 
    type.insert(std::pair<std::string,std::string>("u16StepTime","REAL"));              // colunm_index = 11
    type.insert(std::pair<std::string,std::string>("u16StepAngle","REAL"));             // colunm_index = 12
    type.insert(std::pair<std::string,std::string>("u16StepAngleWindow","INTEGER"));    // colunm_index = 13
    type.insert(std::pair<std::string,std::string>("u16StepTorqueWindow","INTEGER"));   // colunm_index = 14
    type.insert(std::pair<std::string,std::string>("u16MinDutyCycle","REAL"));          // colunm_index = 15
    type.insert(std::pair<std::string,std::string>("u16StepFlags","INTEGER"));          // colunm_index = 16
    type.insert(std::pair<std::string,std::string>("ScrewHiTorque","REAL"));            // colunm_index = 17
    type.insert(std::pair<std::string,std::string>("ScrewLoTorque","REAL"));            // colunm_index = 18
    type.insert(std::pair<std::string,std::string>("ScrewHiAngle","REAL"));             // colunm_index = 19
    type.insert(std::pair<std::string,std::string>("ScrewLoAngle","REAL"));             // colunm_index = 20
    type.insert(std::pair<std::string,std::string>("TorqueThreshold","REAL"));          // colunm_index = 21
    type.insert(std::pair<std::string,std::string>("AngleThreshold","REAL"));           // colunm_index = 22
    type.insert(std::pair<std::string,std::string>("TorqueJointOffset","REAL"));        // colunm_index = 23
    type.insert(std::pair<std::string,std::string>("StepDelaytime","REAL"));            // colunm_index = 24
    type.insert(std::pair<std::string,std::string>("ScrewStepDirection","INTEGER"));    // colunm_index = 25
    type.insert(std::pair<std::string,std::string>("StepMonitoringMode","INTEGER"));    // colunm_index = 26
    type.insert(std::pair<std::string,std::string>("off_set","INTEGER"));               // colunm_index = 27
    type.insert(std::pair<std::string,std::string>("ScrewReverseForce","INTEGER"));     // colunm_index = 28
    type.insert(std::pair<std::string,std::string>("u16WindowModeFlags","INTEGER"));    // colunm_index = 29
    type.insert(std::pair<std::string,std::string>("enable_downshift","INTEGER"));      // colunm_index = 30
    type.insert(std::pair<std::string,std::string>("downshift_torque","INTEGER"));      // colunm_index = 31
    type.insert(std::pair<std::string,std::string>("downshift_speed","INTEGER"));       // colunm_index = 32
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseStepInfo ::InitialColumnName()
{
    columnnames.push_back("job_id");                // colunm_index = 0
    columnnames.push_back("seq_id");                // colunm_index = 1
    columnnames.push_back("target_type");           // colunm_index = 2
    columnnames.push_back("program_name");          // colunm_index = 3
    columnnames.push_back("ScrewStepID");           // colunm_index = 4
    columnnames.push_back("u8StepName");            // colunm_index = 5
    columnnames.push_back("u16StepRpm");            // colunm_index = 6
    columnnames.push_back("u16StepSlope");          // colunm_index = 7
    columnnames.push_back("u16StepMaxCurrent");     // colunm_index = 8
    columnnames.push_back("u16StepMaxTorque");      // colunm_index = 9 
    columnnames.push_back("u16StepMaxRevol");       // colunm_index = 10 
    columnnames.push_back("u16StepTime");           // colunm_index = 11
    columnnames.push_back("u16StepAngle");          // colunm_index = 12
    columnnames.push_back("u16StepAngleWindow");    // colunm_index = 13
    columnnames.push_back("u16StepTorqueWindow");   // colunm_index = 14
    columnnames.push_back("u16MinDutyCycle");       // colunm_index = 15
    columnnames.push_back("u16StepFlags");          // colunm_index = 16
    columnnames.push_back("ScrewHiTorque");         // colunm_index = 17
    columnnames.push_back("ScrewLoTorque");         // colunm_index = 18
    columnnames.push_back("ScrewHiAngle");          // colunm_index = 19
    columnnames.push_back("ScrewLoAngle");          // colunm_index = 20
    columnnames.push_back("TorqueThreshold");       // colunm_index = 21
    columnnames.push_back("AngleThreshold");        // colunm_index = 22
    columnnames.push_back("TorqueJointOffset");     // colunm_index = 23
    columnnames.push_back("StepDelaytime");         // colunm_index = 24
    columnnames.push_back("ScrewStepDirection");    // colunm_index = 25
    columnnames.push_back("StepMonitoringMode");    // colunm_index = 26
    columnnames.push_back("off_set");               // colunm_index = 27
    columnnames.push_back("ScrewReverseForce");     // colunm_index = 28
    columnnames.push_back("u16WindowModeFlags");    // colunm_index = 29
    columnnames.push_back("enable_downshift");      // colunm_index = 30
    columnnames.push_back("downshift_torque");      // colunm_index = 31
    columnnames.push_back("downshift_speed");       // colunm_index = 32
}
#pragma endregion
