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
#include "../include/GtcsMain.h"

// main.
int main()
{
    // Initial GtcsManager object.s;
    GtcsManager manager;
    // GtcsTcpSocket tcpserver;

    #pragma region  step 1
    // Initial GTCS system.
    manager.SetMcbPortName("/dev/ttymxc3");
    manager.SetEmmcDatabasePath("/var/www/html/database/tcs.db");
    manager.SetRamdiskDatabasePath("/mnt/ramdisk/tcs.db");
    
    #ifdef _DEBUG_MODE_
    manager.SetGtcsTcpSocketServerIP("192.168.0.207"); // OTTO : 207,ERIC : 202
    #else
    manager.SetGtcsTcpSocketServerIP("127.0.0.1");
    #endif    
    manager.SetGtcsTcpSocketServerPort(9000);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Thread sleep 1s.
    
    manager.InitialGtcsSystem();
    
    // Check GTCS System.
    manager.CheckGtcsSystem();

    // Ste 3 = Set tcpsocket thread and start.
    std::thread thread_tcpserver = std::thread(GtcsTcpSocket::GtcsTcpSocketServerHandler);
    #pragma endregion

    #pragma region step 2
    // loop.
    while (true)
    {
        switch(manager.GetMainFSM())
        {
            case MAIN_FSM::READY:
                std::cout << "CheckMainFSM = READY" << std::endl;
                manager.RunGtcsSystem();
                break;
            case MAIN_FSM::ALARM:
                std::cout << "CheckMainFSM = ALARM" << std::endl;
                manager.ClearGtcsSystemAlarm();
                break;
            case MAIN_FSM::SETTING:
                std::cout << "CheckMainFSM = SETTING" << std::endl;
                manager.SettingGtcsSystem();
                break;
        }
    }
    #pragma endregion

    // Join thread.
    thread_tcpserver.join();
    return 0;
}