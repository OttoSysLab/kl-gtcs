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
#include "gtcstcpsocket.h"

/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      TcpSocket::TcpSocket()
 *
 *  @brief   TcpSocket constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
TcpSocket::TcpSocket()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      TcpSocket::~TcpSocket()
 *
 *  @brief   TcpSocket distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
TcpSocket::~TcpSocket()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcpSocket::GtcsTcpSocket()
 *
 *  @brief   GtcsTcpSocket constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcpSocket::GtcsTcpSocket(/* args */)
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcpSocket::~GtcsTcpSocket()
 *
 *  @brief   GtcsTcpSocket distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcpSocket::~GtcsTcpSocket()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcpSocket::GtcsTcpSocketServerHandler()
 *
 *  @brief   GtcsTcpSocketServerHandler
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsTcpSocket::GtcsTcpSocketServerHandler()
{
    // Initials vlaue & object.
    GtcsManager manager;
    std::string socketrevcmd = "";
    std::string sockip = manager.GetGtcsTcpSocketServerIP();   // Get tcp server ip from bulletin by manager.
    int sockport = manager.GetGtcsTcpSocketServerPort();       // Get tcp port number from bulletin by manager.
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
    servaddr.sin_addr.s_addr = inet_addr(sockip.c_str());
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
        // std::cout << revbuff << std::endl;
        #endif

        // Decoder CMD dn check request.
        if (manager.CheckUiRequestCmd(revbuff)==true)
        {
            socketrevcmd = manager.GetUiRequestCmd();
            if (socketrevcmd=="REQ300")
            {
                manager.SetUiSettingStatus(false);
            }
            else
            {
                manager.SetUiSettingStatus(true);
            }
            // Waiting for app to process CMD.
            while (manager.GetUiSettingStatus()==true)        
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            // Send data to tcpclient.
            std::fill_n(sendbuff,sizeof(sendbuff),0);   
            strcpy(sendbuff,manager.GetUiResponseCmd(socketrevcmd).c_str());
            if (send(connfd,sendbuff,sizeof(sendbuff),0)<0)
            {
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            }    
        }
        close(connfd);
    }
    close(listenfd);
}