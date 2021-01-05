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
#include "GtcsGloabDefine.h"

#pragma region database base.
GtcsDatabaseBaseInfo::GtcsDatabaseBaseInfo()
{}
GtcsDatabaseBaseInfo::~GtcsDatabaseBaseInfo()
{}
void GtcsDatabaseBaseInfo::InitialDataStruct()
{ 
    int columnnames_size = columnnames.size();
    for (int i = 0; i < columnnames_size; i++)
    {
        data.insert(std::pair<std::string,std::string>(columnnames[i],"-"));
    }        
}
void GtcsDatabaseBaseInfo::SetDataValue(std::string *ptr)
{
    // std::cout << "======================SetDataValue====================" << std::endl;
    data[columnnames[0]] = *ptr;
    // std::cout<<"Index = "<< "0 " << columnnames[0] << " = " <<*ptr<< std::endl;
    int columnnames_size = columnnames.size();

    ptr = (std::string *)(void *)(ptr+1);
    for (int i = 1; i < columnnames_size; i++)
    {
        data[columnnames[i]] = *ptr;
        // std::cout <<"Index = "<<i<<" "<< columnnames[i] << " = " <<*ptr<< std::endl;
        ptr = (std::string *)(void *)(ptr+1);
    }       
}
#pragma endregion

#pragma region basic database object 
// Constructor.
GtcsDatabaseBasicInfo::GtcsDatabaseBasicInfo()
{
    InitialColumnName();
    InitialDataStruct();
    InitialTypeList();
}
// Distructor.
GtcsDatabaseBasicInfo::~GtcsDatabaseBasicInfo()
{}
// Initial basic columnName
void GtcsDatabaseBasicInfo::InitialColumnName()
{
    columnnames.push_back("mintemp");         // Min temperature       (REAL)
    columnnames.push_back("maxtemp");         // Max temperature       (REAL)
    columnnames.push_back("maxcurrent");      // Max current           (REAL)
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
    columnnames.push_back("[motswver ]");         // (TEXT)
    // columnnames.push_back("end");           // CL,RF  
}
void GtcsDatabaseBasicInfo::InitialTypeList()
{    
    type.insert(std::pair<std::string,std::string>("mintemp","REAL"));            // Min temperature       (REAL)
    type.insert(std::pair<std::string,std::string>("maxtemp","REAL"));            // Max temperature       (REAL)
    type.insert(std::pair<std::string,std::string>("maxcurrent","INTEGER"));      // Max current           (REAL)
    type.insert(std::pair<std::string,std::string>("maxpeakcurrent","INTEGER"));  // Max peak current      (INTEGER)
    type.insert(std::pair<std::string,std::string>("torquesensortype","INTEGER"));// torque sensor type    (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxdutycycle","REAL"));    // Max duty cycle        (REAL)
    type.insert(std::pair<std::string,std::string>("maxtorque","REAL"));       // Max torque            (REAL)
    type.insert(std::pair<std::string,std::string>("pwmfreq","INTEGER"));         // PWM frequency         (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxrpm","INTEGER"));          // Max rpm               (INTEGER)
    type.insert(std::pair<std::string,std::string>("maxslope","INTEGER"));        // Max slope             (INTEGER)
    type.insert(std::pair<std::string,std::string>("minbusvolt","REAL"));      // Min bus voltage       (REAL)
    type.insert(std::pair<std::string,std::string>("maxbusvolt","REAL"));      // Max bus voltage       (REAL)
    type.insert(std::pair<std::string,std::string>("startdutycycle","REAL"));  // Start duty cycle      (REAL)
    type.insert(std::pair<std::string,std::string>("gearboxratio","REAL"));    // Gear box ratio        (REAL)
    type.insert(std::pair<std::string,std::string>("startinp","INTEGER"));        // Start input source    (INTEGER)
    type.insert(std::pair<std::string,std::string>("revinp","INTEGER"));          // Reverse ipnut source  (INTEGER)
    type.insert(std::pair<std::string,std::string>("revrpm","INTEGER"));          // Reverse rpm           (INTEGER)
    type.insert(std::pair<std::string,std::string>("revslope","INTEGER"));        // Reverse slope         (INTEGER)
    type.insert(std::pair<std::string,std::string>("revmaxcurrent","INTEGER"));   // Reverse max current   (INTEGER)
    type.insert(std::pair<std::string,std::string>("revmaxtorque","REAL"));    // Reverse max torque    (REAL)
    type.insert(std::pair<std::string,std::string>("erroridletime","INTEGER"));   // Error idle time       (INTEGER)
    type.insert(std::pair<std::string,std::string>("backlash","INTEGER"));        // Bachlash              (INTEGER)
    type.insert(std::pair<std::string,std::string>("pgain","INTEGER"));           // Proportional gain     (INTEGER)
    type.insert(std::pair<std::string,std::string>("igain","INTEGER"));           // Integral gain         (INTEGER)
    type.insert(std::pair<std::string,std::string>("encoder","INTEGER"));         // Encoder               (INTEGER)
    // New add
    type.insert(std::pair<std::string,std::string>("mintorque","REAL"));        // (REAL)
    type.insert(std::pair<std::string,std::string>("minrpm","INTEGER"));           // (INTEGER)
    type.insert(std::pair<std::string,std::string>("revminrpm","INTEGER"));        // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmsswver","INTEGER"));         // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmscoreid","INTEGER"));        // (INTEGER)
    type.insert(std::pair<std::string,std::string>("dmssernr","INTEGER"));         // (INTEGER)
    type.insert(std::pair<std::string,std::string>("led","INTEGER"));              // (INTEGER)
    type.insert(std::pair<std::string,std::string>("lever_sensitivity","INTEGER"));// (INTEGER)
    type.insert(std::pair<std::string,std::string>("push_sensitivity","INTEGER")); // (INTEGER) 
    type.insert(std::pair<std::string,std::string>("[motswver ]","TEXT"));         // (TEXT)
}
std::string GtcsDatabaseBasicInfo::GetUpdateSqlCommand()
{
    std::string sqlcmd = "";
    sqlcmd = "update " + dbtablename + " set ";
    int columnnames_size  = columnnames.size();
    for (int i = 0; i < columnnames_size; i++)
    {
        if (type[columnnames[i]]!="TEXT")
        {
            sqlcmd += columnnames[i] + " = " + data[columnnames[i]] + ",";   
        }   
        else
        {
            sqlcmd += columnnames[i] + " = " + "'" + data[columnnames[i]] + "'" +",";   
        }
    }
    sqlcmd = sqlcmd.replace(sqlcmd.end()-1,sqlcmd.end()," ");
    sqlcmd += "where rowid = 1;";
    return sqlcmd;
}
#pragma endregion
