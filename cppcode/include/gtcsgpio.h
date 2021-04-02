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
#include "gtcsgloabdefine.h"
#include "manager.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

#pragma region 
//GPIO Define.
class GPIOHandler
{
private:
    /* data */
public:
    GPIOHandler(/* args */);             // 
    ~GPIOHandler();                      // 
};

// Gtcs GPIO object. 
class GtcsGPIOHandler : public GPIOHandler
{
private:
    // status value.
    uint16_t inputstatusvalue = 0;
    uint16_t outputstatusvalue = 0;
    // Iuput & output status.
    static uint16_t GetGPIOInputStatus( volatile uint32_t *gpio4,volatile uint32_t *gpio5);
    static bool SetGPIOOutputStatus(uint16_t &outputvalue,volatile uint32_t *gpio2,volatile uint32_t *gpio3);
public:
    GtcsGPIOHandler(/* args */);
    ~GtcsGPIOHandler();
    // Thread.
    static void GtcsGPIOProcessHandler();
};
#pragma endregion