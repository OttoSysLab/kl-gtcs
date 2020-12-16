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
// Get MCB realy time status string.

// Get Instance.
int GtcsBulletinManager::ConvertActuralData300(GtcsStatusTelegramStrcut *ptr)
{
    int result = -1;
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();  
    AmsDATA300Struct *data300 = &bulletin->AmsBulletin.DATA300Struct;  
    // time.
    time_t now = time(0);
    tm *ltm = localtime(&now);
    data300->str2 = std::to_string(ltm->tm_year)+
                    std::to_string(ltm->tm_mon) +
                    std::to_string(ltm->tm_mday)+
                    '_'+
                    std::to_string(ltm->tm_hour)+
                    ':'+
                    std::to_string(ltm->tm_min)+
                    ':'+
                    std::to_string(ltm->tm_sec);
    data300->str10 = std::to_string(ptr->u16ActProcNr);                 //            
    data300->str11 = std::to_string(ptr->u16ActStepNr);                 // 
    data300->str18 = std::to_string(((float)ptr->u16ActTorque/1862)*5);
    data300->str19 = std::to_string((ptr->u32Angle));
    data300->str20 = std::to_string(((float)ptr->u16MaxTorque/1862)*5);
    data300->str21 = std::to_string((ptr->u32Revolutions));    

    return result;
}
