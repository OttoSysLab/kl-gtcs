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
// #define MCB_RW

// Display telegram status.
void displaymonitor(){
    GtcsMcbCommunication* mcb = GtcsMcbCommunication::GetInstance();
    std::cout <<"==============================================="<<std::endl;
        std::cout <<"u16Statusflags = "<<std::to_string(mcb->telegram.status.mcb_status.u16Statusflags)<< std::endl;
        std::cout <<"u32ActError    = "<<std::to_string(mcb->telegram.status.mcb_status.u32ActError)<< std::endl; 
        std::cout <<"u16ActProcNr   = "<<std::to_string(mcb->telegram.status.mcb_status.u16ActProcNr)<< std::endl; 
        std::cout <<"u16ActStepNr   = "<<std::to_string(mcb->telegram.status.mcb_status.u16ActStepNr)<< std::endl; 
        std::cout <<"u16ActCurr     = "<<std::to_string(mcb->telegram.status.mcb_status.u16ActCurr)<< std::endl; 
        std::cout <<"u16ActTorque   = "<<std::to_string(mcb->telegram.status.mcb_status.u16ActTorque)<< std::endl; 
        std::cout <<"u16ActRPM      = "<<std::to_string(mcb->telegram.status.mcb_status.u16ActRPM)<< std::endl; 
        std::cout <<"u16MaxCurrent  = "<<std::to_string(mcb->telegram.status.mcb_status.u16MaxCurrent)<< std::endl; 
        std::cout <<"u16MaxTorque   = "<<std::to_string(mcb->telegram.status.mcb_status.u16MaxTorque)<< std::endl; 
        std::cout <<"u32Angle       = "<<std::to_string(mcb->telegram.status.mcb_status.u32Angle)<< std::endl; 
        std::cout <<"u16TMDFlags    = "<<std::to_string(mcb->telegram.status.mcb_status.u16TMDFlags)<< std::endl;    
        std::cout <<"s16Debug       = "<<std::to_string(mcb->telegram.status.mcb_status.s16Debug)<< std::endl;    
        std::cout <<"s32Debug       = "<<std::to_string(mcb->telegram.status.mcb_status.s32Debug)<< std::endl;    
}
// TCP socket.
void tcpsocket()
{
    // Socket.
    TcpSocket mytcpserver;
    GtcsAmsProtocol* ams = GtcsAmsProtocol::GetInstance();
    char sockip[] = "127.0.0.1";
    int sockport= 9000;    
    int MAXLINE = 4096;                   // Buffer size. 
    // Initial parameter.
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char revbuff[4096];
    char sendbuff[4096];
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
// main.
int main()
{
    // Initial object.
    GtcsMcbCommunication* mcb = GtcsMcbCommunication::GetInstance();
    GtcsAmsProtocol* ams = GtcsAmsProtocol::GetInstance();
    GtcsBulletinManager manager;
    ComPort comm;
    char com_name[] = "/dev/ttymxc3";
    int com_num = comm.InitialComm(com_name); 
    // Set tcpsocket thread and start.
    std::thread thread_tcpsocket = std::thread(tcpsocket);
    // loop.
    while (true)
    {
        switch(manager.GetMainFSM())
        {
            case MAIN_FSM::READY:
                mcb->telegram.ctrl.EncodeTelegramArray(&mcb->telegram.ctrl.fasten,
                                            mcb->telegram.ctrl.struct_length);
                for(int index=0;index<48;index++){
                    comm.SendChar(com_num,mcb->telegram.ctrl.telegram_array[index]);
                }   
                // Thread sleep 15(ms).
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                // Read data form mcb.
                comm.ReadData(com_num,mcb->telegram.status.telegram_array);
                mcb->telegram.status.DecodeTelegramArray();
                // displaymonitor();    
                manager.ConvertActuralData300(&mcb->telegram.status.mcb_status); 
                break;
            case MAIN_FSM::ALARM:
                break;
            case MAIN_FSM::SETTING:
                break;
            // Start System.
            case MAIN_FSM::INITIAL:
                break;
            case MAIN_FSM::STATRT:
                break;
        }
    }
    // Jion thread.
    thread_tcpsocket.join();    
    return 0;
}