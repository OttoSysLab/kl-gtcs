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

// Gtcs database struct.
class Sqlite3Manager
{
private:
    std::string dbPath = "";
public:
    Sqlite3Manager();
    ~Sqlite3Manager();
    
    // Basic function.
    bool UpdateDatabase(std::string db_path,std::string table,std::string sqlcmd);
    // int SetDatabaseFilePath(std::string path);
    bool ReadDatabase(std::string db_path, std::string table,std::string *ptr);
    // Get database table & assigne default value.
    std::string GetDatabaseTableDefaultValue(std::string &tablename,
                                            std::vector<std::string> &coulunmnames, 
                                            std::map<std::string,std::string> &coulunmtypes);    
    // Get Database Insert Sql Command. 
    std::string GetDatabaseInsertSqlCommand(std::string &tablename,
                                            std::vector<std::string> &coulunmnames,
                                            std::map<std::string,std::string> &coulunmtypes,
                                            std::map<std::string,std::string> &coulunmtdatas); 
};

// GtcsTcsDatabase
class GtcsTcsDatabase : public Sqlite3Manager
{
private:
    std::string dbPath = "";
public:
    // Constructor.
    GtcsTcsDatabase(std::string Path);
    ~GtcsTcsDatabase();
       
    // Table = Basic parameter.  
    bool ReadDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct);     
    bool UpdateDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct);    
    
    // Table = jobsequence.
    bool ReadDatabaseJobData(GtcsTcsDatabaseJobInfo &dbstruct,int jobid);

    // Table = program.     
    bool ReadDataBaseSequenceList(std::vector<GtcsTcsDatabaseSequenceInfo> &dblist,int jobid);  
    
    // Table = step.
    bool ReadDatabaseStepList(std::vector<GtcsTcsDatabaseStepInfo> &dblist,int jobid,int seqid);

    // Table = inputstep_job.
    bool ReadDatabaseGPIOInputList(std::vector<GtcsTcsDatabaseStepInfo> &dblist,int jobid,int seqid);

    // Table = outputstep_job.
    bool ReadDatabaseGPIOOutputList(std::vector<GtcsTcsDatabaseStepInfo> &dblist,int jobid,int seqid);

};

// GctsScrewStatusData
class GtcsScrewStatusDatabase : public Sqlite3Manager
{
private:
    std::string dbPath = "";
public:
    // Constructor.
    GtcsScrewStatusDatabase(std::string Path);
    ~GtcsScrewStatusDatabase();

    // Create database.
    bool CreatScrewStatusDatabaseTable();
    bool InsertScrewStatusToDatabaseTable(GtcsScrewStatusbaseInfo &screwstatus);
};