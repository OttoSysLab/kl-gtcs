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
int Sqlite3Manager::UpdateDatabase(std::string table,std::string sqlcmd)
{
    // int result = (int)SQL_WRITE_STATUS::NONE;
    int result = -1;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database. 
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        // result = (int)SQL_STATUS::SQL_WRITE_NG;
        result = 0;
        return result;
    }

    std::cout << "sqlcmd = " << sqlcmd <<std::endl;
    // updata database.
    rc = sqlite3_prepare_v2(db, 
                            sqlcmd.c_str(), 
                            -1, 
                            &stmt, 
                            NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        // result = (int)SQL_WRITE_STATUS::NG;
        result = 1;
        return result;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) 
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        // result = (int)SQL_WRITE_STATUS::NG;
        result = 1;
        return result;
    }
    
    // Finialize process.    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    // result = (int)SQL_WRITE_STATUS::OK;
    result = 0;
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
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
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
        std::cout<<" Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
    }

    int index = 0; 
    *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
    #ifdef _BDEBUG_DB_
    // std::cout << "========================Start DB READ============================="<< std::endl;
    // std::cout << "DB index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
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
                // std::cout << "DB index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
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
// Read data from Gtcs sqlite database.
int GtcsDatabase::ReadFromSqliteDatabase(Sqlite3Manager db,std::string table,std::string *ptr_start)
{
    int result = db.ReadDatabase(table,ptr_start);
    return result;
}
// Update Sqlite Database.
int GtcsDatabase::UpdateSqliteDatabase(Sqlite3Manager db,std::string table, std::string sqlcmd)
{
    int result = db.UpdateDatabase(table,sqlcmd); 
    return result;
}
#pragma endregion
