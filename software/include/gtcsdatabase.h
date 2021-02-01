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
#include <cstring>
#include <sqlite3.h>
#include <iostream>
#include "gtcsgloabdefine.h"

#pragma region Sqlite3Manager 
// Gtcs database struct.
class Sqlite3Manager
{
private:
public:
    Sqlite3Manager();
    ~Sqlite3Manager();
    // 
    virtual std::string GetDatabasePath(){};
    virtual void SetDatabasePath(std::string Path){};
    
    // Basic function.
    bool UpdateDatabase(std::string db_path,std::string table,std::string sqlcmd);
    bool ReadDatabase(std::string db_path,std::string table,std::string *ptr);       // int SetDatabaseFilePath(std::string path);
};
#pragma endregion

#pragma region GtcsDatabase
class GtcsDatabase : public Sqlite3Manager
{
private:
    std::string dbPath = "";
public:
    // Constructor.
    GtcsDatabase(std::string Path);
    ~GtcsDatabase();
    
    // Get database path.
    std::string GetDatabasePath();                                   //
    void SetDatabasePath(std::string Path);                          // 
       
    // Table = Basic parameter.  
    bool ReadDatabaseBasicData(GtcsDatabaseBasicInfo &dbstruct);    // 
    bool UpdateDatabaseBasicData(GtcsDatabaseBasicInfo &dbstruct);  //  
    
    // Table = jobsequence.
    bool ReadDatabaseJobData(GtcsDatabaseJobInfo &dbstruct,int jobid);
    bool UpdateDatabaseJobData(GtcsDatabaseJobInfo &dbstruct,int jobid);

    // Table = program.     
    bool ReadDataBaseSequenceList(std::vector<GtcsDatabaseSequenceInfo> &dblist,int jobid);
    // Table = step.
    // bool GetDatabaseStepList(Gtcs);
};
