#ifndef GTCS_TLG_DECODER
#define GTCS_TLG_DECODER
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
#include <iostream>
#include <thread>
#include <string>
#include <array>
#include <vector>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close(), Unix Port System Call

#pragma region CrcChecker
/* */
class CrcChecker{
private:
    /* data */
    uint32_t m_pu32CRC32[256] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};
    uint8_t m_pu8Revers[256] = {
        //0     1     2     3     4     5     6     7    8      9     A     B     C     D     E     F
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, // 0
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, // 1
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, // 2
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, // 3
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, // 4
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, // 5
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, // 6
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, // 7
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, // 8
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, // 9
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, // A
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, // B
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, // C
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, // D
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, // E
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF, // F
    };
    uint32_t u32CRC32(uint32_t u32CRC, uint8_t *pData);
    uint32_t u32Bitrevers(uint32_t dwValue); 
    std::array<uint8_t,4> u32CRC32_function(uint8_t* pData, uint32_t u32DWORDCount);
public:
    CrcChecker();
    ~CrcChecker();
    int checkCrc32(uint8_t* pData);
    uint8_t crc32_value[4];
    // Calculate CRC32 checksum.
    void calculateCrc32(uint8_t* pData);
};
CrcChecker::CrcChecker(){}
CrcChecker::~CrcChecker(){}
uint32_t CrcChecker::u32CRC32(uint32_t u32CRC, uint8_t *pData){
    uint32_t CRC32 = u32CRC;
    int i ;
    for ( i = 0; i < 4; i++)
    {
	    CRC32 = (CRC32 >> 8) ^ m_pu32CRC32[(CRC32 ^ pData[i]) & 0xff];
    }
    return CRC32;
};
uint32_t CrcChecker::u32Bitrevers(uint32_t dwValue){
    // uint8_t* pu8;
  	uint8_t pu8[4] ;
    // pu8 = new byte[4];
  	uint32_t dwResult;
    uint32_t u32Buffer;

    pu8[0] = dwValue >> 24 ;
    pu8[1] = dwValue >> 16 ;
    pu8[2] = dwValue >> 8 ;
    pu8[3] = dwValue ;

    dwResult = 0;
    dwResult += m_pu8Revers[pu8[3]];
    u32Buffer = 0;
    u32Buffer = m_pu8Revers[pu8[2]];
    u32Buffer <<= 8;
    dwResult += u32Buffer;
    u32Buffer = 0;
    u32Buffer = m_pu8Revers[pu8[1]];
    u32Buffer <<= 16;
    dwResult += u32Buffer;

    u32Buffer = 0;
    u32Buffer = m_pu8Revers[pu8[0]];
    u32Buffer <<= 24;
    dwResult += u32Buffer;
    return dwResult;
}
std::array<uint8_t,4> CrcChecker::u32CRC32_function(uint8_t* pData, uint32_t u32DWORDCount){
	uint32_t u32CRC = 0xFFFFFFFF;
    uint32_t u32Data;
    int i ;
    uint8_t pu8[4];
    for ( i = 0; i < u32DWORDCount; i++){
        int n = i * 4;
	    u32Data = pData[n];
	    u32Data += (uint32_t)pData[n + 1] * 0x100;
	    u32Data += (uint32_t)pData[n + 2] * 0x10000;
	    u32Data += (uint32_t)pData[n + 3] * 0x1000000;
	    u32Data = u32Bitrevers(u32Data);

	    pu8[0] = u32Data >> 24 ;
	    pu8[1] = u32Data >> 16 ;
	    pu8[2] = u32Data >> 8 ;
	    pu8[3] = u32Data ;
	    u32CRC = u32CRC32(u32CRC, pu8);
    }
    uint32_t u32_buf = u32Bitrevers(u32CRC);
    std::array<uint8_t,4> reslut;
    reslut[0] = u32_buf>>24;
    reslut[1] = u32_buf>>16;
    reslut[2] = u32_buf>>8;
    reslut[3] = u32_buf;
    return reslut;
}
int CrcChecker::checkCrc32(uint8_t* pData){
    int check_ok = 0;
    std::array<uint8_t,4> check_buf = u32CRC32_function(pData,11);
    if(pData[44]==check_buf[0]){
        if (pData[45]==check_buf[1]){
            if (pData[46]==check_buf[2]){
                if(pData[47]==check_buf[3]){
                    check_ok = 1;
                }
            }
        }
    }
    return check_ok;
};
void CrcChecker::calculateCrc32(uint8_t* pData){
    std::array<uint8_t,4> check_buf = u32CRC32_function(pData,11);
    for(int idx=0;idx<4;idx++){
        crc32_value[idx]=check_buf[idx];
    }
};
#pragma endregion

