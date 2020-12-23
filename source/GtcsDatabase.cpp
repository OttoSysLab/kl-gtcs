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

// Initial memory database.
int GtcsDatabase::InitialMemoryDB()
{
    int result = -1;
    
    return result;
}
