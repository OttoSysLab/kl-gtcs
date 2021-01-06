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

// Convert int to 8 bites array.
std::array<bool,8> BitArray::To8BiteArray(int num)
{
    std::array<bool,8> result;
    int count = sizeof(result);
    for(int i = 0 ; i<count ; i++)
    {
        if (num &(1<<i))
        {
            result[i] = true;  
        }
        else
        {
            result[i] = false;
        }
    }
    return result;
}
// Convert int to 16 bites array.
std::array<bool,16> BitArray::To16BiteArray(int num)
{
    std::array<bool,16> result;
    int count = sizeof(result);
    for(int i = 0 ; i < count ; i++)
    {
        if (num &(1<<i))
        {
            result[i] = true;  
        }
        else
        {
            result[i] = false;
        }
    }
    return result;
}
// Convert int to 32 bites array.
std::array<bool,32> BitArray::To32BiteArray(int num)
{
    std::array<bool,32> result;
    int count = sizeof(result);
    for(int i=0;i<count;i++)
    {
        if (num &(1<<i))
        {
            result[i] = true;  
        }
        else
        {
            result[i] = false;
        }
    }
    return result;
}
// Convert 8 bites array to uint8_t.
uint8_t BitConverter::ToUInt8(std::array<bool,8> array)
{
    uint8_t result  = 0;
    int count  = sizeof(array);
    for (int i = 0; i < count; i++)
    {
        if (array[i] == 1)
        {
            result |= 1;  
        }        
        else
        {
            result |= 0;
        }
        result << 1;        
    }
    return result;
}
// Convert 16 bites array to uint16_t.
uint16_t BitConverter::ToUInt16(std::array<bool,16> array)
{
    uint16_t result  = 0;
    int count  = sizeof(array);
    for (int i = 0; i < count; i++)
    {
        if (array[i] == 1)
        {
            result |= 1;  
        }        
        else
        {
            result |= 0;
        }
        result << 1;        
    }
    return result;
}
// Convert 32 bites array to uint32_t.
uint32_t BitConverter::ToUInt32(std::array<bool,32> array)
{
    uint32_t result = 0;
    int count  = sizeof(array);
    for (int i = 0; i < count; i++)
    {
        if (array[i] == 1)
        {
            result |= (1<<i);  
        }        
        else
        {
            result |= (0<<i);
        }
        result << 1;        
    }
    return result;
}

// Get scale sort string.
std::string DataSorter::GetFloatScaleSortString(float src,int scalenum)
{
    char strbuff[20];
    std::fill_n(strbuff,sizeof(strbuff),0);   
    std::string cmd = "%."+ std::to_string(scalenum) + "f";
    // std::cout << "GetFloatScaleSortString  = " << src << ",cmd = " << cmd <<std::endl;
    snprintf(strbuff,sizeof(strbuff),cmd.c_str(),src);    
    return strbuff;
}