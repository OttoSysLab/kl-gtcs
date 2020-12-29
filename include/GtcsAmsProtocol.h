#ifndef _GTCS_AMS_PROTOCOL_
#define _GTCS_AMS_PROTOCOL_

#pragma once

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
#include <map>
#include <iostream>
#include <cstring>
#include <vector>
#include "GtcsGloabDefine.h"
#include "GtcsBulletin.h"

#pragma region AMS Protocol object
// Gtcs AMS Protocol
class GtcsAmsProtocol
{
private: 
    // SignleTon instance object.
    static GtcsAmsProtocol* instance;
    // Constructor.
    GtcsAmsProtocol(/* args */);
    int ConvertToProtocolString(std::string* ptr,std::string& result);
    int UpdateProtocolStruct(std::string* ptr,std::vector<std::string>& ams_array);
    std::vector<std::string> SplitString(const std::string & str,const std::string& pattern);
    std::vector<std::string> GetAmsSpliteArray(const std::string & str);
    int SetAmsBulletin(std::string ams_string);
public:    
    int cmdsn = 0; 
    ~GtcsAmsProtocol();
    static GtcsAmsProtocol* GetInstance();
    // AMSBulletin amsprotocol;
    int GetAmsCmdNum(std::string amscmd);
    std::string GetAmsBulletin(int amscmd);
    int CheckRequestStatus(std::string requestcmd);
};
#pragma endregion
#endif