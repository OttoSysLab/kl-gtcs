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
#include "crcchecker.h"

// Initial CrcDecoder function.
void CrcChecker::Initial(){
}
// Check CRC32 .
uint32_t CrcChecker::u32CRC32(uint32_t u32CRC, uint8_t *pData){
    uint32_t CRC32 = u32CRC;
    int i ;
    for ( i = 0; i < 4; i++)
    {
	    CRC32 = (CRC32 >> 8) ^ m_pu32CRC32[(CRC32 ^ pData[i]) & 0xff];
    }
    return CRC32;
};
// 
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
// 
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
// Check pData CRC23 is current.
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
// CRC32 Checker.
void CrcChecker::calculateCrc32(uint8_t* pData){
    std::array<uint8_t,4> check_buf = u32CRC32_function(pData,11);
    for(int idx=0;idx<4;idx++){
        crc32_value[idx]=check_buf[idx];
    }
};
