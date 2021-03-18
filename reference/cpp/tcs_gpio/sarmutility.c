/*
==========================================================================================================================
 Program Nane  	: sarmdb.c     
 Subject 		: SARM Serial Port Communication Driver Process                                  
---------------------------------------------------------------------------------------
 Compiler/IDE  	: gcc 4.6.3                                                             
 Library       	:                                                                       
 Commands      	: gcc -Wall -o "sarm" "sarm.c" "sarmdb.c"  -l sqlite3 `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 Programmer    	: DYW                                                                   
 Date	       	: 2019/04/17                                                          
===========================================================================================================================
*/

#define _SARMUTILITY_

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sqlite3.h>



#include "sarmutility.h"
//#include "sarmtag2iom.h"


/* Returns number of pid matching process name, or -1 on error */  
int lookup_pid(char *name, pid_t *pids, int n)  
{  
    FILE *fp;  
    char buf[256], cmd[200], s[3][32];  
    pid_t *pid;  

    if ((fp = popen(PS_COMMAND, "r")) == NULL)  
        return -1;  

    pid = pids;  
    while (n > 0 && fgets(buf, sizeof(buf), fp))  
    {
        if (SCAN(buf) && strstr(cmd, name)) /* improve on this simple name check */  
        {
            pid++;
            n--;  
        }           
	}
	
    pclose(fp);  
    return pid - pids;  
}  


void sig_handler(int signo)
{
	int 		L_Sqlite_Error;
	static  int L_Errcnt = 0;


    printf("\n==> Catch the signal SIGINT %d", signo);
    printf("\n==> If process didn't exit, Press Ctrl+Z again !!");

    printf("\nTry to close port and database ...");
	close(G_CommPort); /* Close the serial port */
	printf("\nPort was closed !!\n");

         
	
RETRY_CLOSE_DB:

	
    if (++L_Errcnt < 10) 
    {
		printf("%d", L_Errcnt);
	}
	else
	{
		printf("\nCloae Database timeout !! \n");
		exit(0);
    }
    
    sqlite3_busy_timeout(G_MemoryDb, 30*1000); //最長等待30m ,也相當於延時
    L_Sqlite_Error = sqlite3_close(G_MemoryDb);
    
    switch(L_Sqlite_Error)
    {
    case SQLITE_OK : 
		printf("\nDatabase was closed !!\n");

		//system("sudo rm -r /tmp/ramdisk/sarm_memory.db");
		//if( !remove("/tmp/ramdisk/sarm_memory.db") )
		//	perror("remove");	

		exit(1);
	case SQLITE_BUSY :
		printf("\nDatabase is busy , waiting for busy ...");
		sqlite3_busy_timeout(G_MemoryDb, 30*1000); //最長等待30m ,也相當於延時
		//usleep(1000);
		goto RETRY_CLOSE_DB;
	case SQLITE_LOCKED:
		printf("\nDatabase is locked , waiting for locked ...");
		sqlite3_busy_timeout(G_MemoryDb, 30*1000); //最長等待30m ,也相當於延時
		//usleep(1000);
		goto RETRY_CLOSE_DB;
	default :
		printf("\nClose Database error !! [%d]", L_Sqlite_Error);

		//system("sudo rm -r /tmp/ramdisk/sarm_memory.db");
		//if( !remove("/tmp/ramdisk/sarm_memory.db") )
		//	perror("remove");	

		//system("sudo killall -9 ntcs");

		exit (0);
	}	
}	


unsigned char GetIO(unsigned char P_IO_No)
{
	FILE * 			stream;
	int 			i;
	unsigned char 	ch;

	switch(P_IO_No)
	{
	case 1:
		if (( stream = fopen("/dev/IN1/value", "r"))== NULL )		
			perror( "Error opening IN1" );
		break;
	case 2:
		if (( stream = fopen("/dev/IN2/value", "r"))== NULL )		
			perror( "Error opening IN2" );
		break;
	case 3:
		if (( stream = fopen("/dev/IN3/value", "r"))== NULL )		
			perror( "Error opening IN3" );
		break;
	case 4:
		if (( stream = fopen("/dev/IN4/value", "r"))== NULL )		
			perror( "Error opening IN4" );
		break;
	case 5:
		if (( stream = fopen("/dev/IN5/value", "r"))== NULL )		
			perror( "Error opening IN5" );
		break;
	case 6:
		if (( stream = fopen("/dev/IN6/value", "r"))== NULL )		
			perror( "Error opening IN6" );
		break;
	case 7:
		if (( stream = fopen("/dev/IN7/value", "r"))== NULL )		
			perror( "Error opening IN7" );
		break;
	case 8:
		if (( stream = fopen("/dev/IN8/value", "r"))== NULL )		
			perror( "Error opening IN8" );
		break;
	case 9:
		if (( stream = fopen("/dev/IN9/value", "r"))== NULL )		
			perror( "Error opening IN9" );
		break;
	case 10:
		if (( stream = fopen("/dev/IN10/value", "r"))== NULL )		
			perror( "Error opening IN10" );
		break;
	case 11:
		if (( stream = fopen("/dev/IN11/value", "r"))== NULL )		
			perror( "Error opening IN11" );
		break;
	case 12:
		if (( stream = fopen("/dev/IN12/value", "r"))== NULL )		
			perror( "Error opening IN12" );
		break;
	default :
			perror( "Error opening port" );
			return -1;
	}

	ch = fgetc(stream);
	fclose( stream );

	if (ch == '0')
	{
		return 1;
	}		
	else if (ch == '1')
	{
		return 0;
	}
	else
	{
		return -1;
	}
	
}

