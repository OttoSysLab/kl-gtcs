#ifndef _GTCS_MCB_TELEGRAM_
#define _GTCS_MCB_TELEGRAM_
#define _GTCS_MCB_TELEGRAM_TEST_
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
#include "gtcstypedefine.h"
#include "gtcsbulletin.h"

#pragma region Telegram 
class Header
{
private:
    /* data */
public:
    Header(/* args */){};
    ~Header(){};
    uint8_t type_num = 0;
    uint8_t addr_2 = 0;
    uint8_t addr_3 = 0;
    uint8_t addr_4 = 0;
    uint8_t addr_5 = 0;
    uint8_t addr_6 = 0;
    uint8_t status = 0;
};

class GtcsTelegram
{
private:   
    #pragma region telegram strcut define.
    // Ctrl Telegram.
    class CtrlTelegram
    {
    private:
        /* data */
    public:
        Header header;
        CtrlTelegram(/* args */)
        {
            header.type_num = 1;
        };
        ~CtrlTelegram(){};
    };
    // Status Telegram.s
    class StatusTelegram
    {
    private:
        /* data */
    public:
        Header header;
        StatusTelegram(/* args */)
        {
            header.type_num = 2;
        };
        ~StatusTelegram(){};
    };
    // Wrie Request Telegram
    class WriteRequestTelegram
    {
    private:
        /* data */
    public:
        Header header;
        WriteRequestTelegram(/* args */)
        {
            header.type_num = 3;
        };
        ~WriteRequestTelegram(){};
    };
    // Read request telegram.
    class ReadRequestTelegram
    {
    private:
        /* data */
    public:
        Header header;
        ReadRequestTelegram(/* args */)
        {
            header.type_num = 5;
        };
        ~ReadRequestTelegram(){};
    };
    // Response Telegram.
    class ResponseTelegram
    {
    private:
        /* data */
    public:
        Header header;
        ResponseTelegram(/* args */)
        {
            header.type_num = 4;
        };
        ~ResponseTelegram(){};
    };
    // TMD Request telegram. 
    class TMDRequestTelegram
    {
    private:
        /* data */
    public:
        TMDRequestTelegram(/* args */);
        ~TMDRequestTelegram(){};
    };
    // TMD Response Telegram.
    class TMDResponseTelegram
    {
    private:
        /* data */
    public:
        TMDResponseTelegram(/* args */)
        {

        };
        ~TMDResponseTelegram(){};
    };    
    // Data log request telegram.
    class DataLogRequestTelegram
    {
    private:
        /* data */
    public:
        DataLogRequestTelegram(/* args */)
        {

        };
        ~DataLogRequestTelegram(){};
    };
    // Data Log Response telegram
    class DataLogResponseTelegram
    {
    private:
        /* data */
    public:
        DataLogResponseTelegram(/* args */)
        {

        };
        ~DataLogResponseTelegram(){};
    };
    // Process Data Request telegram.
    class ProcessDataRequestTelegram
    {
    private:
        /* data */
    public:
        ProcessDataRequestTelegram(/* args */)
        {

        };
        ~ProcessDataRequestTelegram(){};
    };
    // Process Data Response Telegram.
    class ProcessDataResponseTelegram
    {
    private:
        /* data */
    public:
        ProcessDataResponseTelegram(/* args */)
        {

        };
        ~ProcessDataResponseTelegram(){};
    };
    // Step Data Request Telegram/
    class StepDataRequestTelegram
    {
    private:
        /* data */
    public:
        StepDataRequestTelegram(/* args */)
        {

        };
        ~StepDataRequestTelegram(){};
    };    
    // Step Data Response Telegram.
    class StepDataResponseTelegram
    {
    private:
        /* data */
    public:
        StepDataResponseTelegram(/* args */)
        {

        };
        ~StepDataResponseTelegram(){};
    };    
    #pragma endregion
public:
    GtcsTelegram(/* args */){};
    ~GtcsTelegram(){};
    CtrlTelegram CtrlTetegram;                 // type_num = 1
    StatusTelegram StatusTelegram;             // type_num = 2
    WriteRequestTelegram WriteRequestTelegram; // type_num = 3
    ReadRequestTelegram  ReadRequestTelegram;  // type_num = 5
};
#pragma endregion
#pragma region Telgram
class GtcsMCBProtocol
{
private:
    /* data */
    static GtcsMCBProtocol* instance;
    GtcsMCBProtocol(/* args */);
public:
    ~GtcsMCBProtocol();    
    static GtcsMCBProtocol* GetInstance();
    // GtcsParameter parameter;
    GtcsTelegram telegram;
    std::vector<std::string> GetMCBBulletin(MCBMAID mainid);
    int SetMCBBulletin(MCBMAID mainid);
};
#pragma endregion
#endif