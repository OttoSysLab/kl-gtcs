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
#include "ComPort.h"

// Initial.
int ComPort::InitialComm(char *P_port_name){
    int 			L_fd;
    struct termios	L_SerialPortSettings;	/* Create the structure                          */
    /*------------------------------- Opening the Serial Port -------------------------------*/
    L_fd = open(P_port_name , O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY );
	    		/* O_RDWR   - Read/Write access to serial port       */
				/* O_NOCTTY - No terminal will control the process   */
				/* O_NONBLOCK - Open in non blocking mode,read will wait              */
   	if(L_fd)    /* Error Checking */
    {
	    printf("[%s] Opened Successfully \n", P_port_name);
	    //system("sudo systemctl stop serial-getty@ttyAMA0.service");
	    /*---------- Setting the Attributes of the serial port using termios structure --------- */
	    tcgetattr(L_fd, &L_SerialPortSettings);	/* Get the current attributes of the Serial port */
	    /* Setting the Baud rate : 115200 */
	    cfsetispeed(&L_SerialPortSettings,B115200); /* Set Read  Speed as 115200                       */
	    cfsetospeed(&L_SerialPortSettings,B115200); /* Set Write Speed as 115200                       */
        cfmakeraw(&L_SerialPortSettings);

        /* 8N1 Mode */
        L_SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
        L_SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
        L_SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
        L_SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
        L_SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
        L_SerialPortSettings.c_cflag |= (CLOCAL | CREAD); /* Enable receiver,Ignore Modem Control lines       */
        L_SerialPortSettings.c_cflag |= (INPCK | ISTRIP);
        L_SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          	/* Disable XON/XOFF flow control both i/p and o/p */
        L_SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  	/* Non Cannonical mode                            */
        L_SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

	    /* Setting Time outs */
	    L_SerialPortSettings.c_cc[VMIN] = 0; /* Read at least 10 characters */
	    L_SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */

	    if((tcsetattr(L_fd, TCSANOW, &L_SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
	    {
	        std::cout << "ERROR ! in Setting attributes" << std::endl;
	        //close(L_fd);
	        return 0;
	    }
	    else
	    {
	        std::cout << "BaudRate = 115200" << std::endl;
            std::cout << "StopBits = 1" << std::endl;
            std::cout << "Parity = none" << std::endl;
	    }
	    /*------------------------------- Flush data from serial port -----------------------------*/
	    tcflush(L_fd, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	    tcflush(L_fd, TCOFLUSH);
	    return L_fd;
   	}
   	else
   	{
        return 0;
    }
};
// 
int ComPort::SendChar(int P_CommPort, unsigned char P_Value){
    char L_WriteBuffer[1];
    int  L_BytesWritten = 0;
    try{
        L_WriteBuffer[0] = P_Value;
        L_BytesWritten = write(P_CommPort, L_WriteBuffer, 1);
        usleep(10);
    }
    catch (const char* message) {
        std::cout << message << std::endl;
    }
    return L_BytesWritten;
};
// Read cpomport data.
int ComPort::ReadData(int openCommPort,std::array<uint8_t,1024> &read_array)
{
    int BytesRead = 0;    
    static uint8_t readbuffer[1024];
    BytesRead = read(openCommPort, &readbuffer, 1024);
    for(int i = 0; i < 48; i++)
    {
        read_array[i] = readbuffer[i];
    }    
    return BytesRead;
};
