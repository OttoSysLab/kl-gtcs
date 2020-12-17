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
#include "../include/GtcsMcbCommunication.h"

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
#pragma endregion 
// Constructor.
GtcsMcbCommunication::GtcsMcbCommunication(/* args */)
{}
// Distructor.
GtcsMcbCommunication::~GtcsMcbCommunication()
{}
// Polling to MCB.
int GtcsMcbCommunication::PollingToMcb(GtcsCtrlTelegramStrcut *ptr_ctrl_telegram)
{
    int result = -1;
    // telegram.ctrl.EncodeTelegramArray(&mcb->telegram.ctrl.fasten,
    //                                         mcb->telegram.ctrl.struct_length);
    // for(int index=0;index<48;index++){
    //     comm.SendChar(com_num,mcb->telegram.ctrl.telegram_array[index]);
    // }   
    // // Thread sleep 15(ms).
    // std::this_thread::sleep_for(std::chrono::milliseconds(15));
    // // Read data form mcb.
    // comm.ReadData(com_num,mcb->telegram.status.telegram_array);
    // telegram.status.DecodeTelegramArray();
    // // displaymonitor();    
    // manager.ConvertActuralData300(&mcb->telegram.status.mcb_status); 
    return result;   
}
// SignleTon instance object.
GtcsMcbCommunication* GtcsMcbCommunication::instance = 0;
// Get Instance.
GtcsMcbCommunication* GtcsMcbCommunication::GetInstance()
{
    if(instance == 0)
    {
        instance = new GtcsMcbCommunication();
    }
    return instance;
}
#pragma region RW MCB Paramter.
// Identification Parameter.(MainID = 1)
int GtcsMcbCommunication::ReadIdentificationParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbCommunication::WriteIdentificationParameter()
{
    int result = -1;
    return result;
}
// Basic Parameter.(MainID = 2)
int GtcsMcbCommunication::ReadBasicParameter(McbID2Struct *ptr_basic_para)
{
    int result = -1;
    // First send.
    // Second send.

    // third send. 
    return result;
}    
int GtcsMcbCommunication::WriteBasicParameter()
{
    int result  = -1;
    return result;
}
// Step Parameter. (Main ID = 3)
int GtcsMcbCommunication::ReadStepParametrer()
{
    int result = -1;
    return result;
}
int GtcsMcbCommunication::WriteStepParameter()
{
    int result = -1;
    return result;
}
// Prcoess Parameter.(Main ID = 4)
int GtcsMcbCommunication::ReadProcessParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbCommunication::WriteProcessParameter()
{
    int result = -1;
    return result;
}
int GtcsMcbCommunication::ReadProcessStepList()
{
    int result = -1;
    return result;
}
int GtcsMcbCommunication::WritePrcessStepList()
{
    int result = -1;
    return result;
}
#pragma endregion
#pragma endregion
