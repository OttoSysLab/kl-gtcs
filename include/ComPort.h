#ifndef _GTCS_COMPORT_
#define _GTCS_COMPORT_
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
#include <iostream>    // 
#include <array>

// #if defined(OS_LINUX)
#include <termios.h>   // Contains POSIX terminal control definitions.
#include <unistd.h>    // write(), read(), close(), Unix Port System Call
#include <fcntl.h>     // 
// #endif
class ComPort
{
private:
    /* data */
    // char comport[] = "/dev/ttymxc3";
public:
    ComPort(/* args */){};             // 
    ~ComPort(){};                      // 
    // Initial Comm port.              //
    int InitialComm(char *P_port_name);//
    // Send Char.
    int SendChar(int P_CommPort, unsigned char P_Value);
    // Read Data.
    int ReadData(int openCommPort,std::array<uint8_t,1024> &read_array);
};
#endif