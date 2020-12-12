#ifndef _GTCS_MAIN_
#define _GTCS_MAIN_
#define OS_WINDOWS
// #define OS_LINUX
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
#include <iostream>
#include <thread>
#include <string>
#include <array>
#include <vector>
#include <ctime>
#include <typeinfo>
// #include <pthread.h>

#include "../include/comport.h"
#include "../include/crcchecker.h"
#include "../include/database.h"
#include "../include/tcpsocket.h"
#include "../include/gtcstypedefine.h"
#include "../include/gtcsamsprotocol.h"
#include "../include/gtcsmcbtelegram.h"

#if defined(OS_WINDOWS)    
    #include <windows.h>
#endif
#if defined(OS_LINUX)
#endif

#endif
