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
#include "TcpSocket.h"

// Initial CrcDecoder function.
void TcpSocket::Initial(){
}
// Tcp Socket server.
int TcpSocket::TcpSocketServer(char *IP,int Port){
    // Initial parameter.
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char revbuff[4096];
    char sendbuff[4096];
    int  n;
    // 
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    // 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    servaddr.sin_port = htons(Port);
    
    // Check and release bind.
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&servaddr,sizeof(servaddr))<0)
    {
        printf("Unbind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    // Binf socket server ip.
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    // Listen.
    if( listen(listenfd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    printf("======waiting for client‘s request======\n");
    // Loop.
    while(1){
        // 
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, revbuff, MAXLINE, 0);
        revbuff[n] = '\0';
        printf("recv msg from client: %s\n", revbuff);
        // 
        for(int index=0;index<4096;index++)
        {
            sendbuff[index] = revbuff[index];
        }
        printf("Send data = %s",sendbuff);
        if (send(connfd,sendbuff,sizeof(sendbuff),0)<0)
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 0;
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}
