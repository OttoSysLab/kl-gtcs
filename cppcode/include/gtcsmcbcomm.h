#pragma once
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
#include <cstring>           // 
#include <vector>
#include <thread>
#include <ctime>
#include <chrono>

#include "comport.h"
#include "gtcsgloabdefine.h"
#include "gtcsbulletin.h"
#include "crcchecker.h"

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
    std::array<uint8_t,1024> telegram_array;
    // Method.
    int InitialTelegramArray();
    int EncodeHeaderArray();
    int EncodeTelegramArray();
    int DecodeTelegramArray();
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
        .u16Ctrlflags     = 0,     
        .u16ControlMode   = 1,          // Manuala mode = 0,Process mode = 1,Process ram mode = 4
        .u16WorkProc      = 4000,       // 4248
        .u16CtrlProgram   = 1,          
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 4,          // 8 = bule  
    }; 
    GtcsCtrlTelegramStrcut loosen = {
        .u16Ctrlflags     = 0,          // 
        .u16ControlMode   = 0,          // Manuala mode = 0,Process mode = 1,Process ram mode = 4,
        .u16WorkProc      = 4000,       
        .u16CtrlProgram   = 1,          
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 8,          // green = 4
    };
    GtcsCtrlTelegramStrcut config = {
        .u16Ctrlflags     = 0,          // 
        .u16ControlMode   = 0,          // Manuala mode = 0,Process mode = 1,
        .u16WorkProc      = 4000,       
        .u16CtrlProgram   = 1,
        .u16ManRpm        = 1000,
        .u16ManSlope      = 1000,
        .u16ManMaxTorque  = 1862,
        .u16ManMaxCurrent = 30000,
        .u16ManRpmMode    = 0,
        .u8TMDControl     = 12,
    };
    GtcsCtrlTelegramStrcut self_leaning = {
        .u16Ctrlflags     = 0,       // 
        .u16ControlMode   = 1,          // Manuala mode
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
    // Ctrl telegram flags configuration.
    void InitialCtrlFlags(GtcsCtrlTelegramStrcut &telegram);
    void SetCtrlFlags(GtcsCtrlTelegramStrcut &telegram,int flagIdx);

    // Encode Telegram array.
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
    GtcsStatusTelegramStrcut last_status = 
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
    GtcsStatusTelegramStrcut current_status = 
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
    int CheckLoosenStatus(uint16_t last_status_flags,uint16_t current_status_flags);
    bool loosen_status = false;
    int check_loosen_cnt = 0;
    // int lc_status = 0;
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
#pragma region GtcsMcbComm
class GtcsMcbComm
{
private:
    /* data */
    static GtcsMcbComm* instance;
    GtcsMcbComm(/* args */);
    // Initial MCB comm.
    ComPort comm;
    char com_name[128];
    int com_num = 0; 

public:
    ~GtcsMcbComm();    
    static GtcsMcbComm* GetInstance();
    // GtcsParameter parameter;
    GtcsMcbTelegram telegram;
    // Initial MCB com.
    int InitialMcbComPort(std::string com_bname);
    
    #pragma region RW MCB Parameter. 
    // Identification Parameter.(MainID = 1)
    int ReadIdentificationParameter();
    int WriteIdentificationParameter();
    // Basic Parameter.(MainID = 2)
    int ReadBasicParameter(McbID2Struct &basic_para);    
    int WriteBasicParameter(McbID2Struct &basic);
    // Write to mcb flash.
    int WriteToMcbFlash(int mainid,int subid ,int add_num); // mainid = 7 , subid = 11(step) ,subid = 12 (Process)
    // Step Parameter. (Main ID = 3)
    int ReadStepParametrer(McbID3Struct &step_para,int mainid);
    int WriteStepParameter(McbID3Struct &step_para,int mainid);
    // Prcoess Parameter.(Main ID = 4)
    int ReadProcessParameter(McbID4Struct &process,int processnum);
    int WriteProcessParameter(McbID4Struct &process,int processid);
    #pragma endregion

    // Process ram mode.
    int WriteProcessRamData(McbID4Struct &process);
    int WriteStepRamData(McbID3Struct &step,uint16_t mainid);

    // Polling to MCB.
    int CheckLoosenStatus();
    int GetMcbPollingStatus(GtcsCtrlTelegramStrcut &ctrltelegram);
};
#pragma endregion
