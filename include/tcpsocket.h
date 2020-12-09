#ifndef _TCP_SOCKET_
#define _TCP_SOCKET_
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
#include <iostream>       // 
#include <cstring>        // 
#include <cerrno>         // 
#include <unistd.h>       //  
#if defined(OS_LINUX)
#include <arpa/inet.h>    // 
#include <termios.h>      // 
#include <netinet/in.h>   //
#include <sys/types.h>    // 
#include <sys/socket.h>   // 
#include <sys/stat.h>     //
#endif

// Class strcut.
class TcpSocket
{
private:
    /* data */
    int MAXLINE = 4096;                   // Buffer size.
    // char tcpserverIP[] = "127.0.0.1";  // Socket IP & Port.  
    
public:
    TcpSocket(/* args */){};
    ~TcpSocket(){};
    void Initial();
    int TcpSocketServer(char *IP,int Port);
};
#endif