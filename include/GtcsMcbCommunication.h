#ifndef _GTCS_MCB_TELEGRAM_
#define _GTCS_MCB_TELEGRAM_
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
#include <iostream>          // 
#include <array>             // 
#include <cstring>           // 
#include <vector>
#include "ComPort.h"
#include "GtcsTypeDefine.h"
#include "GtcsBulletin.h"
#include "CrcChecker.h"
#include <typeinfo>

#pragma region telegram strcut define.
class TelegramStruct
{
private:
    class Header{
        /* data */
    public:
       uint8_t type_num       = 0;
       uint8_t address_2      = 0;
       uint8_t address_3      = 0;
       uint8_t address_4      = 0;
       uint8_t address_5      = 0;
       uint8_t address_6      = 0;
       uint8_t RW_para_status = 0;
       uint8_t RW_ram_status  = 0;
    };
public:
    // Attribute
    TelegramStruct(/* args */);
    ~TelegramStruct();
    Header header;
    uint8_t header_start_index = 0;
    uint8_t payload_start_index = 8;
    std::array<uint8_t,48> telegram_array;
    // Method.
    uint8_t InitialTelegramArray();
    uint8_t EncodeHeaderArray();
    uint8_t EncodeTelegramArray();
    uint8_t DecodeTelegramArray();
}; 
// Ctrl Telegram.
class CtrlTelegram : public TelegramStruct
{
private:
    /* data */
public:
    CtrlTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::CTRL;
    };
    ~CtrlTelegram(){};
    GtcsCtrlTelegramStrcut fasten = {
        .u16Ctrlflags     = 9216, 
        .u16ControlMode   = 1,
        .u16WorkProc      = 4000,
        .u16CtrlProgram   = 1,
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 15,  
    }; 
    GtcsCtrlTelegramStrcut loosen = {
        .u16Ctrlflags     = 9216,       // 
        .u16ControlMode   = 0,          // Manuala mode
        .u16WorkProc      = 4000,
        .u16CtrlProgram   = 1,
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 15,
    };
    GtcsCtrlTelegramStrcut self_leaning = {
        .u16Ctrlflags     = 9216,       // 
        .u16ControlMode   = 0,          // Manuala mode
        .u16WorkProc      = 4000,
        .u16CtrlProgram   = 1,
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 15,
    };
    int struct_length = 10;
    int EncodeTelegramArray(GtcsCtrlTelegramStrcut *ptr_ctrl_telegram,int length);
};
// Status Telegram.
class StatusTelegram : public TelegramStruct
{
private:
/* data */
public:
    StatusTelegram(/* args */)
    {
        header.type_num  = MCB_TELEGRAM_TYPE::STATUS;
    };
    ~StatusTelegram(){};
    GtcsStatusTelegramStrcut mcb_status = 
    {
        .u16Statusflags = 0,   // uint16
        .u32ActError    = 0,
        .u16ActProcNr   = 0,
        .u16ActStepNr   = 0,
        .u16ActCurr     = 0,
        .u16ActTorque   = 0,
        .u16ActRPM      = 0,
        .u16MaxCurrent  = 0,
        .u16MaxTorque   = 0,
        .u32Angle       = 0,      
        .u32Revolutions = 0,
        .u16TMDFlags    = 0,
        .s16Debug       = 0,
        .s32Debug       = 0, 
    };
    // int DecodeTelegramArray(std::array<std::uint8_t,48> received_telegram);
    int DecodeTelegramArray();
};
// Wrie Request Telegram
class WriteRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    WriteRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::W_REQUEST;
    };
    ~WriteRequestTelegram(){};
    GtcsRW16TelegramStruct payload_data = {
        .address_data_1 = 0,
        .address_data_2 = 0,
        .address_data_3 = 0,
        .address_data_4 = 0,
        .address_data_5 = 0,
        .address_data_6 = 0,
    };
    std::array<uint8_t,36> EncodeTelegramArray(GtcsRW16TelegramStruct *ptr_request);
};
// Response Telegram.
class ResponseTelegram : public TelegramStruct 
{
private:
    /* data */
public:
    ResponseTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::RW_RESPONSE;
    };
    ~ResponseTelegram(){};
    GtcsRW16TelegramStruct payload_data = {
        .address_data_1 = 0,
        .address_data_2 = 0,
        .address_data_3 = 0,
        .address_data_4 = 0,
        .address_data_5 = 0,
        .address_data_6 = 0,
    };
};
// Read request telegram.
class ReadRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    ReadRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::R_REQUEST;
    };
    ~ReadRequestTelegram(){};
    GtcsRW16TelegramStruct payload_data = {
        .address_data_1 = 0,
        .address_data_2 = 0,
        .address_data_3 = 0,
        .address_data_4 = 0,
        .address_data_5 = 0,
        .address_data_6 = 0,
    };
};
// TMD Request telegram. 
class TMDRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    TMDRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::TMD_REQUEST;
    };
    ~TMDRequestTelegram(){};
};
// TMD Response Telegram.
class TMDResponseTelegram : public TelegramStruct
{
private:
    /* data */
public:
    TMDResponseTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::TMD_RESPONSE;
    };
    ~TMDResponseTelegram(){};
};    
// Data log request telegram.
class DataLogRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    DataLogRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::LOG_REQUEST;
    };
    ~DataLogRequestTelegram(){};
};
// Data Log Response telegram
class DataLogResponseTelegram : public TelegramStruct
{
private:
    /* data */
public:
    DataLogResponseTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::LOG_RESPONSE;
    };
    ~DataLogResponseTelegram(){};
};
// Process Data Request telegram.
class ProcessDataRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    ProcessDataRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::PROCESS_REQUST;
    };
    ~ProcessDataRequestTelegram(){};
};
// Process Data Response Telegram.
class ProcessDataResponseTelegram : public TelegramStruct
{
private:
    /* data */
public:
    ProcessDataResponseTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::PROCESS_RESPONSE;
    };
    ~ProcessDataResponseTelegram(){};
};
// Step Data Request Telegram/
class StepDataRequestTelegram : public TelegramStruct
{
private:
    /* data */
public:
    StepDataRequestTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::STEP_REQUEST;
    };
    ~StepDataRequestTelegram(){};
};    
// Step Data Response Telegram.
class StepDataResponseTelegram : public TelegramStruct
{
private:
    /* data */
public:
    StepDataResponseTelegram(/* args */)
    {
        header.type_num = MCB_TELEGRAM_TYPE::STEP_RESPONSE;
    };
    ~StepDataResponseTelegram(){};
};    
#pragma endregion

