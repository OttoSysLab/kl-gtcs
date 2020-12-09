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
GtcsAMSProtocol* GtcsAMSProtocol::getInstance(){
    if(instance == 0){
        instance = new GtcsAMSProtocol();
    }
    return instance;
}
// Get 

// Get AMS Protocl string from bulletin.
std::string GtcsAMSProtocol::getAMSBulletinData(AMSCMD amscmd){
    switch (amscmd)
    {
        case AMSCMD::CMD300:
            //  *prt_cmd300 = &amsBulletin.cmd300;
            break;    
        default:
            break;
    }
    return "--";
}

// Tdd.
int main(){
    // Test Singleton.
    GtcsAMSProtocol* gtcs_1 = GtcsAMSProtocol::getInstance();
    std::cout<<"GTCS_1 cmd300 str2s = "<<gtcs_1->amsBulletin.CMD300Struct.str2<<std::endl;
    gtcs_1->amsBulletin.CMD300Struct.str2 = "Fuck your family!!";
    GtcsAMSProtocol* gtcs_2 = GtcsAMSProtocol::getInstance();
    std::cout<<"GTCS_2 cmd300 str2 = "<<gtcs_2->amsBulletin.CMD300Struct.str2<<std::endl;
    
    AmsCMD300Struct* prt_com300 = &gtcs_2->amsBulletin.CMD300Struct;
    std::cout<<std::to_string(sizeof(gtcs_2->amsBulletin.CMD300Struct))<<std::endl;
    std::cout<<"Prt str2= "<<prt_com300->str2<<std::endl;
    
    std::string* str_1 = (std::string *)(void *)prt_com300;
    std::cout << *str_1 <<std::endl;
    std::string* str_2 = (std::string *)(void *)(str_1+1);
    std::cout << *str_2 <<std::endl;
    std::string* str_3 = (std::string *)(void *)(str_2+1);
    std::cout << *str_3 <<std::endl;
    std::string* str_4 = (std::string *)(void *)(str_3+1);
    std::cout << *str_4 <<std::endl;
    std::string* str_5 = (std::string *)(void *)(str_4+1); 
    std::cout << *str_5 <<std::endl;
    
    return 0;
}
