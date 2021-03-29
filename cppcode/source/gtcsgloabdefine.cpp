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
 *  @fn      GtcsDatabaseBasicInfo::GtcsDatabaseBasicInfo()
 *
 *  @brief   GtcsDatabaseBasicInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseBasicInfo::GtcsDatabaseBasicInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/03/15
 *
 *  @fn      GtcsDatabaseDeviceInfo::GtcsDatabaseDeviceInfo()
 *
 *  @brief   GtcsDatabaseDeviceInfo contructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseDeviceInfo::GtcsDatabaseDeviceInfo()
{
    InitialColumnType();
    InitialColumnName();
    InitialDataStruct();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/15
 *
 *  @fn      GtcsDatabaseDeviceInfo::~GtcsDatabaseDeviceInfo()
 *
 *  @brief   GtcsDatabaseDeviceInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseDeviceInfo::~GtcsDatabaseDeviceInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/15
 *
 *  @fn      GtcsDatabaseDeviceInfo::InitialColumnType()
 *
 *  @brief   GtcsDatabaseDeviceInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseDeviceInfo::InitialColumnType()
{
    type.insert(std::pair<std::string,std::string>("device_sn","TEXT"));            // column_index  = 0
    type.insert(std::pair<std::string,std::string>("device_version","TEXT"));       // column_index  = 1
    type.insert(std::pair<std::string,std::string>("device_id","INTEGER"));         // column_index  = 2
    type.insert(std::pair<std::string,std::string>("device_type","INTEGER"));       // column_index  = 3
    type.insert(std::pair<std::string,std::string>("batch_mode","INTEGER"));        // column_index  = 4
    type.insert(std::pair<std::string,std::string>("operation_mode","INTEGER"));    // column_index  = 5
    type.insert(std::pair<std::string,std::string>("mode","INTEGER"));              // column_index  = 6
    type.insert(std::pair<std::string,std::string>("net_work","INTEGER"));          // column_index  = 7
    type.insert(std::pair<std::string,std::string>("torque_unit","INTEGER"));       // column_index  = 8
    type.insert(std::pair<std::string,std::string>("iom_torque","REAL"));           // column_index  = 9
    type.insert(std::pair<std::string,std::string>("language","INTEGER"));          // column_index  = 10
    type.insert(std::pair<std::string,std::string>("target_torque","REAL"));        // column_index  = 11
    type.insert(std::pair<std::string,std::string>("ip","TEXT"));                   // column_index  = 12
    type.insert(std::pair<std::string,std::string>("hq","REAL"));                   // column_index  = 13
    type.insert(std::pair<std::string,std::string>("lq","REAL"));                   // column_index  = 14
    type.insert(std::pair<std::string,std::string>("tolerance","INTEGER"));         // column_index  = 15
    type.insert(std::pair<std::string,std::string>("time_out","REAL"));             // column_index  = 16
    type.insert(std::pair<std::string,std::string>("screw_direction","INTEGER"));   // column_index  = 17
    type.insert(std::pair<std::string,std::string>("screw_rpm","INTEGER"));         // column_index  = 18
    type.insert(std::pair<std::string,std::string>("screw_count","REAL"));          // column_index  = 19
    type.insert(std::pair<std::string,std::string>("reverse_direction","INTEGER")); // column_index  = 20
    type.insert(std::pair<std::string,std::string>("reverse_rpm","INTEGER"));       // column_index  = 21
    type.insert(std::pair<std::string,std::string>("force","INTEGER"));             // column_index  = 22
    type.insert(std::pair<std::string,std::string>("screw_trigger","INTEGER"));     // column_index  = 23
    type.insert(std::pair<std::string,std::string>("reverse_trigger","INTEGER"));   // column_index  = 24
    // New add
    type.insert(std::pair<std::string,std::string>("torque_threshold","REAL"));     // column_index  = 25
    type.insert(std::pair<std::string,std::string>("angle_threshold","REAL"));      // column_index  = 26
    type.insert(std::pair<std::string,std::string>("iom_torquethreshold","REAL"));  // column_index  = 27
    type.insert(std::pair<std::string,std::string>("torque_offset","REAL"));        // column_index  = 28
    type.insert(std::pair<std::string,std::string>("iom_torqueoffset","REAL"));     // column_index  = 29
    type.insert(std::pair<std::string,std::string>("off_set","INTEGER"));           // column_index  = 30
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/15
 *
 *  @fn      GtcsDatabaseDeviceInfo::InitialColumnName()
 *
 *  @brief   GtcsDatabaseDeviceInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsDatabaseDeviceInfo::InitialColumnName()
{
    columnnames.push_back("device_sn");                 // column_index  = 0
    columnnames.push_back("device_version");            // column_index  = 1
    columnnames.push_back("device_id");                 // column_index  = 2
    columnnames.push_back("device_type");               // column_index  = 3
    columnnames.push_back("batch_mode");                // column_index  = 4
    columnnames.push_back("operation_mode");            // column_index  = 5
    columnnames.push_back("mode");                      // column_index  = 6
    columnnames.push_back("net_work");                  // column_index  = 7
    columnnames.push_back("torque_unit");               // column_index  = 8
    columnnames.push_back("iom_torque");                // column_index  = 9
    columnnames.push_back("language");                  // column_index  = 10
    columnnames.push_back("target_torque");             // column_index  = 11
    columnnames.push_back("ip");                        // column_index  = 12
    columnnames.push_back("hq");                        // column_index  = 13
    columnnames.push_back("lq");                        // column_index  = 14
    columnnames.push_back("tolerance");                 // column_index  = 15
    columnnames.push_back("time_out");                  // column_index  = 16
    columnnames.push_back("screw_direction");           // column_index  = 17
    columnnames.push_back("screw_rpm");                 // column_index  = 18
    columnnames.push_back("screw_count");               // column_index  = 19
    columnnames.push_back("reverse_direction");         // column_index  = 20
    columnnames.push_back("reverse_rpm");               // column_index  = 21
    columnnames.push_back("force");                     // column_index  = 22
    columnnames.push_back("screw_trigger");             // column_index  = 23
    columnnames.push_back("reverse_trigger");           // column_index  = 24
    // New add
    columnnames.push_back("torque_threshold");          // column_index  = 25
    columnnames.push_back("angle_threshold");           // column_index  = 26
    columnnames.push_back("iom_torquethreshold");       // column_index  = 27
    columnnames.push_back("torque_offset");             // column_index  = 28
    columnnames.push_back("iom_torqueoffset");          // column_index  = 29
    columnnames.push_back("off_set");                   // column_index  = 30
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBasicInfo::~GtcsDatabaseBasicInfo()
 *
 *  @brief   GtcsDatabaseBasicInfo distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseBasicInfo::~GtcsDatabaseBasicInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBasicInfo::InitialColumnType()
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
void GtcsDatabaseBasicInfo::InitialColumnType()
{    
    type.insert(std::pair<std::string,std::string>("mintemp","REAL"));            // column_index  = 1
    type.insert(std::pair<std::string,std::string>("maxtemp","REAL"));            // column_index  = 2
    type.insert(std::pair<std::string,std::string>("maxcurrent","INTEGER"));      // column_index  = 3
    type.insert(std::pair<std::string,std::string>("maxpeakcurrent","INTEGER"));  // column_index  = 4
    type.insert(std::pair<std::string,std::string>("torquesensortype","INTEGER"));// column_index  = 5
    type.insert(std::pair<std::string,std::string>("maxdutycycle","REAL"));       // column_index  = 6
    type.insert(std::pair<std::string,std::string>("maxtorque","REAL"));          // column_index  = 7
    type.insert(std::pair<std::string,std::string>("pwmfreq","INTEGER"));         // column_index  = 8
    type.insert(std::pair<std::string,std::string>("maxrpm","INTEGER"));          // column_index  = 9
    type.insert(std::pair<std::string,std::string>("maxslope","INTEGER"));        // column_index  = 10
    type.insert(std::pair<std::string,std::string>("minbusvolt","REAL"));         // column_index  = 11
    type.insert(std::pair<std::string,std::string>("maxbusvolt","REAL"));         // column_index  = 12
    type.insert(std::pair<std::string,std::string>("startdutycycle","REAL"));     // column_index  = 13
    type.insert(std::pair<std::string,std::string>("gearboxratio","REAL"));       // column_index  = 14
    type.insert(std::pair<std::string,std::string>("startinp","INTEGER"));        // column_index  = 15
    type.insert(std::pair<std::string,std::string>("revinp","INTEGER"));          // column_index  = 16
    type.insert(std::pair<std::string,std::string>("revrpm","INTEGER"));          // column_index  = 17
    type.insert(std::pair<std::string,std::string>("revslope","INTEGER"));        // column_index  = 18
    type.insert(std::pair<std::string,std::string>("revmaxcurrent","INTEGER"));   // column_index  = 19
    type.insert(std::pair<std::string,std::string>("revmaxtorque","REAL"));       // column_index  = 20
    type.insert(std::pair<std::string,std::string>("erroridletime","INTEGER"));   // column_index  = 21
    type.insert(std::pair<std::string,std::string>("backlash","INTEGER"));        // column_index  = 22
    type.insert(std::pair<std::string,std::string>("pgain","INTEGER"));           // column_index  = 23
    type.insert(std::pair<std::string,std::string>("igain","INTEGER"));           // column_index  = 24
    type.insert(std::pair<std::string,std::string>("encoder","INTEGER"));         // column_index  = 25
    // New add
    // type.insert(std::pair<std::string,std::string>("mintorque","REAL"));          // (REAL)
    // type.insert(std::pair<std::string,std::string>("minrpm","INTEGER"));           // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("revminrpm","INTEGER"));        // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("dmsswver","INTEGER"));         // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("dmscoreid","INTEGER"));        // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("dmssernr","INTEGER"));         // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("led","INTEGER"));              // (INTEGER)
    // type.insert(std::pair<std::string,std::string>("lever_sensitivity","INTEGER"));// (INTEGER)
    // type.insert(std::pair<std::string,std::string>("push_sensitivity","INTEGER")); // (INTEGER) 
    // type.insert(std::pair<std::string,std::string>("motswver","TEXT"));         // (TEXT)
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseBasicInfo::InitialColumnName()
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
void GtcsDatabaseBasicInfo::InitialColumnName()
{
    columnnames.push_back("mintemp");         // column_index  = 1
    columnnames.push_back("maxtemp");         // column_index  = 2
    columnnames.push_back("maxcurrent");      // column_index  = 3
    columnnames.push_back("maxpeakcurrent");  // column_index  = 4
    columnnames.push_back("torquesensortype");// column_index  = 5
    columnnames.push_back("maxdutycycle");    // column_index  = 6
    columnnames.push_back("maxtorque");       // column_index  = 7
    columnnames.push_back("pwmfreq");         // column_index  = 8
    columnnames.push_back("maxrpm");          // column_index  = 9
    columnnames.push_back("maxslope");        // column_index  = 10
    columnnames.push_back("minbusvolt");      // column_index  = 11
    columnnames.push_back("maxbusvolt");      // column_index  = 12
    columnnames.push_back("startdutycycle");  // column_index  = 13
    columnnames.push_back("gearboxratio");    // column_index  = 14
    columnnames.push_back("startinp");        // column_index  = 15
    columnnames.push_back("revinp");          // column_index  = 16
    columnnames.push_back("revrpm");          // column_index  = 17
    columnnames.push_back("revslope");        // column_index  = 18
    columnnames.push_back("revmaxcurrent");   // column_index  = 19
    columnnames.push_back("revmaxtorque");    // column_index  = 20
    columnnames.push_back("erroridletime");   // column_index  = 21
    columnnames.push_back("backlash");        // column_index  = 22
    columnnames.push_back("pgain");           // column_index  = 23
    columnnames.push_back("igain");           // column_index  = 24
    columnnames.push_back("encoder");         // column_index  = 25
    // New add
    // columnnames.push_back("mintorque");        // (REAL)
    // columnnames.push_back("minrpm");           // (INTEGER)
    // columnnames.push_back("revminrpm");        // (INTEGER)
    // columnnames.push_back("dmsswver");         // (INTEGER)
    // columnnames.push_back("dmscoreid");        // (INTEGER)
    // columnnames.push_back("dmssernr");         // (INTEGER)
    // columnnames.push_back("led");              // (INTEGER)
    // columnnames.push_back("lever_sensitivity");// (INTEGER)
    // columnnames.push_back("push_sensitivity"); // (INTEGER) 
    // columnnames.push_back("motswver");         // (TEXT)
    // columnnames.push_back("end");           // CL,RF  
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseJobInfo::GtcsDatabaseJobInfo()
 *
 *  @brief   GtcsDatabaseJobInfo constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseJobInfo::GtcsDatabaseJobInfo()
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
 *  @fn      GtcsDatabaseJobInfo::~GtcsDatabaseJobInfo()
 *
 *  @brief   GtcsDatabaseJobInfo distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseJobInfo::~GtcsDatabaseJobInfo()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseJobInfo::InitialColumnType()
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
void GtcsDatabaseJobInfo::InitialColumnType()
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
 *  @fn      GtcsDatabaseJobInfo::InitialColumnName()
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
void GtcsDatabaseJobInfo::InitialColumnName()
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
 *  @fn      GtcsDatabaseSequenceInfo ::GtcsDatabaseSequenceInfo()
 *
 *  @brief   GtcsDatabaseSequenceInfo constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseSequenceInfo ::GtcsDatabaseSequenceInfo (/* args */)
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
 *  @fn      GtcsDatabaseSequenceInfo ::~GtcsDatabaseSequenceInfo()
 *
 *  @brief   GtcsDatabaseSequenceInfo distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseSequenceInfo ::~GtcsDatabaseSequenceInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseSequenceInfo ::InitialColumnType()
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
void GtcsDatabaseSequenceInfo ::InitialColumnType()
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
 *  @fn      GtcsDatabaseSequenceInfo ::InitialColumnName()
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
void GtcsDatabaseSequenceInfo ::InitialColumnName()
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
 *  @fn      GtcsDatabaseStepInfo::GtcsDatabaseStepInfo ()
 *
 *  @brief   GtcsDatabaseStepInfo constructor.
 *
 *  @param   none
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseStepInfo::GtcsDatabaseStepInfo ()
 *
 *  @brief   GtcsDatabaseStepInfo constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsDatabaseStepInfo::~GtcsDatabaseStepInfo ()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsDatabaseStepInfo ::InitialColumnType()
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
void GtcsDatabaseStepInfo ::InitialColumnType()
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
 *  @fn      GtcsDatabaseStepInfo ::InitialColumnName()
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
void GtcsDatabaseStepInfo ::InitialColumnName()
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
