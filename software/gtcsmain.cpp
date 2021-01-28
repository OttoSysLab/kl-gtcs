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
#include "../include/manager.h"
#include "../include/gtcstcpsocket.h"

#include <iostream>
#include <pthread.h>
#include <cstring>

// main.
int main()
{
    // Initial GtcsManager object.
    GtcsManager manager;
    // GtcsTcpSocket tcpserver;

    #pragma region  step 1
    // Initial GTCS system.
    manager.SetMcbPortName("/dev/ttymxc3");
    manager.SetEmmcDatabasePath("/var/www/html/database/tcs.db");
    manager.SetRamdiskDatabasePath("/mnt/ramdisk/tcs.db");
    manager.InitialGtcsSystem();
    
    // Check GTCS System.
    manager.CheckGtcsSystem();

    // Ste 3 = Set tcpsocket thread and start.
    #pragma region step 2
    #ifdef _DEBUG_MODE_    
    #if defined(_DEBUG_MODE_202_)
    manager.SetGtcsTcpSocketServerInfo("127.0.0.1",9000); // OTTO : 207,ERIC : 202
    #endif
    #if defined(_DEBUG_MODE_207_)
    manager.SetGtcsTcpSocketServerInfo("192.168.0.207",9000); // OTTO : 207,ERIC : 202
    #endif    
    #else
    manager.SetGtcsTcpSocketServerInfo("127.0.0.1",9000);
    #endif 
    #pragma endregion
    #pragma endregion

    #pragma region step 3
    // loop.
    while (true)
    {
        switch(manager.GetMainFSM())
        {
            case MAIN_FSM::READY:
                #ifdef _DEBUG_MODE_
                std::cout << "CheckMainFSM = READY" << std::endl;
                #endif
                manager.RunGtcsSystem();
                break;
            case MAIN_FSM::ALARM:
                #ifdef _DEBUG_MODE_
                std::cout << "CheckMainFSM = ALARM" << std::endl;
                #endif
                manager.ClearGtcsSystemAlarm();
                break;
            case MAIN_FSM::SETTING:
                #ifdef _DEBUG_MODE_
                std::cout << "CheckMainFSM = SETTING" << std::endl;
                #endif
                manager.SettingGtcsSystem();
                break;
        }
    }
    #pragma endregion
    // Join thread.
    // thread_tcpserver.join();
    // manager.StopAllThread();
    return 0;
}