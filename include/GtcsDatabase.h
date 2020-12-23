#ifndef _GTCS_DATABASE_
#define _GTCS_DATABASE_
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
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sqlite3.h>
#include <list>
#include <iostream>
#include "GtcsTypeDefine.h"
#include "GtcsBulletin.h"

// Gtcs database struct.
class GtcsDatabase
{
private:
    /* data */
    std::list<std::string> data;
public:
    // Constructor.
    GtcsDatabase(/* args */){};
    ~GtcsDatabase(){};
    // Read basic from database.
    int ReadDatabase(std::string dbPath,std::string table,std::string *ptr);
    int WriteDatabase(std::string dbPath,std::string table,std::string *ptr);
};
#endif
