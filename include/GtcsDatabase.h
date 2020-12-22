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
#include <iostream>
#include <sqlite3.h>

// Sqlite3 construct.
class Sqlite3Manager
{
private:
    /* data */
    std::string dbname = "";
    
public:
    Sqlite3Manager(/* args */){};
    ~Sqlite3Manager(){};
    int test();
};

// Gtcs database struct.
class GtcsDatabase : public Sqlite3Manager
{
private:
    /* data */
    int InitialGtcsDatbase();
    int InitialMemoryDB();
public:
    GtcsDatabase(/* args */){};
    ~GtcsDatabase(){};
};
#endif
