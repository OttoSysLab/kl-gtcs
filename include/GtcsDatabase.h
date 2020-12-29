#ifndef _GTCS_DATABASE_
#define _GTCS_DATABASE_
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
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sqlite3.h>
#include <iostream>
#include "GtcsGloabDefine.h"
#include "GtcsBulletin.h"

#pragma region Sqlite3Manager 
// Gtcs database struct.
class Sqlite3Manager
{
private:
    std::string db_Path = "";
public:
    Sqlite3Manager();
    ~Sqlite3Manager();
    int SetDatabasePath(std::string path);
    std::string GetDatabasePath();
    int UpdateDatabase(std::string table,std::string *ptr);
    int ReadDatabase(std::string table,std::string *ptr);  // int SetDatabaseFilePath(std::string path);
};
#pragma endregion

#pragma region GtcsDatabase
class GtcsDatabase
{
private:
    // Attribute.
    std::list<std::string> data;
    Sqlite3Manager db_ramdisk;
    Sqlite3Manager db_emmc;
public:
    // Constructor.
    GtcsDatabase(std::string ramdisk_Path,std::string emmc_Path);
    ~GtcsDatabase(); 
    std::string GetRamdiskDbPath();
    std::string GetEmmcDbPath();
    int CheckDatabaseFSM(int db_fsm);
};
#endif
