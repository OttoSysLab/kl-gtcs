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

// Write data to sqlite.
bool Sqlite3Manager::UpdateDatabase(std::string db_Path,std::string table,std::string sqlcmd)
{
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
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
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finialize process.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
// Write data to sqlite.
bool Sqlite3Manager::ReadDatabase(std::string db_Path, std::string table,std::string *ptr)
{
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sql  = "";
    // Open database.
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
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
        return false;
    }
    // Excute step.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<" Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    int index = 0;
    *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));

    // Get data from sqlite pointer.
    while(true)
    {
        ptr = (std::string *)(void *)(ptr+1);
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
            }
        }
        else
        {
            break;
        }
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
#pragma endregion

#pragma region GtcsDatabase
// Constructor
GtcsDatabase::GtcsDatabase(std::string Path)
{
    dbPath = Path;
}
// Distructor.
GtcsDatabase::~GtcsDatabase()
{}
std::string GtcsDatabase::GetDatabasePath()
{
    return dbPath;
}
#pragma endregion
