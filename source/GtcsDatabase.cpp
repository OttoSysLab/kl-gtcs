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

// Initial 
int  Sqlite3Manager::test()
{
	int result = -1;
	sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
	else
	{
      fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
	return result;
}
// Initail function.
// int GtcsDatabase::InitialGtcsDatbase()
// {
// 	int result = -1;
// 	return reslut;
// }
// // 
// int GtcsDatabase::InitialMemoryDB()
// {
// 	int result = -1;
// 	FILE* 	L_fp;
// 	int 	L_sqlResult;
// 	char 	*L_sqlErrMsg = 0;
// 	char 	L_sqlCmdPtr[] = "CREATE TABLE IF NOT EXISTS IOM(" \
// 		    "ID 			INT PRIMARY KEY NOT NULL," \
// 		    "TYPE			CHAR(1) NOT NULL,"\
// 		    "UPDATE_TIME	DATETIME," \
//     		"STATUS    		CHAR(1)," \
// 	    	"VALUE    		REAL," \
// 		    "DATA       	CHAR(64))";;
	
	
// 	L_fp = fopen(P_Name, "r");
// 	if(L_fp)
// 	{
// 		fclose(L_fp);		
// 		// Open Database 
// 		L_sqlResult = sqlite3_open(P_Name, &G_MemoryDb);
// 		if(L_sqlResult)
// 		{
// 			return 0;
// 		}	
// 		else
// 		{
// 			cout << endl << "Open Database OK" << endl;
// 			sqlite3_enable_shared_cache(1);
// 		}
// 		return (1);	
// 	}	

// 	// Create  Database 
// 	L_sqlResult = sqlite3_open(P_Name, &G_MemoryDb);
// 	  //雖然此database不存在，執行sqlite3_open()時，database不存在，會自動建立
// 	if(L_sqlResult)
// 	{
// 		return 0;
// 	}
// 	else
// 	{
// 		std::cout << " Create database successfully" << srd::endl;
// 		sqlite3_enable_shared_cache(1);
		
//     	// Create Table : IOM 
//     	L_sqlResult = sqlite3_exec(G_MemoryDb, L_sqlCmdPtr, G_sqlCallback, 0, &L_sqlErrMsg);
//     	if(L_sqlResult != SQLITE_OK)
// 	    {
// 	    	sqlite3_free(L_sqlErrMsg);
// 			cout << endl << " L_sqlResult = " << L_sqlResult << endl;
// 			return 0;
//     	} 
// 	    else 
// 	    {
// 			// Check and Initial Memory table
// 			if(InitialMemnryTable())
// 			{
// 				return 1;
// 			}
// 			else
// 			{
// 				cout << endl << " Initial table Fail" << endl;
// 				return 0;
// 			}
// 	    }
// 	}
// 	return result;
// }