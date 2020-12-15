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
//
TelegramStruct::TelegramStruct()
{}
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
        std::cout<<*ptr<<std::endl;
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
// Get MCB bulletin.
std::vector<std::string> GtcsMcbProtocol::GetMcbBulletin(MCBMAID mainid)
{
    std::vector<std::string> result;    
    return result;
}
// Set MCB Mulletin.
int GtcsMcbProtocol::SetMcbBulletin(MCBMAID mainid)
{
    int result = 0;
    return result;
}

// #pragma region test
// // Set send data.
// std::array<uint8_t,48> GetTelegramData()
// {
//     std::array<uint8_t,48> mcb_ctrl_telegram;
//     #pragma region  
//     mcb_ctrl_telegram[0] = 0x01;
//     mcb_ctrl_telegram[1] = 0x00;
//     mcb_ctrl_telegram[2] = 0x00;
//     mcb_ctrl_telegram[3] = 0x00;
//     mcb_ctrl_telegram[4] = 0x00;
//     mcb_ctrl_telegram[5] = 0x00;
//     mcb_ctrl_telegram[6] = 0x00;
//     mcb_ctrl_telegram[7] = 0x00;
//     mcb_ctrl_telegram[8] = 0x01;
//     mcb_ctrl_telegram[9] = 0x24;
//     mcb_ctrl_telegram[10] = 0x00;
//     mcb_ctrl_telegram[11] = 0x00;
//     mcb_ctrl_telegram[12] = 0xa0;
//     mcb_ctrl_telegram[13] = 0x0f;
//     mcb_ctrl_telegram[14] = 0x01;
//     mcb_ctrl_telegram[15] = 0x00;
//     mcb_ctrl_telegram[16] = 0xe8;
//     mcb_ctrl_telegram[17] = 0x03;
//     mcb_ctrl_telegram[18] = 0xe8;
//     mcb_ctrl_telegram[19] = 0x03;
//     mcb_ctrl_telegram[20] = 0x74;
//     mcb_ctrl_telegram[21] = 0x01;
//     mcb_ctrl_telegram[22] = 0xe8;
//     mcb_ctrl_telegram[23] = 0x03;
//     mcb_ctrl_telegram[24] = 0x00;
//     mcb_ctrl_telegram[25] = 0x00;
//     mcb_ctrl_telegram[26] = 0x00;
//     mcb_ctrl_telegram[27] = 0x00;
//     mcb_ctrl_telegram[28] = 0x08;
//     mcb_ctrl_telegram[29] = 0x07;
//     mcb_ctrl_telegram[30] = 0x00;
//     mcb_ctrl_telegram[31] = 0x00;
//     mcb_ctrl_telegram[32] = 0x00;
//     mcb_ctrl_telegram[33] = 0x00;
//     mcb_ctrl_telegram[34] = 0x00;
//     mcb_ctrl_telegram[35] = 0x00;
//     mcb_ctrl_telegram[36] = 0x00;
//     mcb_ctrl_telegram[37] = 0x00;
//     mcb_ctrl_telegram[38] = 0x00;
//     mcb_ctrl_telegram[39] = 0x00;
//     mcb_ctrl_telegram[40] = 0x00;
//     mcb_ctrl_telegram[41] = 0x00;
//     mcb_ctrl_telegram[42] = 0x00;
//     mcb_ctrl_telegram[43] = 0x00;
//     mcb_ctrl_telegram[44] = 0x46;   // DEV = 70
//     mcb_ctrl_telegram[45] = 0xcf;   // DEV = 206
//     mcb_ctrl_telegram[46] = 0xba;   // DEV = 186
//     mcb_ctrl_telegram[47] = 0xbe;   // DEV = 190
//     #pragma endregion

//     return mcb_ctrl_telegram;
// }
// // Get array.
// std::array<uint8_t,36> GetTelegramArray()
// {
//     std::array<uint8_t,36> array;    
//     for(int index;index<36;index++)
//     {
//         array[index] = (uint8_t)index;
//     }
//     return array;
// }
// // Gtcs Telegram TDD.
// int main()
// {
//     // Test Singleton.
//     GtcsMcbProtocol* mcb = GtcsMcbProtocol::GetInstance();
//     GtcsBulletin* bulletin = GtcsBulletin::GetInstance();
//     std::cout << bulletin->McbBulletin.MID1.u8DmsCorel << std::endl;
//     mcb->telegram.ctrl.EncodeHeaderArray();
//     mcb->telegram.ctrl.EncodeTelegramArray(&mcb->telegram.ctrl.fasten
//                                             ,mcb->telegram.ctrl.struct_length);
//     std::array<uint8_t,36> array = GetTelegramArray();
//     mcb->telegram.status.DecodeTelegramArray(array);
//     for(int index = 0;index<8;index++){
//         std::cout << std::to_string(mcb->telegram.ctrl.telegram_array[index]) << std::endl;
//     }
//     return 0;
// }
#pragma endregion
