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
    // result = "{"+*prt;
    // do{
    //     prt = (std::string *)(void *)(prt+1);
    //     result += ","+*prt;
    // }
    // while(*prt != "\n\r");
    // result += "}";
    // std::cout<<result<<std::endl;
    // return result;
    result = "{"+*prt;
    while(true){
        prt = (std::string *)(void *)(prt+1);
        if (*prt != "\n\r"){
            result += ","+*prt;
        }
        else{
            break;
        }
    }
    result += "}";
}

// Get AMS Protocl string from bulletin.
std::string GtcsAMSProtocol::GetAMSBulletinData(AMSCMD amscmd){
    std::string result;      
    std::string* prt_str;    // 接頭
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:     
        prt_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
        ConvertToProtocolString(prt_str,result);    
        // ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA300Struct,result);
        break; 
    case AMSCMD::DATA302:
        prt_str = (std::string *)(void *)&amsBulletin.DATA302Struct;
        ConvertToProtocolString(prt_str,result);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        prt_str = (std::string *)(void *)&amsBulletin.REQ300Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::REQ301:
        prt_str = (std::string *)(void *)&amsBulletin.REQ301Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::REQ302:
        prt_str = (std::string *)(void *)&amsBulletin.REQ302Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        prt_str = (std::string *)(void *)&amsBulletin.CMD300Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD301:
        prt_str = (std::string *)(void *)&amsBulletin.CMD301Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD302:
        prt_str = (std::string *)(void *)&amsBulletin.CMD302Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD303:
        prt_str = (std::string *)(void *)&amsBulletin.CMD303Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD310:
        prt_str = (std::string *)(void *)&amsBulletin.CMD310Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD311:
        prt_str = (std::string *)(void *)&amsBulletin.CMD311Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD312:
        prt_str = (std::string *)(void *)&amsBulletin.CMD312Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD320:
        prt_str = (std::string *)(void *)&amsBulletin.CMD320Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD321:
        prt_str = (std::string *)(void *)&amsBulletin.CMD321Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD322:
        prt_str = (std::string *)(void *)&amsBulletin.CMD322Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD323:
        prt_str = (std::string *)(void *)&amsBulletin.CMD323Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD330:
        prt_str = (std::string *)(void *)&amsBulletin.CMD330Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::CMD340:
        prt_str = (std::string *)(void *)&amsBulletin.CMD340Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        prt_str = (std::string *)(void *)&amsBulletin.ANS301Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS302:
        prt_str = (std::string *)(void *)&amsBulletin.ANS302Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS310:
        prt_str = (std::string *)(void *)&amsBulletin.ANS310Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS311:
        prt_str = (std::string *)(void *)&amsBulletin.ANS311Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS320:
        prt_str = (std::string *)(void *)&amsBulletin.ANS320Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS321:
        prt_str = (std::string *)(void *)&amsBulletin.ANS321Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS322:
        prt_str = (std::string *)(void *)&amsBulletin.ANS322Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS323:
        prt_str = (std::string *)(void *)&amsBulletin.ANS323Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS330:
        prt_str = (std::string *)(void *)&amsBulletin.ANS330Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    case AMSCMD::ANS340:
        prt_str = (std::string *)(void *)&amsBulletin.ANS340Struct;
        ConvertToProtocolString(prt_str,result);
        break;
    default:
        break;
    #pragma endregion
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
