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
#include "../include/gtcsmcbcomm.h"

/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      StatusTelegram::CheckLoosenStatus(uint16_t last_status_flags,uint16_t current_status_flags)
 *
 *  @brief   Check TMS status
 *
 *  @param   uint16_t last_status_flags
 * 
 *  @param   uint16_t current_status_flags
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int StatusTelegram::CheckLoosenStatus(uint16_t last_status_flags,uint16_t current_status_flags)
{
    int result = 0;
    std::array<bool,16> last_TMD_status = BitArray::To16BiteArray(last_status_flags);
    std::array<bool,16> current_TMD_status = BitArray::To16BiteArray(current_status_flags);

    // Falling edge to chang status.
    if ((last_TMD_status[TMD_INPUT::REV_SW] == true)&&(current_TMD_status[TMD_INPUT::REV_SW] == false))
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (loosen_status == false)
        {
            loosen_status = true;
        }
        else
        {
            loosen_status = false;
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
 *  @fn      TelegramStruct::TelegramStruct()
 *
 *  @brief   Telegram Construct.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
TelegramStruct::TelegramStruct()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      TelegramStruct::~TelegramStruct()
 *
 *  @brief   Telegram Distruct.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
TelegramStruct::~TelegramStruct()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      TelegramStruct::InitialTelegramArray()
 *
 *  @brief   Initail telegram array.
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int TelegramStruct::InitialTelegramArray()
{
    int result = -1;
    int length = sizeof(telegram_array);
    for(int index = 0;index<length;index++)
    {
        telegram_array[index] = 0;
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      TelegramStruct::EncodeTelegramArray()
 *
 *  @brief   Encode Ctrl telegram payload array.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
int TelegramStruct::EncodeTelegramArray()
{
    int result = -1;
    CrcChecker crccheck;
    // CRC checker
    crccheck.calculateCrc32((uint8_t*)&telegram_array);
    telegram_array[44] = crccheck.crc32_value[0];
    telegram_array[45] = crccheck.crc32_value[1];
    telegram_array[46] = crccheck.crc32_value[2];
    telegram_array[47] = crccheck.crc32_value[3];
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      TelegramStruct::EncodeHeaderArray()
 *
 *  @brief   Encode Header array.
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int TelegramStruct::EncodeHeaderArray()
{
    int result = -1;
    uint8_t* ptr = &(&header)->type_num;
    int array_length = 8;
    ptr = (uint8_t*)(void**)(ptr);
    telegram_array[0] = *ptr;
    for(int index = 1;index<array_length;index++)
    {
        ptr = (uint8_t*)(void**)(ptr+1);
        telegram_array[index] = *ptr;
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      CtrlTelegram::InitialCtrlFlags(GtcsCtrlTelegramStrcut &telegram)
 *
 *  @brief   Initial Ctrl telegram flags.
 *
 *  @param   GtcsCtrlTelegramStrcut &telegram
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void CtrlTelegram::InitialCtrlFlags(GtcsCtrlTelegramStrcut &telegram)
{
    telegram.u16Ctrlflags = 0;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      CtrlTelegram::SetCtrlFlags(GtcsCtrlTelegramStrcut &telegram,int flagIdx)
 *
 *  @brief   Set Ctrl telegram flags.
 *
 *  @param   GtcsCtrlTelegramStrcut &telegram
 *  
 *  @param   int flagIdx
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void CtrlTelegram::SetCtrlFlags(GtcsCtrlTelegramStrcut &telegram,int flagIdx)
{
    telegram.u16Ctrlflags |= 1<<flagIdx;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      CtrlTelegram::EncodeTelegramArray(GtcsCtrlTelegramStrcut *ptr_ctrl_telegram,int length)
 *
 *  @brief   Encode Ctrl telegram payload array.
 *
 *  @param   GtcsCtrlTelegramStrcut *ptr_ctrl_telegram
 * 
 *  @param   int length
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
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
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      StatusTelegram::DecodeTelegramArray()
 *
 *  @brief   Decode MCB Telegram Array.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
int StatusTelegram::DecodeTelegramArray()
{
    int result = -1;
    current_status.u16Statusflags = ((uint16_t)telegram_array[payload_start_index])+
                                    ((uint16_t)telegram_array[payload_start_index+1]<<8);
    current_status.u32ActError    = ((uint32_t)telegram_array[payload_start_index+2])+
                                    ((uint32_t)telegram_array[payload_start_index+3]<<8)+
                                    ((uint32_t)telegram_array[payload_start_index+4]<<16)+
                                    ((uint32_t)telegram_array[payload_start_index+5]<<24);
    current_status.u16ActProcNr   = ((uint16_t)telegram_array[payload_start_index+6])+
                                    ((uint16_t)telegram_array[payload_start_index+7]<<8);
    current_status.u16ActStepNr   = ((uint16_t)telegram_array[payload_start_index+8])+
                                    ((uint16_t)telegram_array[payload_start_index+9]<<8);
    current_status.u16ActCurr     = ((uint16_t)telegram_array[payload_start_index+10])+
                                    ((uint16_t)telegram_array[payload_start_index+11]<<8);
    current_status.u16ActTorque   = ((uint16_t)telegram_array[payload_start_index+12])+
                                    ((uint16_t)telegram_array[payload_start_index+13]<<8);
    current_status.u16ActRPM      = ((uint16_t)telegram_array[payload_start_index+14])+
                                    ((uint16_t)telegram_array[payload_start_index+15]<<8);
    current_status.u16MaxCurrent  = ((uint16_t)telegram_array[payload_start_index+16])+
                                    ((uint16_t)telegram_array[payload_start_index+17]<<8);
    current_status.u16MaxTorque   = ((uint16_t)telegram_array[payload_start_index+18])+
                                    ((uint16_t)telegram_array[payload_start_index+19]<<8);
    current_status.u32Angle       = ((uint32_t)telegram_array[payload_start_index+20])+
                                    ((uint32_t)telegram_array[payload_start_index+21]<<8)+
                                    ((uint32_t)telegram_array[payload_start_index+22]<<16)+
                                    ((uint32_t)telegram_array[payload_start_index+23]<<24);
    current_status.u32Revolutions = ((uint32_t)telegram_array[payload_start_index+24])+
                                    ((uint32_t)telegram_array[payload_start_index+25]<<8)+
                                    ((uint32_t)telegram_array[payload_start_index+26]<<16)+
                                    ((uint32_t)telegram_array[payload_start_index+27]<<24);
    current_status.u16TMDFlags    = ((uint16_t)telegram_array[payload_start_index+28])+
                                    ((uint16_t)telegram_array[payload_start_index+29]<<8);
    current_status.s16Debug       = ((uint16_t)telegram_array[payload_start_index+30])+
                                    ((uint16_t)telegram_array[payload_start_index+31]<<8);
    current_status.s32Debug       = ((uint32_t)telegram_array[payload_start_index+32])+
                                    ((uint32_t)telegram_array[payload_start_index+33]<<8)+
                                    ((uint32_t)telegram_array[payload_start_index+34]<<16)+
                                    ((uint32_t)telegram_array[payload_start_index+35]<<24);
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::GtcsMcbComm()
 *
 *  @brief   GtcsMcbComm constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsMcbComm::GtcsMcbComm(/* args */)
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::~GtcsMcbComm()
 *
 *  @brief   GtcsMcbComm disstructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsMcbComm::~GtcsMcbComm()
{}
// SignleTon instance object.
GtcsMcbComm* GtcsMcbComm::instance = 0;
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm* GtcsMcbComm::GetInstance()
 *
 *  @brief   Get Instance.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsMcbComm* GtcsMcbComm::GetInstance()
{
    if(instance == 0)
    {
        instance = new GtcsMcbComm();
    }
    return instance;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::ReadIdentificationParameter()
 *
 *  @brief   Identification Parameter.(MainID = 1)
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::ReadIdentificationParameter()
{
    int result = -1;
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::WriteIdentificationParameter()
 *
 *  @brief   Identification Parameter.(MainID = 1)
 *
 *  @param   none
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::WriteIdentificationParameter()
{
    int result = -1;
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::ReadBasicParameter(McbID2Struct &basic_para)
 *
 *  @brief   Basic Parameter.(MainID = 2)
 *
 *  @param   McbID2Struct &basic_para
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::ReadBasicParameter(McbID2Struct &basic_para)
{
    int result = -1;
    // Initial value.
    uint8_t main_id = 2;
    uint8_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;

    // std::this_thread::sleep_for(std::chrono::milliseconds(20));

    #pragma region First package. (SID1-6)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1;
    sub_id[1]  = 2;
    sub_id[2]  = 3;
    sub_id[3]  = 4;
    sub_id[4]  = 5;
    sub_id[5]  = 6;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = main_id;
    telegram.r_request.telegram_array[payload_start_index+1]  = 0;
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.r_request.telegram_array[payload_start_index+7]  = 0;
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = main_id;
    telegram.r_request.telegram_array[payload_start_index+13] = 0;
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = main_id;
    telegram.r_request.telegram_array[payload_start_index+19] = 0;
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = main_id;
    telegram.r_request.telegram_array[payload_start_index+25] = 0;
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.r_request.telegram_array[payload_start_index+30] = main_id;
    telegram.r_request.telegram_array[payload_start_index+31] = 0;
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    basic_para.s16MinTemp          = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 1
    basic_para.s16MaxTemp          = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 2
    basic_para.u16MaxCurrent       = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 3
    basic_para.u16MaxPeakCurrent   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 4
    basic_para.u16TorqueSensorType = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 5
    basic_para.u16MaxDutyCycle     = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                                      ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 4
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    #pragma endregion
    #pragma region second package. (SID7-12)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 7;
    sub_id[1]  = 8;
    sub_id[2]  = 9;
    sub_id[3]  = 10;
    sub_id[4]  = 11;
    sub_id[5]  = 12;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = main_id;
    telegram.r_request.telegram_array[payload_start_index+1]  = 0;
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.r_request.telegram_array[payload_start_index+7]  = 0;
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = main_id;
    telegram.r_request.telegram_array[payload_start_index+13] = 0;
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = main_id;
    telegram.r_request.telegram_array[payload_start_index+19] = 0;
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = main_id;
    telegram.r_request.telegram_array[payload_start_index+25] = 0;
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6.
    telegram.r_request.telegram_array[payload_start_index+30] = main_id;
    telegram.r_request.telegram_array[payload_start_index+31] = 0;
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    basic_para.u16MaxTorque    = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 7
    basic_para.u16PWMFreq      = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 8
    basic_para.u16MaxRPM       = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 9
    basic_para.u16MaxSlope     = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 10
    basic_para.u16MinBusVolt   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 11
    basic_para.u16MaxBusVolt   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 12
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    #pragma endregion
    #pragma region third send. (SID13-18)
     telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 13;
    sub_id[1]  = 14;
    sub_id[2]  = 15;
    sub_id[3]  = 16;
    sub_id[4]  = 17;
    sub_id[5]  = 18;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         // 
    telegram.r_request.header.address_3 = payload_start_index+12;        // 
    telegram.r_request.header.address_4 = payload_start_index+18;        // 
    telegram.r_request.header.address_5 = payload_start_index+24;        // 
    telegram.r_request.header.address_6 = payload_start_index+30;        // 
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = main_id;
    telegram.r_request.telegram_array[payload_start_index+1]  = 0;
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.r_request.telegram_array[payload_start_index+7]  = 0;
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = main_id;
    telegram.r_request.telegram_array[payload_start_index+13] = 0;
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = main_id;
    telegram.r_request.telegram_array[payload_start_index+19] = 0;
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = main_id;
    telegram.r_request.telegram_array[payload_start_index+25] = 0;
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6.
    telegram.r_request.telegram_array[payload_start_index+30] = main_id;
    telegram.r_request.telegram_array[payload_start_index+31] = 0;
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    basic_para.u16StartDutyCycle = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 13
    basic_para.u16GearBoxRatio   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 14
    basic_para.u32StartInp       = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 15
    basic_para.u32RevInp         = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 16
    basic_para.u16RevRpm         = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 17
    basic_para.u16RevSlope       = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                                    ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 18
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));

    #pragma endregion
    #pragma region fourth package. (SID19-24)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 19;
    sub_id[1]  = 20;
    sub_id[2]  = 21;
    sub_id[3]  = 22;
    sub_id[4]  = 23;
    sub_id[5]  = 24;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = main_id;
    telegram.r_request.telegram_array[payload_start_index+1]  = 0;
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.r_request.telegram_array[payload_start_index+7]  = 0;
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = main_id;
    telegram.r_request.telegram_array[payload_start_index+13] = 0;
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = main_id;
    telegram.r_request.telegram_array[payload_start_index+19] = 0;
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = main_id;
    telegram.r_request.telegram_array[payload_start_index+25] = 0;
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6.
    telegram.r_request.telegram_array[payload_start_index+30] = main_id;
    telegram.r_request.telegram_array[payload_start_index+31] = 0;
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    basic_para.u16RevMaxCurrent = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 13
    basic_para.u16RevMaxTorque  = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 14
    basic_para.u16ErrorIdleTime = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 15
    basic_para.u16BackLash      = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 16
    basic_para.u16PGain         = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 17
    basic_para.u16IGain         = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                                   ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 18
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));

    // telegram.rw_response.InitialTelegramArray();
    #pragma endregion
    #pragma region fifth (SID25)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 25;
    sub_id[1]  = 0;
    sub_id[2]  = 0;
    sub_id[3]  = 0;
    sub_id[4]  = 0;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = main_id;
    telegram.r_request.telegram_array[payload_start_index+1]  = 0;
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.r_request.telegram_array[payload_start_index+7]  = 0;
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = main_id;
    telegram.r_request.telegram_array[payload_start_index+13] = 0;
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = main_id;
    telegram.r_request.telegram_array[payload_start_index+19] = 0;
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = main_id;
    telegram.r_request.telegram_array[payload_start_index+25] = 0;
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6.
    telegram.r_request.telegram_array[payload_start_index+30] = main_id;
    telegram.r_request.telegram_array[payload_start_index+31] = 0;
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    basic_para.u16Encoder = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                             ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 25
    // telegram.rw_response.InitialTelegramArray();
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));

    #pragma endregion

    #ifdef _DEBUG_MODE_
    std::cout << "----------------------------------------------------------------" << std::endl;
    // Display package 1.
    std::cout << "basic_para.s16MinTemp = " << std::to_string(basic_para.s16MinTemp) << std::endl;
    std::cout << "basic_para.s16MaxTemp = " << std::to_string(basic_para.s16MaxTemp) << std::endl;
    std::cout << "basic_para.u16MaxCurrent = " << std::to_string(basic_para.u16MaxCurrent) << std::endl;
    std::cout << "basic_para.u16MaxPeakCurrent = " << std::to_string(basic_para.u16MaxPeakCurrent) << std::endl;
    std::cout << "basic_para.u16TorqueSensorType = " << std::to_string(basic_para.u16TorqueSensorType) << std::endl;
    std::cout << "basic_para.u16MaxDutyCycle = " << std::to_string(basic_para.u16MaxDutyCycle) << std::endl;
    // Display package 2.
    std::cout << "basic_para.u16MaxTorque = " << std::to_string(basic_para.u16MaxTorque) << std::endl;
    std::cout << "basic_para.u16PWMFreq = " << std::to_string(basic_para.u16PWMFreq) << std::endl;
    std::cout << "basic_para.u16MaxRPM = " << std::to_string(basic_para.u16MaxRPM) << std::endl;
    std::cout << "basic_para.u16MaxSlope = " << std::to_string(basic_para.u16MaxSlope) << std::endl;
    std::cout << "basic_para.u16MinBusVolt = " << std::to_string(basic_para.u16MinBusVolt) << std::endl;
    std::cout << "basic_para.u16MaxBusVolt = " << std::to_string(basic_para.u16MaxBusVolt) << std::endl;
    // Display package 3.
    std::cout << "basic_para.u16StartDutyCycle = " << std::to_string(basic_para.u16StartDutyCycle) << std::endl;
    std::cout << "basic_para.u16GearBoxRatio = " << std::to_string(basic_para.u16GearBoxRatio) << std::endl;
    std::cout << "basic_para.u32StartInp = " << std::to_string(basic_para.u32StartInp) << std::endl;
    std::cout << "basic_para.u32RevInp = " << std::to_string(basic_para.u32RevInp) << std::endl;
    std::cout << "basic_para.u16RevRpm = " << std::to_string(basic_para.u16RevRpm) << std::endl;
    std::cout << "basic_para.u16RevSlope = " << std::to_string(basic_para.u16RevSlope) << std::endl;
    // Display package 4.
    std::cout << "basic_para.u16RevMaxCurrent = " << std::to_string(basic_para.u16RevMaxCurrent) << std::endl;
    std::cout << "basic_para.u16RevMaxTorque = " << std::to_string(basic_para.u16RevMaxTorque) << std::endl;
    std::cout << "basic_para.u16ErrorIdleTime = " << std::to_string(basic_para.u16ErrorIdleTime) << std::endl;
    std::cout << "basic_para.u16BackLash = " << std::to_string(basic_para.u16BackLash) << std::endl;
    std::cout << "basic_para.u16PGain = " << std::to_string(basic_para.u16PGain) << std::endl;
    std::cout << "basic_para.u16IGain = " << std::to_string(basic_para.u16IGain) << std::endl;
    // Display package 5.
    std::cout << "basic_para.u16Encoder = " << std::to_string(basic_para.u16Encoder) << std::endl;
    #endif
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::WriteBasicParameter(McbID2Struct &basic)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   McbID2Struct &basic
 *
 *  @return  int 
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::WriteBasicParameter(McbID2Struct &basic)
{
    // Initial local paramter.
    int result = -1;
    uint8_t main_id = 2;
    uint8_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;
    #pragma region First package. (SID1-6)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1;
    sub_id[1]  = 2;
    sub_id[2]  = 3;
    sub_id[3]  = 4;
    sub_id[4]  = 5;
    sub_id[5]  = 6;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         // 
    telegram.w_request.header.address_3 = payload_start_index+12;        // 
    telegram.w_request.header.address_4 = payload_start_index+18;        // 
    telegram.w_request.header.address_5 = payload_start_index+24;        // 
    telegram.w_request.header.address_6 = payload_start_index+30;        // 
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = main_id;
    telegram.w_request.telegram_array[payload_start_index+1]  = 0;
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(basic.s16MinTemp%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(basic.s16MinTemp/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.w_request.telegram_array[payload_start_index+7]  = 0;
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(basic.s16MaxTemp%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(basic.s16MaxTemp/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = main_id;
    telegram.w_request.telegram_array[payload_start_index+13] = 0;
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(basic.u16MaxCurrent%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(basic.u16MaxCurrent/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = main_id;
    telegram.w_request.telegram_array[payload_start_index+19] = 0;
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(basic.u16MaxPeakCurrent%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(basic.u16MaxPeakCurrent/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = main_id;
    telegram.w_request.telegram_array[payload_start_index+25] = 0;
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(basic.u16TorqueSensorType%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(basic.u16TorqueSensorType/256);
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = main_id;
    telegram.w_request.telegram_array[payload_start_index+31] = 0;
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(basic.u16MaxDutyCycle%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(basic.u16MaxDutyCycle/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    // comm.ReadData(com_num,telegram.rw_response.telegram_array);
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    #pragma endregion
    #pragma region second package. (SID7-12)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 7;
    sub_id[1]  = 8;
    sub_id[2]  = 9;
    sub_id[3]  = 10;
    sub_id[4]  = 11;
    sub_id[5]  = 12;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = main_id;
    telegram.w_request.telegram_array[payload_start_index+1]  = 0;
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(basic.u16MaxTorque%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(basic.u16MaxTorque/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.w_request.telegram_array[payload_start_index+7]  = 0;
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(basic.u16PWMFreq%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(basic.u16PWMFreq/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = main_id;
    telegram.w_request.telegram_array[payload_start_index+13] = 0;
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(basic.u16MaxRPM%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(basic.u16MaxRPM/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = main_id;
    telegram.w_request.telegram_array[payload_start_index+19] = 0;
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(basic.u16MaxSlope%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(basic.u16MaxSlope/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = main_id;
    telegram.w_request.telegram_array[payload_start_index+25] = 0;
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(basic.u16MinBusVolt%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(basic.u16MinBusVolt/256);
    // Package SID = 6.
    telegram.w_request.telegram_array[payload_start_index+30] = main_id;
    telegram.w_request.telegram_array[payload_start_index+31] = 0;
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(basic.u16MaxBusVolt%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(basic.u16MaxBusVolt/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    // comm.ReadData(com_num,telegram.rw_response.telegram_array);
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region third send. (SID13-18)
     telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 13;
    sub_id[1]  = 14;
    sub_id[2]  = 15;
    sub_id[3]  = 16;
    sub_id[4]  = 17;
    sub_id[5]  = 18;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = main_id;
    telegram.w_request.telegram_array[payload_start_index+1]  = 0;
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(basic.u16StartDutyCycle%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(basic.u16StartDutyCycle/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.w_request.telegram_array[payload_start_index+7]  = 0;
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(basic.u16GearBoxRatio%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(basic.u16GearBoxRatio/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = main_id;
    telegram.w_request.telegram_array[payload_start_index+13] = 0;
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(basic.u32StartInp%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(basic.u32StartInp/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = main_id;
    telegram.w_request.telegram_array[payload_start_index+19] = 0;
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(basic.u32RevInp%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(basic.u32RevInp/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = main_id;
    telegram.w_request.telegram_array[payload_start_index+25] = 0;
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(basic.u16RevRpm%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(basic.u16RevRpm/256);
    // Package SID = 6.
    telegram.w_request.telegram_array[payload_start_index+30] = main_id;
    telegram.w_request.telegram_array[payload_start_index+31] = 0;
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(basic.u16RevSlope%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(basic.u16RevSlope/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region fourth package. (SID19-24)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 19;
    sub_id[1]  = 20;
    sub_id[2]  = 21;
    sub_id[3]  = 22;
    sub_id[4]  = 23;
    sub_id[5]  = 24;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = main_id;
    telegram.w_request.telegram_array[payload_start_index+1]  = 0;
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(basic.u16RevMaxCurrent%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(basic.u16RevMaxCurrent/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.w_request.telegram_array[payload_start_index+7]  = 0;
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(basic.u16RevMaxTorque%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(basic.u16RevMaxTorque/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = main_id;
    telegram.w_request.telegram_array[payload_start_index+13] = 0;
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(basic.u16ErrorIdleTime%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(basic.u16ErrorIdleTime/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = main_id;
    telegram.w_request.telegram_array[payload_start_index+19] = 0;
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(basic.u16BackLash%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(basic.u16BackLash/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = main_id;
    telegram.w_request.telegram_array[payload_start_index+25] = 0;
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(basic.u16PGain%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(basic.u16PGain/256);
    // Package SID = 6.
    telegram.w_request.telegram_array[payload_start_index+30] = main_id;
    telegram.w_request.telegram_array[payload_start_index+31] = 0;
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(basic.u16IGain%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(basic.u16IGain/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    //
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region fifth (SID25)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 25;
    sub_id[1]  = 0;
    sub_id[2]  = 0;
    sub_id[3]  = 0;
    sub_id[4]  = 0;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = main_id;
    telegram.w_request.telegram_array[payload_start_index+1]  = 0;
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(basic.u16Encoder%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(basic.u16Encoder/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = main_id;
    telegram.w_request.telegram_array[payload_start_index+7]  = 0;
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = 0;
    telegram.w_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = main_id;
    telegram.w_request.telegram_array[payload_start_index+13] = 0;
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = 0;
    telegram.w_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = main_id;
    telegram.w_request.telegram_array[payload_start_index+19] = 0;
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = 0;
    telegram.w_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = main_id;
    telegram.w_request.telegram_array[payload_start_index+25] = 0;
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = 0;
    telegram.w_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6.
    telegram.w_request.telegram_array[payload_start_index+30] = main_id;
    telegram.w_request.telegram_array[payload_start_index+31] = 0;
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = 0;
    telegram.w_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // 
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::WriteToMcbFlash(int mainid,int subid,int addr_num)
 *
 *  @brief   Write to mcb flash.
 *
 *  @param   int mainid
 *  
 *  @param   int subid
 * 
 *  @param   int addr_num
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::WriteToMcbFlash(int mainid,int subid,int addr_num)
{
    int result = -1;
    int main_id = mainid;
    uint8_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;
    #pragma region final package. Wriete to flash.
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = (uint8_t)subid;
    sub_id[1]  = 0;
    sub_id[2]  = 0;
    sub_id[3]  = 0;
    sub_id[4]  = 0;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(addr_num%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(addr_num/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = 0;
    telegram.w_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = 0;
    telegram.w_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = 0;
    telegram.w_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = 0;
    telegram.w_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = 0;
    telegram.w_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::ReadStepParametrer(McbID3Struct &step_para,int mainid)
 *
 *  @brief   Step Parameter. (Main ID = 3XXX)
 *
 *  @param   McbID3Struct &step_para
 * 
 *  @param   int mainid
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::ReadStepParametrer(McbID3Struct &step_para,int mainid)
{
    int result = -1;
    // GtcsBulletin *bulltein = GtcsBulletin::GetInstance();
    // McbID3Struct *step_para = &bulltein->McbBulletin.StepPara;
    int main_id = mainid;
    uint8_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;
    #pragma region First package. (SID2-7)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 2;
    sub_id[1]  = 3;
    sub_id[2]  = 4;
    sub_id[3]  = 5;
    sub_id[4]  = 6;
    sub_id[5]  = 7;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    step_para.u16StepRpm          = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 2
    step_para.u16StepSlope        = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 3
    step_para.u16StepMaxCurrent   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 4
    step_para.u16StepMaxTorque    = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 5
    step_para.u16StepMaxRevol     = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 6
    step_para.u16StepTime         = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 7
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region Second package.(SID8-12)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 9;
    sub_id[1]  = 10;
    sub_id[2]  = 11;
    sub_id[3]  = 12;
    sub_id[4]  = 8;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+32;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    step_para.u16StepAngleWindow  = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 9
    step_para.u16StepTorqueWindow = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 10
    step_para.u16MinDutyCycle     = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 11
    step_para.u16StepFlags        = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                                     ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 12
    step_para.s32StepAngle        = (uint32_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                                     ((uint32_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8);
                                     ((uint32_t)telegram.rw_response.telegram_array[payload_start_index+30]<<16);
                                     ((uint32_t)telegram.rw_response.telegram_array[payload_start_index+31]<<24); // SID = 8
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion.
    #pragma region Thrid package. (SID13-15)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 13;
    sub_id[1]  = 14;
    sub_id[2]  = 15;
    sub_id[3]  = 0;
    sub_id[4]  = 0;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.r_request.telegram_array[payload_start_index+3]  = 0;
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.r_request.telegram_array[payload_start_index+9]  = 0;
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.r_request.telegram_array[payload_start_index+15] = 0;
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.r_request.telegram_array[payload_start_index+21] = 0;
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.r_request.telegram_array[payload_start_index+27] = 0;
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.r_request.telegram_array[payload_start_index+33] = 0;
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    step_para.u16WindowMode    = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 13
    step_para.u16AngleWindow2  = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 14
    step_para.u16TorqueWindow2 = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                                  ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 15
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    
    #ifdef _DEBUG_MODE_
    // Display step parameter SID2-7.
    std::cout << "step_para.u16StepRpm = " << std::to_string(step_para.u16StepRpm) << std::endl;
    std::cout << "step_para.u16StepSlope = " << std::to_string(step_para.u16StepSlope) << std::endl;
    std::cout << "step_para.u16StepMaxCurrent = " << std::to_string(step_para.u16StepMaxCurrent) << std::endl;
    std::cout << "step_para.u16StepMaxTorque = " << std::to_string(step_para.u16StepMaxTorque) << std::endl;
    std::cout << "step_para.u16StepMaxRevol = " << std::to_string(step_para.u16StepMaxRevol) << std::endl;
    std::cout << "step_para.u16StepTime = " << std::to_string(step_para.u16StepTime) << std::endl;
    // Display step parameter SID2-7.
    std::cout << "step_para.s32StepAngle = " << std::to_string(step_para.s32StepAngle) << std::endl;
    std::cout << "step_para.u16StepAngleWindow = " << std::to_string(step_para.u16StepAngleWindow) << std::endl;
    std::cout << "step_para.u16StepTorqueWindow = " << std::to_string(step_para.u16StepTorqueWindow) << std::endl;
    std::cout << "step_para.u16MinDutyCycle = " << std::to_string(step_para.u16MinDutyCycle) << std::endl;
    std::cout << "step_para.u16StepFlags = " << std::to_string(step_para.u16StepFlags) << std::endl;
    // Display step parameter SID2-7.
    std::cout << "step_para.u16WindowMode = " << std::to_string(step_para.u16WindowMode) << std::endl;
    std::cout << "step_para.u16AngleWindow2 = " << std::to_string(step_para.u16AngleWindow2) << std::endl;
    std::cout << "step_para.u16TorqueWindow2 = " << std::to_string(step_para.u16TorqueWindow2) << std::endl;
    #endif
    
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::WriteStepParameter(McbID3Struct &step, int mainid)
 *
 *  @brief   Write Step Parameter. (Main ID = 3XXX)
 *
 *  @param   McbID3Struct &step
 * 
 *  @param   int mainid
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::WriteStepParameter(McbID3Struct &step, int mainid)
{
    int result = -1;
    // Get
    int main_id = mainid;
    uint8_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;
    #pragma region First package. (SID2-7)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 2;
    sub_id[1]  = 3;
    sub_id[2]  = 4;
    sub_id[3]  = 5;
    sub_id[4]  = 6;
    sub_id[5]  = 7;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(step.u16StepRpm%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(step.u16StepRpm/256);;
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(step.u16StepSlope%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(step.u16StepSlope/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(step.u16StepMaxCurrent%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(step.u16StepMaxCurrent/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(step.u16StepMaxTorque%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(step.u16StepMaxTorque/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(step.u16StepMaxRevol%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(step.u16StepMaxRevol/256);
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(step.u16StepTime%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(step.u16StepTime/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region Second package. (SID8-12)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 9;
    sub_id[1]  = 10;
    sub_id[2]  = 11;
    sub_id[3]  = 12;
    sub_id[4]  = 8;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+32;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(step.u16StepAngleWindow%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(step.u16StepAngleWindow/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(step.u16StepTorqueWindow%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(step.u16StepTorqueWindow/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(step.u16MinDutyCycle%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(step.u16MinDutyCycle/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(step.u16StepFlags%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(step.u16StepFlags/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(step.s32StepAngle%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(step.s32StepAngle/256);
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(step.s32StepAngle/(256*256));
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(step.s32StepAngle/(256*256*256));
    // ??
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = 0;
    telegram.w_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {        
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion.
    #pragma region thrid package. (SID13-15)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 13;
    sub_id[1]  = 14;
    sub_id[2]  = 15;
    sub_id[3]  = 0;
    sub_id[4]  = 0;
    sub_id[5]  = 0;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = sub_id[0];
    telegram.w_request.telegram_array[payload_start_index+3]  = 0;
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(step.u16WindowMode%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(step.u16WindowMode/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = sub_id[1];
    telegram.w_request.telegram_array[payload_start_index+9]  = 0;
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(step.u16AngleWindow2%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(step.u16AngleWindow2/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = sub_id[2];
    telegram.w_request.telegram_array[payload_start_index+15] = 0;
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(step.u16TorqueWindow2%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(step.u16TorqueWindow2/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = sub_id[3];
    telegram.w_request.telegram_array[payload_start_index+21] = 0;
    telegram.w_request.telegram_array[payload_start_index+22] = 0;
    telegram.w_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = sub_id[4];
    telegram.w_request.telegram_array[payload_start_index+27] = 0;
    telegram.w_request.telegram_array[payload_start_index+28] = 0;
    telegram.w_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = sub_id[5];
    telegram.w_request.telegram_array[payload_start_index+33] = 0;
    telegram.w_request.telegram_array[payload_start_index+34] = 0;
    telegram.w_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region Write to mcb flash.
    WriteToMcbFlash(7,11,main_id);// Write to mcb flach,step number = mainid,
    #pragma endregion
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::ReadProcessParameter(McbID4Struct &process,int processnum)
 *
 *  @brief   Prcoess Parameter.(Main ID = 4XXX)
 *
 *  @param   McbID4Struct &process
 * 
 *  @param   int processnum
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::ReadProcessParameter(McbID4Struct &process,int processnum)
{ 
    // Initial local parameter.
    int result = -1;
    // GtcsBulletin *bulltein = GtcsBulletin::GetInstance();
    // McbID4Struct *process = &bulltein->McbBulletin.ProcessPara;
    int main_id = processnum;
    uint16_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;

    #pragma region First package. (SID1-6)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 2;
    sub_id[1]  = 3;
    sub_id[2]  = 4;
    sub_id[3]  = 5;
    sub_id[4]  = 6;
    sub_id[5]  = 7;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.r_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.r_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.r_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.r_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.r_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = .G.?
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.r_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    process.u16ProcPGain   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 1
    process.u16ProcIGain   = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 2
    process.u16ProcMaxTime = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 3
    process.u16ProcMinTime = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 4
    process.u16ProcRevFunc = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 5
    process.u16NbrSteps    = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                              ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 4
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region Second package 2. (Step list 1-6)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1000;
    sub_id[1]  = 1001;
    sub_id[2]  = 1002;
    sub_id[3]  = 1003;
    sub_id[4]  = 1004;
    sub_id[5]  = 1005;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.r_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.r_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.r_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.r_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.r_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.r_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    process.stepid_list[0] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 1
    process.stepid_list[1] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 2
    process.stepid_list[2] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 3
    process.stepid_list[3] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 4
    process.stepid_list[4] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 5
    process.stepid_list[5] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 4
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region Second package 3. (Step list 1-6)
    telegram.r_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1006;
    sub_id[1]  = 1007;
    sub_id[2]  = 1008;
    sub_id[3]  = 1009;
    sub_id[4]  = 1010;
    sub_id[5]  = 1011;
    // Initial header address.
    telegram.r_request.header.address_2 = payload_start_index+6;         //
    telegram.r_request.header.address_3 = payload_start_index+12;        //
    telegram.r_request.header.address_4 = payload_start_index+18;        //
    telegram.r_request.header.address_5 = payload_start_index+24;        //
    telegram.r_request.header.address_6 = payload_start_index+30;        //
    telegram.r_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.r_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.r_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.r_request.telegram_array[payload_start_index+4]  = 0;
    telegram.r_request.telegram_array[payload_start_index+5]  = 0;
    // Package SID = 2
    telegram.r_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.r_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.r_request.telegram_array[payload_start_index+10] = 0;
    telegram.r_request.telegram_array[payload_start_index+11] = 0;
    // Package SID = 3
    telegram.r_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.r_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.r_request.telegram_array[payload_start_index+16] = 0;
    telegram.r_request.telegram_array[payload_start_index+17] = 0;
    // Package SID = 4
    telegram.r_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.r_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.r_request.telegram_array[payload_start_index+22] = 0;
    telegram.r_request.telegram_array[payload_start_index+23] = 0;
    // Package SID = 5
    telegram.r_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.r_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.r_request.telegram_array[payload_start_index+28] = 0;
    telegram.r_request.telegram_array[payload_start_index+29] = 0;
    // Package SID = 6
    telegram.r_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.r_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.r_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.r_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.r_request.telegram_array[payload_start_index+34] = 0;
    telegram.r_request.telegram_array[payload_start_index+35] = 0;
    // Package.
    telegram.r_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.r_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // Get basic parameter from telegram.
    process.stepid_list[6] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+4]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+5]<<8);  // SID = 1
    process.stepid_list[7] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+10]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+11]<<8); // SID = 2
    process.stepid_list[8] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+16]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+17]<<8); // SID = 3
    process.stepid_list[9] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+22]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+23]<<8); // SID = 4
    process.stepid_list[10] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+28]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+29]<<8); // SID = 5
    process.stepid_list[11] = (uint16_t)telegram.rw_response.telegram_array[payload_start_index+34]+
                            ((uint16_t)telegram.rw_response.telegram_array[payload_start_index+35]<<8); // SID = 4
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    
    #ifdef _DEBUG_MODE_
    // Display package 1.
    std::cout << "process.u16ProcPGain = " << std::to_string(process.u16ProcPGain) << std::endl;
    std::cout << "process.u16ProcIGain = " << std::to_string(process.u16ProcIGain) << std::endl;
    std::cout << "process.u16ProcMaxTime = " << std::to_string(process.u16ProcMaxTime) << std::endl;
    std::cout << "process.u16ProcMinTime = " << std::to_string(process.u16ProcMinTime) << std::endl;
    std::cout << "process.u16ProcRevFunc = " << std::to_string(process.u16ProcRevFunc) << std::endl;
    std::cout << "process.u16NbrSteps = " << std::to_string(process.u16NbrSteps) << std::endl;
    // package 2.
    std::cout << "process.step_id_1 = " << std::to_string(process.stepid_list[0]) << std::endl;
    std::cout << "process.step_id_2 = " << std::to_string(process.stepid_list[1]) << std::endl;
    std::cout << "process.step_id_3 = " << std::to_string(process.stepid_list[2]) << std::endl;
    std::cout << "process.step_id_4 = " << std::to_string(process.stepid_list[3]) << std::endl;
    std::cout << "process.step_id_5 = " << std::to_string(process.stepid_list[4]) << std::endl;
    std::cout << "process.step_id_6 = " << std::to_string(process.stepid_list[5]) << std::endl;
    // package 3.
    std::cout << "process.step_id_7 = " << std::to_string(process.stepid_list[6]) << std::endl;
    std::cout << "process.step_id_8 = " << std::to_string(process.stepid_list[7]) << std::endl;
    std::cout << "process.step_id_9 = " << std::to_string(process.stepid_list[8]) << std::endl;
    std::cout << "process.step_id_10 = " << std::to_string(process.stepid_list[9]) << std::endl;
    std::cout << "process.step_id_11 = " << std::to_string(process.stepid_list[10]) << std::endl;
    std::cout << "process.step_id_12 = " << std::to_string(process.stepid_list[11]) << std::endl;
    #endif
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::WriteProcessParameter(McbID4Struct &process, int processid)
 *
 *  @brief   Write MCB Process Parameter to MCB board.
 *
 *  @param   McbID4Struct &process
 *  
 *  @param   int processid
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::WriteProcessParameter(McbID4Struct &process, int processid)
{
    // Initial local parameter.
    int result = -1;
    int main_id = processid;
    uint16_t sub_id[6];
    uint16_t payload_start_index = 8;
    int delaytime = 100;

    #pragma region First package. (SID1-6)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 2;
    sub_id[1]  = 3;
    sub_id[2]  = 4;
    sub_id[3]  = 5;
    sub_id[4]  = 6;
    sub_id[5]  = 7;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.w_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(process.u16ProcPGain%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(process.u16ProcPGain/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.w_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(process.u16ProcIGain%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(process.u16ProcIGain/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.w_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(process.u16ProcMaxTime%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(process.u16ProcMaxTime/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.w_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(process.u16ProcMinTime%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(process.u16ProcMinTime/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.w_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(process.u16ProcRevFunc%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(process.u16ProcRevFunc/256);
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.w_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(process.u16NbrSteps%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(process.u16NbrSteps/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region First package. (SID1-6)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1000;
    sub_id[1]  = 1001;
    sub_id[2]  = 1002;
    sub_id[3]  = 1003;
    sub_id[4]  = 1004;
    sub_id[5]  = 1005;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.w_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(process.stepid_list[0]%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(process.stepid_list[0]/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.w_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(process.stepid_list[1]%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(process.stepid_list[1]/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.w_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(process.stepid_list[2]%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(process.stepid_list[2]/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.w_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(process.stepid_list[3]%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(process.stepid_list[3]/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.w_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(process.stepid_list[4]%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(process.stepid_list[4]/256);
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.w_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(process.stepid_list[5]%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(process.stepid_list[5]/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    // Get basic parameter from telegram.
    #pragma endregion
    #pragma region First package. (SID1-6)
    telegram.w_request.InitialTelegramArray();
    // Initial sub id array.
    sub_id[0]  = 1006;
    sub_id[1]  = 1007;
    sub_id[2]  = 1008;
    sub_id[3]  = 1009;
    sub_id[4]  = 1010;
    sub_id[5]  = 1011;
    // Initial header address.
    telegram.w_request.header.address_2 = payload_start_index+6;         //
    telegram.w_request.header.address_3 = payload_start_index+12;        //
    telegram.w_request.header.address_4 = payload_start_index+18;        //
    telegram.w_request.header.address_5 = payload_start_index+24;        //
    telegram.w_request.header.address_6 = payload_start_index+30;        //
    telegram.w_request.EncodeHeaderArray();
    // Package SID = 1
    telegram.w_request.telegram_array[payload_start_index]    = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+1]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+2]  = (uint8_t)(sub_id[0]%256);
    telegram.w_request.telegram_array[payload_start_index+3]  = (uint8_t)(sub_id[0]/256);
    telegram.w_request.telegram_array[payload_start_index+4]  = (uint8_t)(process.stepid_list[6]%256);
    telegram.w_request.telegram_array[payload_start_index+5]  = (uint8_t)(process.stepid_list[6]/256);
    // Package SID = 2
    telegram.w_request.telegram_array[payload_start_index+6]  = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+7]  = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+8]  = (uint8_t)(sub_id[1]%256);
    telegram.w_request.telegram_array[payload_start_index+9]  = (uint8_t)(sub_id[1]/256);
    telegram.w_request.telegram_array[payload_start_index+10] = (uint8_t)(process.stepid_list[7]%256);
    telegram.w_request.telegram_array[payload_start_index+11] = (uint8_t)(process.stepid_list[7]/256);
    // Package SID = 3
    telegram.w_request.telegram_array[payload_start_index+12] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+13] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+14] = (uint8_t)(sub_id[2]%256);
    telegram.w_request.telegram_array[payload_start_index+15] = (uint8_t)(sub_id[2]/256);
    telegram.w_request.telegram_array[payload_start_index+16] = (uint8_t)(process.stepid_list[8]%256);
    telegram.w_request.telegram_array[payload_start_index+17] = (uint8_t)(process.stepid_list[8]/256);
    // Package SID = 4
    telegram.w_request.telegram_array[payload_start_index+18] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+19] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+20] = (uint8_t)(sub_id[3]%256);
    telegram.w_request.telegram_array[payload_start_index+21] = (uint8_t)(sub_id[3]/256);
    telegram.w_request.telegram_array[payload_start_index+22] = (uint8_t)(process.stepid_list[9]%256);
    telegram.w_request.telegram_array[payload_start_index+23] = (uint8_t)(process.stepid_list[9]/256);
    // Package SID = 5
    telegram.w_request.telegram_array[payload_start_index+24] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+25] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+26] = (uint8_t)(sub_id[4]%256);
    telegram.w_request.telegram_array[payload_start_index+27] = (uint8_t)(sub_id[4]/256);
    telegram.w_request.telegram_array[payload_start_index+28] = (uint8_t)(process.stepid_list[10]%256);
    telegram.w_request.telegram_array[payload_start_index+29] = (uint8_t)(process.stepid_list[10]/256);
    // Package SID = .G.?
    telegram.w_request.telegram_array[payload_start_index+30] = (uint8_t)(main_id%256);
    telegram.w_request.telegram_array[payload_start_index+31] = (uint8_t)(main_id/256);
    telegram.w_request.telegram_array[payload_start_index+32] = (uint8_t)(sub_id[5]%256);
    telegram.w_request.telegram_array[payload_start_index+33] = (uint8_t)(sub_id[5]/256);
    telegram.w_request.telegram_array[payload_start_index+34] = (uint8_t)(process.stepid_list[11]%256);
    telegram.w_request.telegram_array[payload_start_index+35] = (uint8_t)(process.stepid_list[11]/256);
    // Package.
    telegram.w_request.EncodeTelegramArray();
    // Send to MCB.
    for(int index = 0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.w_request.telegram_array[index]);
    }
    // Get Data for telegram rw request.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    telegram.rw_response.InitialTelegramArray();
    result = read(com_num, &telegram.rw_response.telegram_array, 1024);
    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    #pragma endregion
    #pragma region display 
    WriteToMcbFlash(7,12,main_id);// Write to mcb flach,step number = mainid,
    #pragma endregion
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::InitialMcbComPort(std::string com_name_string)
 *
 *  @brief   Initial MCB comport.
 *
 *  @param   std::string com_name_string
 *
 *  @return  int
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::InitialMcbComPort(std::string com_name_string)
{
    int result = 0;
    // Initial ComPort.
    strcpy(com_name,com_name_string.c_str());
    com_num = comm.InitialComm(com_name);
    
    #ifdef _DEBUG_MODE_
    std::cout << "open com_num= " <<std::to_string(com_num) <<std::endl;
    #endif

    return result;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsMcbComm::GetMcbPollingStatus(GtcsCtrlTelegramStrcut &ctrltelegram)
 *
 *  @brief   Normal polling to MCB.
 *
 *  @param   GtcsCtrlTelegramStrcut &ctrltelegram
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsMcbComm::GetMcbPollingStatus(GtcsCtrlTelegramStrcut &ctrltelegram)
{
    int result = 0;
    int delaytime = 40;  // 20,30,40
    int MAX_READ = 1024; 

    // Encode ctrl telegram array.
    telegram.ctrl.EncodeTelegramArray(&ctrltelegram,telegram.ctrl.struct_length);
    // Send to MCB.
    for(int index=0;index<48;index++)
    {
        comm.SendChar(com_num,telegram.ctrl.telegram_array[index]);
    }
    // Read data form mcb.
    std::this_thread::sleep_for(std::chrono::milliseconds(delaytime));
    // 
    telegram.status.InitialTelegramArray();
    result = read(com_num, &telegram.status.telegram_array, MAX_READ);
    telegram.status.DecodeTelegramArray();
    telegram.status.CheckLoosenStatus(telegram.status.last_status.u16TMDFlags,telegram.status.current_status.u16TMDFlags);
    
    #if defined (_DEBUG_MODE_)
    std::cout <<"u16Statusflags = "<<std::to_string(telegram.status.current_status.u16Statusflags)<< std::endl;
    std::cout <<"u32ActError    = "<<std::to_string(telegram.status.current_status.u32ActError)<< std::endl;
    std::cout <<"u16ActProcNr   = "<<std::to_string(telegram.status.current_status.u16ActProcNr)<< std::endl;
    std::cout <<"u16ActStepNr   = "<<std::to_string(telegram.status.current_status.u16ActStepNr)<< std::endl;
    std::cout <<"u16ActCurr     = "<<std::to_string(telegram.status.current_status.u16ActCurr)<< std::endl;
    std::cout <<"u16ActTorque   = "<<std::to_string(telegram.status.current_status.u16ActTorque)<< std::endl;
    std::cout <<"u16ActRPM      = "<<std::to_string(telegram.status.current_status.u16ActRPM)<< std::endl;
    std::cout <<"u16MaxCurrent  = "<<std::to_string(telegram.status.current_status.u16MaxCurrent)<< std::endl;
    std::cout <<"u16MaxTorque   = "<<std::to_string(telegram.status.current_status.u16MaxTorque)<< std::endl;
    std::cout <<"u32Angle       = "<<std::to_string(telegram.status.current_status.u32Angle)<< std::endl;
    std::cout <<"u16TMDFlags    = "<<std::to_string(telegram.status.current_status.u16TMDFlags)<< std::endl;
    std::cout <<"s16Debug       = "<<std::to_string(telegram.status.current_status.s16Debug)<< std::endl;
    std::cout <<"s32Debug       = "<<std::to_string(telegram.status.current_status.s32Debug)<< std::endl;
    #endif

    // flush buffer.
    tcflush(com_num, TCIFLUSH);   	/* Discards old data in the rx buffer            */
	tcflush(com_num, TCOFLUSH);
    return result;
}