void SetIO(unsigned char P_IO_No, unsigned char P_OnOff)
{	
	FILE * 	stream;
	int 	i;
	int 	ch;

	switch(P_IO_No)
	{
	case 1:
		if (( stream = fopen("/dev/OUT1/value", "w"))== NULL )		
			perror( "Error opening OUT1" );
		break;
	case 2:
		if (( stream = fopen("/dev/OUT2/value", "w"))== NULL )		perror( "Error opening OUT2" );
		break;
	case 3:
		if (( stream = fopen("/dev/OUT3/value", "w"))== NULL )		perror( "Error opening OUT3" );
		break;
	case 4:
		if (( stream = fopen("/dev/OUT4/value", "w"))== NULL )		perror( "Error opening OUT4" );
		break;
	case 5:
		if (( stream = fopen("/dev/OUT5/value", "w"))== NULL )		perror( "Error opening OUT5" );
		break;
	case 6:
		if (( stream = fopen("/dev/OUT6/value", "w"))== NULL )		perror( "Error opening OUT6" );
		break;
	case 7:
		if (( stream = fopen("/dev/OUT7/value", "w"))== NULL )		perror( "Error opening OUT7" );
		break;
	case 8:
		if (( stream = fopen("/dev/OUT8/value", "w"))== NULL )		perror( "Error opening OUT8" );
		break;
	case 9:
		if (( stream = fopen("/dev/OUT9/value", "w"))== NULL )		perror( "Error opening OUT9" );
		break;
	case 10:
		if (( stream = fopen("/dev/OUT10/value", "w"))== NULL )		perror( "Error opening OUT10" );
		break;
	case 11:
		if (( stream = fopen("/dev/OUT11/value", "w"))== NULL )		perror( "Error opening OUT11" );
		break;
	case 12:
		if (( stream = fopen("/dev/OUT12/value", "w"))== NULL )		perror( "Error opening OUT12" );
		break;
	case 13:
		if (( stream = fopen("/dev/OUT13/value", "w"))== NULL )		perror( "Error opening OUT13" );
		break;
	case 14:
		if (( stream = fopen("/dev/OUT14/value", "w"))== NULL )		perror( "Error opening OUT14" );
		break;
	default:
		break;
	}
	
	if (P_OnOff)
		ch = fputc((int) 49, stream);
	else
		ch = fputc((int) 48, stream);

	fclose( stream );
		
	if (ch == EOF)
	{
		printf("\nWrite value file error ==> EOF");
	}
	else
	{
		//printf("\nWrite [%c][%d][%x] to value file OK", ch, ch, ch);
	}
}


unsigned long GetTimeOffset(struct timeval *P_Timeval_Start_Ptr)
{
    struct timeval 	L_Timeval_End;
	unsigned long 	L_Time_Offset;
	
	// Check time offset 	
	gettimeofday(&L_Timeval_End, NULL);
	//printf("\n[sarm.c][640] L_Timeval_End.tv_usec = [%ld]", L_Timeval_End.tv_usec);
	//printf("\n[sarm.c][641] L_Timeval_End.tv_sec = [%ld]", L_Timeval_End.tv_sec);
	L_Time_Offset = 1000000 * (L_Timeval_End.tv_sec - P_Timeval_Start_Ptr->tv_sec) + (L_Timeval_End.tv_usec - P_Timeval_Start_Ptr->tv_usec);
	//printf("\n[sarmutility.c][283] Start@[%ld:%ld] End@[%ld:%ld] Time offset : [%ld] us \n", P_Timeval_Start_Ptr->tv_sec, P_Timeval_Start_Ptr->tv_usec, L_Timeval_End.tv_sec, L_Timeval_End.tv_usec, L_Time_Offset);
	return L_Time_Offset; 
}			
