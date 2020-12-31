#define _BDEBUG_DB_
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
#include "GtcsDatabase.h"
#include <sqlite3.h>

#pragma region Sqlite3Manager
// Constructor.
Sqlite3Manager::Sqlite3Manager()
{}
// Disturctor.
Sqlite3Manager::~Sqlite3Manager()
{}
// Set database path.
int Sqlite3Manager::SetDatabasePath(std::string path)
{
    int result  = 0;
    db_Path = path;
    return result;
}
// Get database path.
std::string Sqlite3Manager::GetDatabasePath()
{
    return db_Path;
}
// Write data to sqlite.
int Sqlite3Manager::UpdateDatabase(std::string table,std::string *ptr)
{
    int result = 0;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sqlcmd  = "";
    // Open database. 
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return result;
    }
    
    // Send.
    sqlcmd = "set = ";    

    /*
    // Send SQL statement to db.
    sql = "UPDATE " 
          + table 
          + " set = "
          + " where ";
    */
    
    // updata database.
    rc = sqlite3_prepare_v2(db, 
                            sqlcmd.c_str(), 
                            -1, 
                            &stmt, 
                            NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }

    // rc = sqlite3_bind_int(stmt, 1, 1;    // Using parameters ("?") is not
    // if (rc != SQLITE_OK)                      // really necessary, but recommended 
    // {                 
    //     string errmsg(sqlite3_errmsg(db));    // (especially for strings) to avoid
    //     sqlite3_finalize(stmt);               // formatting problems and SQL
    // }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) 
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
    }

    int index = 0; 
    *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
    // std::cout << "Fuck index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
    
    while(true)
    {
        ptr = (std::string *)(void *)(ptr+1);
        // 
        if (*ptr != "\n\r")
        {
            index++;
            if (sqlite3_column_text(stmt, index)==NULL) // If get data == null,break the while loop. 
            {
                break;
            }
            else
            {
                *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
                std::cout << "Fuck index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
            }
        }
        else
        {
            break;
        }
    }
    
    sqlite3_finalize(stmt);
    // Close sqlite3.
    sqlite3_close(db);
    result = 1;
    return result;
}
// Write data to sqlite.
int Sqlite3Manager::ReadDatabase(std::string table,std::string *ptr)
{
    int result = 0;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sql  = "";
    // Open database. 
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return result;
    }
    // Send SQL statement to db.
    sql = "SELECT * from "+ table;
    rc = sqlite3_prepare_v2(db, 
                            sql.c_str(), 
                            -1, 
                            &stmt, 
                            NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }

    // rc = sqlite3_bind_int(stmt, 1, 1;    // Using parameters ("?") is not
    // if (rc != SQLITE_OK)                      // really necessary, but recommended 
    // {                 
    //     string errmsg(sqlite3_errmsg(db));    // (especially for strings) to avoid
    //     sqlite3_finalize(stmt);               // formatting problems and SQL
    // }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) 
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
    }

    int index = 0; 
    *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
    #ifdef _BDEBUG_DB_
    std::cout << "========================Start DB READ============================="<< std::endl;
    std::cout << "DB index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
    #endif

    while(true)
    {
        ptr = (std::string *)(void *)(ptr+1);
        // 
        if (*ptr != "\n\r")
        {
            index++;
            if (sqlite3_column_text(stmt, index)==NULL) // If get data == null,break the while loop. 
            {
                break;
            }
            else
            {
                *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
                #ifdef _BDEBUG_DB_
                std::cout << "DB index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
                #endif
            }
        }
        else
        {
            break;
        }
    }
    
    sqlite3_finalize(stmt);
    // Close sqlite3.
    sqlite3_close(db);
    result = 1;
    return result; 
}
#pragma endregion

#pragma region GtcsDatabase
// Constructor
GtcsDatabase::GtcsDatabase(std::string ramdisk_Path,std::string emmc_Path)
{
    db_ramdisk.SetDatabasePath(ramdisk_Path);
    db_emmc.SetDatabasePath(emmc_Path);
}
// Distructor.
GtcsDatabase::~GtcsDatabase()
{}
// Get ramdisk database file path.
std::string GtcsDatabase::GetRamdiskDbPath()
{
    return db_ramdisk.GetDatabasePath();   
}
// Get emmc database file path.
std::string GtcsDatabase::GetEmmcDbPath()
{
    return db_emmc.GetDatabasePath();   
}
// Check database FSM
int GtcsDatabase::CheckDatabaseFSM(int db_fsm)
{
    int result = 0;
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    switch (db_fsm)
    {
        // Select Gtcs database. 
        case DB_FSM::R_RAM_BAIIC_PARA:
            result = db_ramdisk.ReadDatabase("basic",&bulletin->DbBulletin.basic.mintemp);
            //result = db_ramdisk.GetTableNameList("basic");
            break;
        case DB_FSM::W_RAM_BAIIC_PARA:
            result = db_ramdisk.UpdateDatabase("basic",&bulletin->DbBulletin.basic.mintemp);
            break;
    }
    return result;
}
#pragma endregion
