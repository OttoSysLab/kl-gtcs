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

#define MCB_CS
#define GTCS_TEST_INITIAL
// #define MCB_RW

// TCP socket.
void tcpsocket()
{
    // Socket.
    TcpSocket mytcpserver;
    GtcsAmsProtocol *ams = GtcsAmsProtocol::GetInstance();
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    // char sockip[] = "127.0.0.1";
    char sockip[] = "192.168.0.207";
    int sockport= 9000;    
    int MAXLINE = 256;                                         // Buffer size. 
    // Initial parameter.
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char revbuff[MAXLINE];
    char sendbuff[MAXLINE];
    int  n;
    // Call listen.
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
    }
    // Clear buff.
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(sockip);
    servaddr.sin_port = htons(sockport);
    // Check and release bind.
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&servaddr,sizeof(servaddr))<0)
    {
        printf("Unbind socket error: %s(errno: %d)\n",strerror(errno),errno);
    }
    // Binf socket server ip.
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
    }
    // Listen.
    if( listen(listenfd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    }
    printf("======waiting for client‘s request======\n");
    // Loop.
    while(true){
        // Accept connected.
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);
            continue;
        }
        // Received data from tcpclinet.
        n = recv(connfd, revbuff, MAXLINE, 0);
        revbuff[n] = '\0';     
        bulletin->sockrevcmd  = revbuff;
        // std::cout << bulletin->sockrevcmd << std::endl;
        
        // Decoder CMD dn check request.
        if (ams->CheckRequestStatus(bulletin->sockrevcmd))
        {
            bulletin->uisetting = false;
        }
        else
        {
            bulletin->uisetting = true;
        }
        // Waiting for app to process CMD.
        while(bulletin->uisetting)
        {
            std::cout << bulletin->sockrevcmd << std::endl;
        } 
        
        // Send data to tcpclient.   
        strcpy(sendbuff,ams->GetAmsBulletin(ams->GetAmsCmdNum("DATA300")).c_str());
        if (send(connfd,sendbuff,sizeof(sendbuff),0)<0)
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        }
        close(connfd);
    }
    close(listenfd);
}
// main.
int main()
{
    #pragma region Start system. 
    // Step 1 = Initial object.
    GtcsMcbCommunication *mcb = GtcsMcbCommunication::GetInstance();
    GtcsAmsProtocol *ams = GtcsAmsProtocol::GetInstance();
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    GtcsBulletinManager manager;

    // Step 2 = First polling to MBC.
    mcb->InitialMcbComPort("/dev/ttymxc3");
    for(int index=0;index<5;index++)
    {
        mcb->CheckMcbFSM((int)MCB_FSM::POLLING);
        manager.ConvertActuralData300();
    }   
    // Ste 3 = Set tcpsocket thread and start.    
    std::thread thread_tcpsocket = std::thread(tcpsocket);
    #pragma endregion
    
    #pragma region Check System   
    // Step 1 = Read data from mcb basice parameter.
    if (mcb->CheckMcbFSM((int)MCB_FSM::READ_MCB_BASIC)!=0)
    {
        std::uint16_t *ptr = &bulletin->McbBulletin.BasicPara.s16MinTemp;
        std::cout << std::to_string(bulletin->McbBulletin.BasicPara.s16MaxTemp) <<std::endl;
    }   
    // Step 2 = Copy tcs.db to ramdisk.
    
    std::string db_emmc_Path = "/var/www/html/database/tcs.db";   // Initial database path.
    std::string db_ramdisk_Path = "/mnt/ramdisk/tcs.db";          // Initial database path.
    std::string systemcmd = "sudo cp " + db_emmc_Path + " " + db_ramdisk_Path;
    system(systemcmd.c_str());  
    
    // Step 3 = Write basic parameter to tcs.db which is in ramdisk.
    GtcsDatabase database(db_ramdisk_Path,db_emmc_Path);
    database.CheckDatabaseFSM((int)DB_FSM::R_RAM_BAIIC_PARA);    
    std::cout << database.GetRamdiskDbPath() << std::endl;
    std::cout << database.GetEmmcDbPath() << std::endl;
    
    // Step 4 = Compare data bwtweem ramdisk and emmc database basic table.
    
    // Step 5 = Jump to selected MAIN_FSM. 
    #ifdef GTCS_TEST_INITIAL
    #endif
    #pragma endregion
    
    #pragma region 
    // Display some informaiton. 
    std::cout <<"Gear Ratio = "<<std::to_string(bulletin->McbBulletin.BasicPara.u16GearBoxRatio)<<std::endl;
    #pragma endregion

    #pragma region loop
    // loop.
    while (true)
    {
        switch(manager.GetMainFSM())
        {
            case MAIN_FSM::READY: 
                if (bulletin->uisetting==false)
                {
                    mcb->CheckMcbFSM((int)MCB_FSM::POLLING);
                    manager.ConvertActuralData300();
                } 
                else // MAIN_FSM Jump to setting mode. 
                {
                    manager.SetMainFSM(MAIN_FSM::SETTING);
                }                
                break;
            case MAIN_FSM::ALARM:    
                break;
            case MAIN_FSM::SETTING:
                // 
                if (bulletin->uisetting==true)
                {
                    std::cout<< "MAIN_FSM::SETTING bulletin->uisetting = true" << std::endl;
                }
                // mcb->CheckMcbFSM((int)MCB_FSM::WRITE_MCB_BASIC);                                              
                break;
            #pragma region 
            // Start System.
            case MAIN_FSM::INITIAL:
                manager.SetMainFSM(MAIN_FSM::READY);
                break;
            case MAIN_FSM::STATRT:
                manager.SetMainFSM(MAIN_FSM::INITIAL);
                break;
            #pragma endregion
        }
    }
    #pragma endregion
    // Jion thread.
    thread_tcpsocket.join();
    return 0;
}