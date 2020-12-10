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
int GtcsAMSProtocol::ConvertToProtocolString(std::string* prt,std::string & result){
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
    return 0;
}
// 
int GtcsAMSProtocol::UpdateProtocolStruct(std::string* prt,std::string & result){
    return 0;
}
// Get AMS Protocol string from bulletin.
std::string GtcsAMSProtocol::GetAMSBulletin(AMSCMD amscmd){
    std::string result;      
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:     
        // prt_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
        // ConvertToProtocolString(prt_str,result);    
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA300Struct,result);
        break; 
    case AMSCMD::DATA302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA302Struct,result);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ300Struct,result);
        break;
    case AMSCMD::REQ301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ301Struct,result);
        break;
    case AMSCMD::REQ302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ302Struct,result);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD300Struct,result);
        break;
    case AMSCMD::CMD301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD301Struct,result);
        break;
    case AMSCMD::CMD302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD302Struct,result);
        break;
    case AMSCMD::CMD303:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD303Struct,result);
        break;
    case AMSCMD::CMD310:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD310Struct,result);
        break;
    case AMSCMD::CMD311:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD311Struct,result);
        break;
    case AMSCMD::CMD312:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD312Struct,result);
        break;
    case AMSCMD::CMD320:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD320Struct,result);
        break;
    case AMSCMD::CMD321:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD321Struct,result);
        break;
    case AMSCMD::CMD322:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD322Struct,result);
        break;
    case AMSCMD::CMD323:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD323Struct,result);
        break;
    case AMSCMD::CMD330:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD330Struct,result);
        break;
    case AMSCMD::CMD340:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD340Struct,result);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS301Struct,result);
        break;
    case AMSCMD::ANS302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS302Struct,result);
        break;
    case AMSCMD::ANS310:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS310Struct,result);
        break;
    case AMSCMD::ANS311:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS311Struct,result);
        break;
    case AMSCMD::ANS320:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS320Struct,result);
        break;
    case AMSCMD::ANS321:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS321Struct,result);
        break;
    case AMSCMD::ANS322:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS322Struct,result);
        break;
    case AMSCMD::ANS323:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS323Struct,result);
        break;
    case AMSCMD::ANS330:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS330Struct,result);
        break;
    case AMSCMD::ANS340:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS340Struct,result);
        break;
    default:
        break;
    #pragma endregion
    }
     std::cout<<result<<std::endl;
    return result;
}
// Set AMS Protocol struct to bulletin.
int GtcsAMSProtocol::SetAMSBulletin(std::string amsstring){
    std::string result;      
    std::string amscms = "";

    // switch (amscmd)
    // {
    // #pragma region DATA 
    // case "DATA300":     
    //     // prt_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
    //     // ConvertToProtocolString(prt_str,result);    
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.DATA300Struct,result);
    //     break; 
    // case "DATA302":
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.DATA302Struct,result);
    //     break;     
    // #pragma endregion
    // #pragma region REQ
    // case "REQ300":
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ300Struct,result);
    //     break;
    // case "REQ301":
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ301Struct,result);
    //     break;
    // case AMSCMD::REQ302:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ302Struct,result);
    //     break;
    // #pragma endregion  
    // #pragma region CMD
    // case AMSCMD::CMD300:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD300Struct,result);
    //     break;
    // case AMSCMD::CMD301:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD301Struct,result);
    //     break;
    // case AMSCMD::CMD302:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD302Struct,result);
    //     break;
    // case AMSCMD::CMD303:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD303Struct,result);
    //     break;
    // case AMSCMD::CMD310:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD310Struct,result);
    //     break;
    // case AMSCMD::CMD311:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD311Struct,result);
    //     break;
    // case AMSCMD::CMD312:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD312Struct,result);
    //     break;
    // case AMSCMD::CMD320:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD320Struct,result);
    //     break;
    // case AMSCMD::CMD321:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD321Struct,result);
    //     break;
    // case AMSCMD::CMD322:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD322Struct,result);
    //     break;
    // case AMSCMD::CMD323:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD323Struct,result);
    //     break;
    // case AMSCMD::CMD330:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD330Struct,result);
    //     break;
    // case AMSCMD::CMD340:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD340Struct,result);
    //     break;
    // #pragma endregion
    // #pragma region ANS
    // case AMSCMD::ANS301:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS301Struct,result);
    //     break;
    // case AMSCMD::ANS302:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS302Struct,result);
    //     break;
    // case AMSCMD::ANS310:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS310Struct,result);
    //     break;
    // case AMSCMD::ANS311:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS311Struct,result);
    //     break;
    // case AMSCMD::ANS320:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS320Struct,result);
    //     break;
    // case AMSCMD::ANS321:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS321Struct,result);
    //     break;
    // case AMSCMD::ANS322:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS322Struct,result);
    //     break;
    // case AMSCMD::ANS323:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS323Struct,result);
    //     break;
    // case AMSCMD::ANS330:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS330Struct,result);
    //     break;
    // case AMSCMD::ANS340:
    //     UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS340Struct,result);
    //     break;
    // default:
    //     break;
    // #pragma endregion
    // }
    // std::cout<<result<<std::endl;
    
    
    return 0;
}

// Tdd.
int main(){
    // // Test Singleton.
    GtcsAMSProtocol* ams = GtcsAMSProtocol::GetInstance();
    ams->GetAMSBulletin(AMSCMD::DATA300);
    return 0;
}
