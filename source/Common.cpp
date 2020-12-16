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
#include "Common.h"

BitArray::BitArray()
{}
BitArray::~BitArray()
{}
std::array<bool,32> BitArray::To32BiteArray(int num)
{
    std::array<bool,32> result;
    int length = sizeof(result);
    for(int index=0;index<length;index++)
    {
        if (num &(1<<index))
        {
            result[index] = true;  
        }
        else
        {
            result[index] = false;
        }
    }
    return result;
}
std::array<bool,16> BitArray::To16BiteArray(int num)
{
    std::array<bool,16> result;
    int length = sizeof(result);
    for(int index=0;index<length;index++)
    {
        if (num &(1<<index))
        {
            result[index] = true;  
        }
        else
        {
            result[index] = false;
        }
    }
    return result;
}
std::array<bool,8> BitArray::To8BiteArray(int num)
{
    std::array<bool,8> result;
    int length = sizeof(result);
    for(int index=0;index<length;index++)
    {
        if (num &(1<<index))
        {
            result[index] = true;  
        }
        else
        {
            result[index] = false;
        }
    }
    return result;
}


