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
#include "../include/GtcsMcbProtocol.h"

#pragma region telegram strcut define.
// Telegram Construct.
TelegramStruct::TelegramStruct()
{}
// Telegram Distruct
TelegramStruct::~TelegramStruct()
{}
// Initail telegram array.
uint8_t TelegramStruct::InitialTelegramArray()
{
    uint8_t result = -1;
    int length = sizeof(telegram_array);
    for(int index = 0;index<length;index++)
    {
        telegram_array[index] = 0;
    }
    return result;
}
// Encode Header array.
uint8_t TelegramStruct::EncodeHeaderArray()
{
    uint8_t result = -1;
    // Header* ptr_header = &header;
    uint8_t* ptr = &(&header)->type_num;
    int array_length = 8;
    ptr = (uint8_t*)(void**)(ptr);
    telegram_array[0] = *ptr;
    for(int index = 1;index<array_length;index++)
    {
        ptr = (uint8_t*)(void**)(ptr+1);
        // std::cout<<*ptr<<std::endl;
        telegram_array[index] = *ptr;
    }
    return result;
} 
// Encode Ctrl teelgram payload array.
int CtrlTelegram::EncodeTelegramArray(GtcsCtrlTelegramStrcut *ptr_ctrl_telegram,int length)
{
    int result = -1;
    CrcChecker crccheck;
    // Initial telegram array.
    InitialTelegramArray();
    // Encode header.
    EncodeHeaderArray();
    // Encode pyaload.
    uint16_t* ptr = (uint16_t*)(void**)ptr_ctrl_telegram;
    telegram_array[payload_start_index] = (uint8_t)((*ptr) & 0xff);
    telegram_array[payload_start_index+1] = (uint8_t)((*ptr>>8) & 0xff);
    for(int index = 1;index<length;index++)
    {
        ptr = (uint16_t *)(void **)(ptr+1);
        telegram_array[payload_start_index+index*2] = (uint8_t)((*ptr) & 0xff);
        telegram_array[payload_start_index+index*2+1] = (uint8_t)((*ptr>>8) & 0xff);
    }
    // CRC checker
    crccheck.calculateCrc32((uint8_t*)&telegram_array);
    telegram_array[44] = crccheck.crc32_value[0];
    telegram_array[45] = crccheck.crc32_value[1];
    telegram_array[46] = crccheck.crc32_value[2];
    telegram_array[47] = crccheck.crc32_value[3];
    return result;
}
// GTCS 
int StatusTelegram::DecodeTelegramArray()
{   
    int result = -1;
    mcb_status.u16Statusflags = ((uint16_t)telegram_array[payload_start_index])+
                            ((uint16_t)telegram_array[payload_start_index+1]<<8);
    mcb_status.u32ActError    = ((uint32_t)telegram_array[payload_start_index+2])+
                            ((uint32_t)telegram_array[payload_start_index+3]<<8)+
                            ((uint32_t)telegram_array[payload_start_index+4]<<16)+
                            ((uint32_t)telegram_array[payload_start_index+5]<<24);
    mcb_status.u16ActProcNr   = ((uint16_t)telegram_array[payload_start_index+6])+
                            ((uint16_t)telegram_array[payload_start_index+7]<<8);
    mcb_status.u16ActStepNr   = ((uint16_t)telegram_array[payload_start_index+8])+
                            ((uint16_t)telegram_array[payload_start_index+9]<<8);
    mcb_status.u16ActCurr     = ((uint16_t)telegram_array[payload_start_index+10])+
                            ((uint16_t)telegram_array[payload_start_index+11]<<8);
    mcb_status.u16ActTorque   = ((uint16_t)telegram_array[payload_start_index+12])+
                            ((uint16_t)telegram_array[payload_start_index+13]<<8);
    mcb_status.u16ActRPM      = ((uint16_t)telegram_array[payload_start_index+14])+
                            ((uint16_t)telegram_array[payload_start_index+15]<<8);
    mcb_status.u16MaxCurrent  = ((uint16_t)telegram_array[payload_start_index+16])+
                            ((uint16_t)telegram_array[payload_start_index+17]<<8);
    mcb_status.u16MaxTorque   = ((uint16_t)telegram_array[payload_start_index+18])+
                            ((uint16_t)telegram_array[payload_start_index+19]<<8);
    mcb_status.u32Angle       = ((uint32_t)telegram_array[payload_start_index+20])+
                            ((uint32_t)telegram_array[payload_start_index+21]<<8)+
                            ((uint32_t)telegram_array[payload_start_index+22]<<16)+
                            ((uint32_t)telegram_array[payload_start_index+23]<<24);     
    mcb_status.u32Revolutions = ((uint32_t)telegram_array[payload_start_index+24])+
                            ((uint32_t)telegram_array[payload_start_index+25]<<8)+
                            ((uint32_t)telegram_array[payload_start_index+26]<<16)+
                            ((uint32_t)telegram_array[payload_start_index+27]<<24);
    mcb_status.u16TMDFlags    = ((uint16_t)telegram_array[payload_start_index+28])+
                            ((uint16_t)telegram_array[payload_start_index+29]<<8);
    mcb_status.s16Debug       = ((uint16_t)telegram_array[payload_start_index+30])+
                            ((uint16_t)telegram_array[payload_start_index+31]<<8);
    mcb_status.s32Debug       = ((uint32_t)telegram_array[payload_start_index+32])+
                            ((uint32_t)telegram_array[payload_start_index+33]<<8)+
                            ((uint32_t)telegram_array[payload_start_index+34]<<16)+
                            ((uint32_t)telegram_array[payload_start_index+35]<<24);
    return result;
}
//
// Encode write request teldgram array.
std::array<uint8_t,36> WriteRequestTelegram::EncodeTelegramArray(GtcsRW16TelegramStruct *ptr_request)
{
    std::array<uint8_t,36> result;
    return result;
}
#pragma endregion 
// Constructor.
GtcsMcbProtocol::GtcsMcbProtocol(/* args */)
{}
// Distructor.
GtcsMcbProtocol::~GtcsMcbProtocol()
{}
// SignleTon instance object.
GtcsMcbProtocol* GtcsMcbProtocol::instance = 0;
// Get Instance.
GtcsMcbProtocol* GtcsMcbProtocol::GetInstance()
{
    if(instance == 0)
    {
        instance = new GtcsMcbProtocol();
    }
    return instance;
}
#pragma region RW MCB Paramter.
// Identification Parameter.(MainID = 1)
int GtcsMcbProtocol::ReadIdentificationParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbProtocol::WriteIdentificationParameter()
{
    int result = -1;
    return result;
}
// Basic Parameter.(MainID = 2)
int GtcsMcbProtocol::ReadBasicParameter()
{
    int result = -1;
    
    return result;
}    
int GtcsMcbProtocol::WriteBasicParameter()
{
    int result  = -1;
    return result;
}
// Step Parameter. (Main ID = 3)
int GtcsMcbProtocol::ReadStepParametrer()
{
    int result = -1;
    return result;
}
int GtcsMcbProtocol::WriteStepParameter()
{
    int result = -1;
    return result;
}
// Prcoess Parameter.(Main ID = 4)
int GtcsMcbProtocol::ReadProcessParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbProtocol::WriteProcessParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbProtocol::ReadProcessStepList()
{
    int result = -1;
    return result;
}
int GtcsMcbProtocol::WritePrcessStepList()
{
    int result = -1;
    return result;
}
#pragma endregion
#pragma endregion
