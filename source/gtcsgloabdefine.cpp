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
    type.insert(std::pair<std::string,std::string>("program_id","INTEGER"));      
    type.insert(std::pair<std::string,std::string>("program_name","TEXT"));       
    type.insert(std::pair<std::string,std::string>("first_step","INTEGER"));      
    type.insert(std::pair<std::string,std::string>("second_step","INTEGER"));    
    type.insert(std::pair<std::string,std::string>("third_step","INTEGER"));     
    type.insert(std::pair<std::string,std::string>("fourth_step","INTEGER"));       
    type.insert(std::pair<std::string,std::string>("fifth_step","INTEGER"));         
    type.insert(std::pair<std::string,std::string>("sixth_step","INTEGER"));         
    type.insert(std::pair<std::string,std::string>("seventh_step","INTEGER"));         
    type.insert(std::pair<std::string,std::string>("eighth_step","INTEGER"));        
    type.insert(std::pair<std::string,std::string>("ok_time","INTEGER"));         
    type.insert(std::pair<std::string,std::string>("ng_stop","INTEGER"));         
    type.insert(std::pair<std::string,std::string>("pgain","INTEGER"));     
    type.insert(std::pair<std::string,std::string>("igain","INTEGER"));       
    type.insert(std::pair<std::string,std::string>("maxtime","INTEGER"));        
    type.insert(std::pair<std::string,std::string>("mintime","INTEGER"));          
    type.insert(std::pair<std::string,std::string>("reversefunction","INTEGER"));          
    type.insert(std::pair<std::string,std::string>("numbersteps","INTEGER"));        
    type.insert(std::pair<std::string,std::string>("joint_offset","REAL"));       
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
    columnnames.push_back("program_id");   
    columnnames.push_back("program_name"); 
    columnnames.push_back("first_step");   
    columnnames.push_back("second_step");  
    columnnames.push_back("third_step");   
    columnnames.push_back("fourth_step");   
    columnnames.push_back("fifth_step");    
    columnnames.push_back("sixth_step");    
    columnnames.push_back("seventh_step");  
    columnnames.push_back("eighth_step");   
    columnnames.push_back("ok_time");      
    columnnames.push_back("ng_stop");      
    columnnames.push_back("pgain");           
    columnnames.push_back("igain");    
    columnnames.push_back("maxtime");        
    columnnames.push_back("mintime");          
    columnnames.push_back("reversefunction");
    columnnames.push_back("numbersteps");    
    columnnames.push_back("joint_offset");   
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
GtcsDatabaseStepInfo ::GtcsDatabaseStepInfo (/* args */)
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
GtcsDatabaseStepInfo ::~GtcsDatabaseStepInfo ()
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
    type.insert(std::pair<std::string,std::string>("ScrewStepID","INTEGER"));            
    type.insert(std::pair<std::string,std::string>("u8StepName","TEXT"));            
    type.insert(std::pair<std::string,std::string>("u16StepRpm","INTEGER"));        
    type.insert(std::pair<std::string,std::string>("u16StepSlope","REAL"));         
    type.insert(std::pair<std::string,std::string>("u16StepMaxCurrent","INTEGER"));
    type.insert(std::pair<std::string,std::string>("u16StepMaxTorque","REAL"));    
    type.insert(std::pair<std::string,std::string>("u16StepMaxRevol","REAL"));     
    type.insert(std::pair<std::string,std::string>("u16StepTime","REAL"));         
    type.insert(std::pair<std::string,std::string>("u16StepAngle","REAL"));        
    type.insert(std::pair<std::string,std::string>("u16StepAngleWindow","INTEGER"));  
    type.insert(std::pair<std::string,std::string>("u16StepTorqueWindow","INTEGER")); 
    type.insert(std::pair<std::string,std::string>("u16MinDutyCycle","REAL"));        
    type.insert(std::pair<std::string,std::string>("u16StepFlags","INTEGER"));        
    type.insert(std::pair<std::string,std::string>("ScrewHiTorque","REAL"));        
    type.insert(std::pair<std::string,std::string>("ScrewLoTorque","REAL"));        
    type.insert(std::pair<std::string,std::string>("ScrewHiAngle","REAL"));         
    type.insert(std::pair<std::string,std::string>("ScrewLoAngle","REAL"));         
    type.insert(std::pair<std::string,std::string>("TorqueThreshold","REAL"));      
    type.insert(std::pair<std::string,std::string>("AngleThreshold","REAL"));       
    type.insert(std::pair<std::string,std::string>("TorqueJointOffset","REAL"));    
    type.insert(std::pair<std::string,std::string>("StepDelaytime","REAL"));         
    type.insert(std::pair<std::string,std::string>("ScrewStepDirection","INTEGER")); 
    type.insert(std::pair<std::string,std::string>("StepMonitoringMode","INTEGER")); 
    type.insert(std::pair<std::string,std::string>("off_set","INTEGER"));           
    type.insert(std::pair<std::string,std::string>("ScrewReverseForce","INTEGER"));  
    type.insert(std::pair<std::string,std::string>("u16WindowModeFlags","INTEGER")); 
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
    columnnames.push_back("ScrewStepID");   
    columnnames.push_back("u8StepName"); 
    columnnames.push_back("u16StepRpm");   
    columnnames.push_back("u16StepSlope");  
    columnnames.push_back("u16StepMaxCurrent");   
    columnnames.push_back("u16StepMaxTorque");   
    columnnames.push_back("u16StepMaxRevol");    
    columnnames.push_back("u16StepTime");    
    columnnames.push_back("u16StepAngle");  
    columnnames.push_back("u16StepAngleWindow");   
    columnnames.push_back("u16StepTorqueWindow");      
    columnnames.push_back("u16MinDutyCycle");      
    columnnames.push_back("u16StepFlags");           
    columnnames.push_back("ScrewHiTorque");    
    columnnames.push_back("ScrewLoTorque");        
    columnnames.push_back("ScrewHiAngle");          
    columnnames.push_back("ScrewLoAngle");
    columnnames.push_back("TorqueThreshold");    
    columnnames.push_back("AngleThreshold");   
    columnnames.push_back("TorqueJointOffset");   
    columnnames.push_back("StepDelaytime");   
    columnnames.push_back("ScrewStepDirection");   
    columnnames.push_back("StepMonitoringMode");   
    columnnames.push_back("off_set");   
    columnnames.push_back("ScrewReverseForce");   
    columnnames.push_back("u16WindowModeFlags");   
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
 *  @note    none
 *
 *******************************************************************************************/
// Initial basic columnName
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
#pragma endregion
