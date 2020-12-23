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

// Write data to sqlite.
int GtcsDatabase::ReadDatabase(std::string dbPath,std::string table,std::string *ptr)
{
    int result = -1;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sql  = "";
    // Open database. 
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return result;
    }
    // Send SQL statement to db.
    sql = "SELECT * from "+table;
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
    std::cout << "Fuck index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
    
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
int GtcsDatabase::WriteDatabase(std::string dbPath,std::string table,std::string *ptr)
{
    int result = -1;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sql  = "";
    // Open database. 
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return result;
    }
    // Send SQL statement to db.
    sql = "SELECT * from "+table;

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
    std::cout << "Fuck index = "<<std::to_string(index) << " data = " <<*ptr<< std::endl;
    
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