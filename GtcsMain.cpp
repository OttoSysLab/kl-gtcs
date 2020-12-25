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
    GtcsAmsProtocol* ams = GtcsAmsProtocol::GetInstance();
    // char sockip[] = "127.0.0.1";
    char sockip[] = "192.168.0.207";
    int sockport= 9000;    
    int MAXLINE = 256;                   // Buffer size. 
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
        // Received daat from tcpclinet.
        n = recv(connfd, revbuff, MAXLINE, 0);
        revbuff[n] = '\0';     
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
// 
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
        manager.ConvertActuralData300(&mcb->telegram.status.current_status);
    }   
    // Ste 3 = Set tcpsocket thread and start.    
    std::thread thread_tcpsocket = std::thread(tcpsocket);
    #pragma endregion
    
    #pragma region Check System   
    // Step 1 = Read data from mcb basice parameter.
    if (mcb->CheckMcbFSM((int)MCB_FSM::READ_MCB_BASIC)!=0)
    {
        // std::uint16_t *ptr = &bulletin->McbBulletin.BasicPara.s16MinTemp;
        std::cout << std::to_string(bulletin->McbBulletin.BasicPara.s16MaxTemp) <<std::endl;
    }   
    // Step 2 = Copy tcs.db to ramdisk.
    system(" sudo cp /var/www/html/database/tcs.db /mnt/ramdisk/tcs.db");  
    
    // Step 3 = Write basic parameter to tcs.db whoch in ramdisk.
    
    // Step 4 = Compare data bwtweem ramdisk and emmc database basic table.

    #ifdef GTCS_TEST_INITIAL

    #endif
    #pragma endregion

    // Test.
    std::string db_ramdisk_Path = "/mnt/ramdisk/tcs.db";          // Initial database path.
    std::string db_emmc_Path = "/var/www/html/database/tcs.db";          // Initial database path.
    GtcsDatabase database(db_ramdisk_Path,db_emmc_Path);
    database.CheckDatabaseFSM((int)DB_FSM::R_RAM_BAIIC_PARA);    
    std::cout << database.GetRamdiskDbPath() << std::endl;
    std::cout << database.GetEmmcDbPath() << std::endl;

    #pragma region loop
    // loop.
    while (true)
    {
        switch(manager.GetMainFSM())
        {
            case MAIN_FSM::READY: 
                mcb->CheckMcbFSM((int)MCB_FSM::POLLING);
                manager.ConvertActuralData300(&mcb->telegram.status.current_status); 
                break;
            case MAIN_FSM::ALARM:    
                break;
            case MAIN_FSM::SETTING:  
                mcb->CheckMcbFSM((int)MCB_FSM::WRITE_MCB_BASIC);                                              
                break;

            #pragma region 
            // Start System.
            case MAIN_FSM::INITIAL:
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