#pragma region Telegram
class GtcsMcbTelegram
{
private:
public:
    GtcsMcbTelegram(/* args */){};
    ~GtcsMcbTelegram(){};
    CtrlTelegram                ctrl;            // type_num = 1
    StatusTelegram              status;          // type_num = 2
    WriteRequestTelegram        w_request;       // type_num = 3
    ResponseTelegram            rw_response;     // type_num = 4  
    ReadRequestTelegram         r_request;       // type_num = 5
    TMDRequestTelegram          tmd_request;     // type_num = 6
    TMDResponseTelegram         tmd_response;    // type_num = 7
    DataLogRequestTelegram      datalog_request; // type_num = 10
    DataLogResponseTelegram     datalog_response;// type_num = 11
    ProcessDataRequestTelegram  process_request; // type_num = 12
    ProcessDataResponseTelegram process_response;// type_num = 13
    StepDataRequestTelegram     step_request;    // type_num = 14
    StepDataResponseTelegram    step_response;   // type_num = 15
};
#pragma endregion
#pragma region Telgram
class GtcsMcbCommunication
{
private:
    /* data */
    static GtcsMcbCommunication* instance;
    GtcsMcbCommunication(/* args */);
    #pragma region RW MCB Parameter.
    // Identification Parameter.(MainID = 1)
    int ReadIdentificationParameter();
    int WriteIdentificationParameter();
    // Basic Parameter.(MainID = 2)
    int ReadBasicParameter(McbID2Struct *ptr_basic_para);    
    int WriteBasicParameter();
    // Step Parameter. (Main ID = 3)
    int ReadStepParametrer();
    int WriteStepParameter();
    // Prcoess Parameter.(Main ID = 4)
    int ReadProcessParameter();
    int WriteProcessParameter();
    int ReadProcessStepList();
    int WritePrcessStepList();
    #pragma endregion
public:
    ~GtcsMcbCommunication();    
    static GtcsMcbCommunication* GetInstance();
    // GtcsParameter parameter;
    GtcsMcbTelegram telegram;
};
#pragma endregion
#endif