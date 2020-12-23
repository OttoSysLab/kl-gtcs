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
#include <typeinfo>

// Sqlite callback function.
// static int callback(void *data, int argc, char **argv, char **azColName)
// {
//     int i;
//     fprintf(stderr, "%s: ", (const char*)data);
    
//     for(i=0; i<argc; i++){
//         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
//     }
//     printf("\n");
//     return 0;
// }
// Initial sqlite database.
int GtcsDatabase::ReadDatabaseBasicTable(std::string dbPath)
{
    // Initial return status.
    int result = -1;

    // // Initial database parameter.
	// sqlite3 *db;
    // char *zErrMsg = 0;
    // int rc;
    // char sql[] = "";
    // const char* data = "Callback function called";
    // std::string sql_str = "";

    // // Open database. 
    // rc = sqlite3_open(dbPath.c_str(),&db);
    // if (rc){
    //     std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
    //     return result;
    // }
    // else{
    //     std::cout<<"Opened database successfully"<<std::endl;
    // }
    
    // // Create SQL statement.
    // sql_str = "SELECT * from basic";
    
    // // Excute SQL statement.
    // strcpy(sql,sql_str.c_str()); 
    // rc = sqlite3_exec(db,sql,callback,(void*)data,&zErrMsg);
    // if (rc != SQLITE_OK){
    //     std::cout<<"SQL error:"<<zErrMsg<<std::endl;
    //     sqlite3_free(zErrMsg);
    // }
    // else{
    //     std::cout<<"Operation done successfully!"<<std::endl;
    //     result = 1;
    // }
    // sqlite3_close(db);
    return result;
}
int GtcsDatabase::ReadDatabaseBasicData(std::string dbPath,GtcsDatabaseBasicStruct *ptr_basic)
{
    int result = -1;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database. 
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return result;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db, 
                            "SELECT * from basic", 
                            -1, 
                            &stmt, 
                            NULL);
    if (rc != SQLITE_OK)
        // throw string(sqlite3_errmsg(db));
    {
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
    }

    // rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
    // if (rc != SQLITE_OK) {                 // really necessary, but recommended
    //     string errmsg(sqlite3_errmsg(db)); // (especially for strings) to avoid
    //     sqlite3_finalize(stmt);            // formatting problems and SQL
    //     throw errmsg;                      // injection attacks.
    // }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<"SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        // throw errmsg;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        // throw string("customer not found");
    }
    // for(int i=0;i<35;i++)
    // {
    //     std::cout << sqlite3_column_text(stmt, i) << std::endl;
    // }
    // std::cout << sqlite3_column_text(stmt, 1) << std::endl;
    // std::cout << typeid(sqlite3_column_text(stmt, 1)).name() << std::endl;
    // std::cout << sqlite3_column_text(stmt, 2) << std::endl;
    // std::cout << sqlite3_column_text(stmt, 3) << std::endl;

    // ptr_basic->mintemp = sqlite3_column_text(stmt, 0);
    ptr_basic->maxtemp = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

    // result = "{"+*prt;
    // while(true){
    //     prt = (std::string *)(void *)(prt+1);
    //     if (*prt != "\n\r"){
    //         result += ","+*prt;
    //     }
    //     else{
    //         break;
    //     }
    // }
    // result += "}";
    
    sqlite3_finalize(stmt);
    // Close sqlite3.
    sqlite3_close(db);
    result = 1;
   return result; 
}