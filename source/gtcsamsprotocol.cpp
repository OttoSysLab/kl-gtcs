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
#include "../include/gtcsamsprotocol.h"
// Signalton.
// Constructor.
GtcsAMSProtocol::GtcsAMSProtocol(/* args */)
{}
// Distructor.
GtcsAMSProtocol::~GtcsAMSProtocol()
{}
// SignleTon instance object.
GtcsAMSProtocol* GtcsAMSProtocol::instance = 0;
// Get Instance.
GtcsAMSProtocol* GtcsAMSProtocol::GetInstance(){
    if(instance == 0){
        instance = new GtcsAMSProtocol();
    }
    return instance;
}

// Get AMS string 
void GtcsAMSProtocol::ConvertToProtocolString(std::string* prt,std::string & result){
    // std::string result = "";
    result = "{"+*prt;
    do{
        prt = (std::string *)(void *)(prt+1);
        result += ","+*prt;
    }
    while(*prt != "\n\r");
    result += "}";
    // std::cout<<result<<std::endl;
    // return result;
}

// Get AMS Protocl string from bulletin.
std::string GtcsAMSProtocol::GetAMSBulletinData(AMSCMD amscmd){
    std::string result;      
    std::string* prt_str;    // 接頭
    switch (amscmd)
    {
    case AMSCMD::DATA300:     
        prt_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
        ConvertToProtocolString(prt_str,result);    
        // ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA300Struct,result);
        break; 
    case AMSCMD::DATA302:
        // prt_str = (std::string *)(void *)&amsBulletin.DATA302Struct;
        // ConvertToProtocolString(prt_str,result);
        break;        
    }
     std::cout<<result<<std::endl;
    return result;
}

// Tdd.
int main(){
    // // Test Singleton.
    GtcsAMSProtocol* ams = GtcsAMSProtocol::GetInstance();
    ams->GetAMSBulletinData(AMSCMD::DATA300);
    return 0;
}
