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
#include "../include/gtcsgpio.h"

#include <iostream>
#include <pthread.h>
#include <cstring>

// main.
int main()
{
    #pragma region step 1
    // Initial GtcsManager object.
    GtcsManager manager;
    // Initial GTCS system.
    manager.SetMcbPortName("/dev/ttymxc3");
    manager.SetEmmcDatabasePath("/var/www/html/database/tcs.db");
    manager.SetRamdiskDatabasePath("/mnt/ramdisk/tcs.db");
    manager.SetRamdiskTxtPath("/mnt/ramdisk/data300.txt");
    
    // Initial System screw status database.
    std::string screwstatuspath =  "/mnt/ramdisk/screwdata_" + manager.GetCurrentDateYear()+ ".db";
    manager.SetScrewStatusDatabase(screwstatuspath);
    manager.CreatScrewStatusDatabase();                       // Create database.
    manager.InitialGtcsSystem();

    // Check GTCS System.
    if (manager.CheckGtcsSystem() == true)
    {
        #if defined(_DEBUG_MODE_)
        std::cout << "Check Gtcs System = OK!! " << std::endl;
        #endif
    }
    #pragma endregion

    // Ste 3 = Set tcpsocket thread and start.
    #pragma region step 2
    std::string socketip = "";
    int port = 9000;
    #if defined(_DEBUG_MODE_38_)
    socketip = "192.168.0.38";      // i.mx8 Leo.
    #elif defined(_DEBUG_MODE_52_)
    socketip = "192.168.0.52";      // i.mx8 GPIO測試機
    #elif defined(_DEBUG_MODE_54_)
    socketip = "192.168.0.54";      // i.mx8 Otto.
    #elif defined(_DEBUG_MODE_107_)
    socketip = "192.168.0.107";     // i.mx8 ERIC.
    // #elif defined(_DEBUG_MODE_111_)
    // socketip = "192.168.0.111";     // i.mx8 惠達機
    #elif defined(_DEBUG_MODE_202_)
    socketip = "192.168.0.202";     // i.mx6.
    #elif defined(_DEBUG_MODE_207_)kls
    socketip = "192.168.0.207";     // i.mx6 Otto.
    #else
    socketip = "127.0.0.1"; // Linux OTTO
    #endif

    manager.StartGtcsTcpSocketServerThread(socketip, port);     // OTTO : 38,
    manager.StartGtcsGPIOThread();                              // GPIO thread
    #pragma endregion

    #pragma region step 3
    // loop.
    while (true)
    {    
        switch (manager.GetMainFSM())
        {
        case MAIN_FSM::READY:
            #if defined(_DEBUG_MODE_)
            std::cout << "Current MainFSM = READY" << std::endl;
            #endif
            manager.RunGtcsSystem();
            break;
        case MAIN_FSM::SETTING:
            #if defined(_DEBUG_MODE_)
            std::cout << "Current MainFSM = SETTING" << std::endl;
            #endif
            manager.SettingGtcsSystem();
            break;
        case MAIN_FSM::ALARM:
            #if defined(_DEBUG_MODE_)
            std::cout << "Current MainFSM = ALARM" << std::endl;
            #endif
            manager.ClearGtcsSystemAlarm();
            break;
        }
        #if defined(_DEBUG_JOB_SEQ_)
        std::string datatime = manager.GetCurrentDateime();
        std::cout << "Current sys time = " << datatime << std::endl;
        break;
        #endif
    }
    #pragma endregion
    // Join thread.
    manager.StopAllThread();
    return 0;
}
