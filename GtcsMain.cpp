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

// TCP socket.
void TcpSocketServerhandler()
{    
    GtcsBulletin *bulletin = GtcsBulletin::GetInstance();
    GtcsManager manager;
    #ifdef _DEBUG_MODE_
        char sockip[] = "192.168.0.207";
    #else
        char sockip[] = "127.0.0.1";
    #endif

    int sockport= 9000;
    int MAXLINE = 512;                                         // Buffer size.
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
    printf("====== waiting for client‘s request ======\n");
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

        #ifdef _DEBUG_MODE_
        std::cout << revbuff << std::endl;
        #endif

        // Decoder CMD dn check request.
        bulletin->uisockrevcmd = manager.CheckUiCmdRequest(revbuff);
        if (bulletin->uisockrevcmd=="REQ300")
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
            // std::cout << bulletin->uisockrevcmd << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Send data to tcpclient.
        std::fill_n(sendbuff,sizeof(sendbuff),0);   
        strcpy(sendbuff,manager.GetUiCmdResponse(bulletin->uisockrevcmd).c_str());
        // strcpy(sendbuff,bulletin->uisockrevcmd.c_str());
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
    // Initial GtcsManager object.s;
    GtcsManager manager;

    #pragma region  step 1
    // Initial GTCS system.
    manager.SetMcbPortName("/dev/ttymxc3");
    manager.SetEmmcDatabasePath("/var/www/html/database/tcs.db");
    manager.SetRamdiskDatabasePath("/mnt/ramdisk/tcs.db");
    manager.InitialGtcsSystem();
    
    // Check GTCS System.
    manager.CheckGtcsSystem();

    // Ste 3 = Set tcpsocket thread and start.
    std::thread thread_tcpsocket_server = std::thread(TcpSocketServerhandler);
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
    thread_tcpsocket_server.join();
    return 0;
}