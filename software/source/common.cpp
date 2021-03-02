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
#include "common.h"

/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitArray::To8BiteArray(int num)
 *
 *  @brief   Convert int to 8 bits array. 
 *
 *  @param   int num
 *
 *  @return  std::array<bool,8>
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitArray::To16BiteArray(int num)
 *
 *  @brief   Convert int to 16 bites array.
 *
 *  @param   int num
 *
 *  @return  std::array<bool,16>
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitArray::To32BiteArray(int num)
 *
 *  @brief   Convert int to 32 bites array.
 *
 *  @param   int num
 *
 *  @return  std::array<bool,32>
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitConverter::ToUInt8(std::array<bool,8> array)
 *
 *  @brief   Convert 8 bites array to uint8_t.
 *
 *  @param   std::array<bool,8> array
 *
 *  @return  uint8_t
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitConverter::ToUInt16(std::array<bool,16> array)
 *
 *  @brief   Convert 16 bites array to uint16_t.
 *
 *  @param   std::array<bool,16> array
 *
 *  @return  uint16_t
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      BitConverter::ToUInt32(std::array<bool,32> array)
 *
 *  @brief   Convert 32 bites array to uint32_t.
 * 
 *  @param   std::array<bool,32> array
 *
 *  @return  uint32_t
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      DataSorter::GetFloatScaleSortString(float src,int scalenum)
 *
 *  @brief   Get scale sort string.
 *
 *  @param   float src
 *  @param   int scalenum
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string DataSorter::GetFloatScaleSortString(float src,int scalenum)
{
    char strbuff[20];
    std::fill_n(strbuff,sizeof(strbuff),0);   
    std::string cmd = "%."+ std::to_string(scalenum) + "f";
    // std::cout << "GetFloatScaleSortString  = " << src << ",cmd = " << cmd <<std::endl;
    snprintf(strbuff,sizeof(strbuff),cmd.c_str(),src);    
    return strbuff;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/01
 *
 *  @fn      GtcsManager::GetCurrentSystemDateYear()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none.
 *
 *  @return  std::string
 *
 *  @note    none.
 *
 *******************************************************************************************/
std::string DateTime::GetCurrentSystemDateYear()
{
    time_t tt;
    time( &tt );
    tt = tt + 8*3600;  // transform the time zone
    tm* t= gmtime( &tt );
    std::string year = std::to_string(t->tm_year + 1900);
    #if defined(_DEBUG_MODE_)
    std::cout << year <<std::endl;
    #endif
    return year;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/01
 *
 *  @fn      GtcsManager::SetCurrentSystemDateYear(std::string &year)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string &year
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool DateTime::SetCurrentSystemDateYear(std::string &year)
{
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/26
 *
 *  @fn      DateTime::GetCurrentSystemDateTime()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none.
 *
 *  @return  std::string
 *
 *  @note    none.
 *
 *******************************************************************************************/
std::string DateTime::GetCurrentSystemDateTime()
{
    time_t tt;
    time( &tt );
    tt = tt + 8*3600;  // transform the time zone
    tm* t= gmtime( &tt );

    std::string datetime = std::to_string(t->tm_year + 1900)+
            std::to_string(t->tm_mon + 1)+
            std::to_string(t->tm_mday)+
            " "+
            std::to_string(t->tm_hour)+
            ":"+
            std::to_string(t->tm_min)+
            ":"+
            std::to_string(t->tm_sec);
            
    #if defined(_DEBUG_MODE_)
    std::cout << datetime <<std::endl;
    #endif

    return datetime;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/26
 *
 *  @fn      GtcsManager::SetCurrentSystemDateTime(std::string &datetime)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string &datetime
 *
 *  @return  bool
 *
 *  @note    none.
 *
 *******************************************************************************************/
bool DateTime::SetCurrentSystemDateTime(std::string &datetime)
{
    return true;
}