#pragma region TlgDecoder
class TlgDecoder{
private:
    uint8_t arr_tlg[48];
    uint8_t  arr_tlgh[8];
    uint8_t arr_tlgp[36];
    uint8_t  arr_tlgc[4];
public:
    TlgDecoder();
    ~TlgDecoder();
    CrcChecker Crc32Checker;
};
TlgDecoder::TlgDecoder(){}
TlgDecoder::~TlgDecoder(){}
#pragma endregion

#pragma region Serial Port.
class SerialPort{
protected:
private:
public:
    int OpenCommPort = 0;
    unsigned char readBuffor[48];
    // Constructor.
    SerialPort();
    // Distructor.
    ~SerialPort();
    // Initial Comm port.
    int InitialCommPort(char *P_port_name);
    // Send Char.
    int SendChar(int P_CommPort, unsigned char P_Value);
    // ReadData
    int ReadData(int openCommPort);
};
SerialPort::SerialPort(){};
SerialPort::~SerialPort(){};
int SerialPort::InitialCommPort(char *P_port_name){
    int 			L_fd;
    struct termios	L_SerialPortSettings;	/* Create the structure                          */
    /*------------------------------- Opening the Serial Port -------------------------------*/
    L_fd = open(P_port_name , O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY );
	    						/* O_RDWR   - Read/Write access to serial port       */
								/* O_NOCTTY - No terminal will control the process   */
								/* O_NONBLOCK - Open in non blocking mode,read will wait              */
   	if(L_fd)											/* Error Checking */
    {
	    //printf("\n  [%s] Opened Successfully ", P_port_name);
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
int SerialPort::SendChar(int P_CommPort, unsigned char P_Value){
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
int SerialPort::ReadData(int openCommPort){
    // static unsigned char readBuffor[48];
    int BytesRead = 0;
    BytesRead = read(openCommPort, &readBuffor, 48);
    return BytesRead;
};
#pragma endregion

#pragma region
extern "C"{
    TlgDecoder tlgDecoder;
    SerialPort serialPort;

    int checkCrc32(uint8_t* pData){
        return tlgDecoder.Crc32Checker.checkCrc32(pData);
    }
    void calculateCrc32(uint8_t* pData){
        tlgDecoder.Crc32Checker.calculateCrc32(pData);
    }
    uint8_t* getCrc32Value_all(){
        return tlgDecoder.Crc32Checker.crc32_value;
    }
    
    uint8_t* getMCBData(){
        return serialPort.readBuffor;
    }
}
#pragma endregion

#pragma region test aplication.
// Assign value to MCBCtrlTelegram array.
uint8_t MCBCtrlTelegram[48];
void setMCBCtrlTelegramValue(){
    MCBCtrlTelegram[0] = 0x01;
    MCBCtrlTelegram[1] = 0x00;
    MCBCtrlTelegram[2] = 0x00;
    MCBCtrlTelegram[3] = 0x00;
    MCBCtrlTelegram[4] = 0x00;
    MCBCtrlTelegram[5] = 0x00;
    MCBCtrlTelegram[6] = 0x00;
    MCBCtrlTelegram[7] = 0x00;
    MCBCtrlTelegram[8] = 0x01;
    MCBCtrlTelegram[9] = 0x24;
    MCBCtrlTelegram[10] = 0x00;
    MCBCtrlTelegram[11] = 0x00;
    MCBCtrlTelegram[12] = 0xa0;
    MCBCtrlTelegram[13] = 0x0f;
    MCBCtrlTelegram[14] = 0x01;
    MCBCtrlTelegram[15] = 0x00;
    MCBCtrlTelegram[16] = 0xe8;
    MCBCtrlTelegram[17] = 0x03;
    MCBCtrlTelegram[18] = 0xe8;
    MCBCtrlTelegram[19] = 0x03;
    MCBCtrlTelegram[20] = 0x74;
    MCBCtrlTelegram[21] = 0x01;
    MCBCtrlTelegram[22] = 0xe8;
    MCBCtrlTelegram[23] = 0x03;
    MCBCtrlTelegram[24] = 0x00;
    MCBCtrlTelegram[25] = 0x00;
    MCBCtrlTelegram[26] = 0x00;
    MCBCtrlTelegram[27] = 0x00;
    MCBCtrlTelegram[28] = 0x08;
    MCBCtrlTelegram[29] = 0x07;
    MCBCtrlTelegram[30] = 0x00;
    MCBCtrlTelegram[31] = 0x00;
    MCBCtrlTelegram[32] = 0x00;
    MCBCtrlTelegram[33] = 0x00;
    MCBCtrlTelegram[34] = 0x00;
    MCBCtrlTelegram[35] = 0x00;
    MCBCtrlTelegram[36] = 0x00;
    MCBCtrlTelegram[37] = 0x00;
    MCBCtrlTelegram[38] = 0x00;
    MCBCtrlTelegram[39] = 0x00;
    MCBCtrlTelegram[40] = 0x00;
    MCBCtrlTelegram[41] = 0x00;
    MCBCtrlTelegram[42] = 0x00;
    MCBCtrlTelegram[43] = 0x00;
    MCBCtrlTelegram[44] = 0x46;
    MCBCtrlTelegram[45] = 0xcf;
    MCBCtrlTelegram[46] = 0xba;
    MCBCtrlTelegram[47] = 0xbe;
}
// Test Drvier develpoment 01.
void testDriver_01(){
    // Initial tlgDecoder obj.
    TlgDecoder tlgDecoder;
    setMCBCtrlTelegramValue();
    // Calculate CRC32.
    tlgDecoder.Crc32Checker.calculateCrc32(MCBCtrlTelegram);
    uint8_t result[4];
    result[0] = tlgDecoder.Crc32Checker.crc32_value[0];
    result[1] = tlgDecoder.Crc32Checker.crc32_value[1];
    result[2] = tlgDecoder.Crc32Checker.crc32_value[2];
    result[3] = tlgDecoder.Crc32Checker.crc32_value[3];

    for(int index=0;index<sizeof(result);index++){
        std::cout<<"Eocode status flags = "<<std::to_string(result[index])<<std::endl;
    }
}
// Test Drvier develpoment 02.
void testDriver_02(){
    SerialPort serialPort;
    char serialPortName[] = "/dev/ttymxc3";
    setMCBCtrlTelegramValue();
    serialPort.OpenCommPort = serialPort.InitialCommPort(serialPortName);
    std::cout << "Open Comm port number = " << std::to_string(serialPort.OpenCommPort) << std::endl;
    //
    while (true)
    {
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        /* code */
        for(int index=0;index<48;index++){
            int SendStatus = serialPort.SendChar(serialPort.OpenCommPort,MCBCtrlTelegram[index]);
            // std::cout << "Send to MCB array index = "<<std::to_string(index);
            // std::cout <<" value = "<<std::to_string(MCBCtrlTelegram[index])<<std::endl;
        }
        // 
        std::this_thread::sleep_for(std::chrono::milliseconds(15));

        // Read data from MCB.
        int readbytes = serialPort.ReadData(serialPort.OpenCommPort);
        std::cout <<" Read bytes = "<<std::to_string(readbytes)<<std::endl;
        if(serialPort){

        }
        else{ 
            for(int index=0;index<48;index++){
                std::cout << "Serial port read index = "<<std::to_string(index)<<" data = ";
                std::cout << std::to_string(serialPort.readBuffor[index])<<std::endl;
            }
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        std::cout <<"MCB pre-time = "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout <<"(ms)"<<std::endl;
    }
}
// Main function.
int main(){
    // testDriver_01();
    testDriver_02();
    return 0;
}// main
#pragma endregion
#endif // GTCS_TLG_DECODER