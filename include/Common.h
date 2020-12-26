#ifndef _GTCS_COMMON_
#define _GTCS_COMMON_
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
#include <iostream>    // 
#include <array>       // s 
#include <cmath>



// #endif
class Common
{
private:
    /* data */
public:
    Common(/* args */);             // 
    ~Common();                      // 
    // Send Char.
};

class BitArray
{
private:
    /* data */
public:
    BitArray(/* args */){};
    ~BitArray(){};
    static std::array<bool,8> To8BiteArray(int num);
    static std::array<bool,16> To16BiteArray(int num);
    static std::array<bool,32> To32BiteArray(int num);
};
class BitConverter
{
private:
    /* data */
public:
    BitConverter(/* args */){};
    ~BitConverter(){};
    static uint8_t ToUInt8(std::array<bool,8> array);
    static uint16_t ToUInt16(std::array<bool,16> array);
    static uint32_t ToUInt32(std::array<bool,32> array);
};
#endif