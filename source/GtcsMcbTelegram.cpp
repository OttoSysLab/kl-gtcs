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
#include "../include/GtcsMcbTelegram.h"
// Constructor.
GtcsMCBProtocol::GtcsMCBProtocol(/* args */)
{}
// Distructor.
GtcsMCBProtocol::~GtcsMCBProtocol()
{}
// SignleTon instance object.
GtcsMCBProtocol* GtcsMCBProtocol::instance = 0;
// Get Instance.
GtcsMCBProtocol* GtcsMCBProtocol::GetInstance()
{
    if(instance == 0)
    {
        instance = new GtcsMCBProtocol();
    }
    return instance;
}
// Get MCB bulletin.
std::vector<std::string> GtcsMCBProtocol::GetMCBBulletin(MCBMAID mainid)
{
    std::vector<std::string> result;
    
    return result;
}
// Set MCB Mulletin.
int GtcsMCBProtocol::SetMCBBulletin(MCBMAID mainid)
{
    int result = 0;
    return result;
}

#if defined(_GTCS_MCB_TELEGRAM_TEST_)
// Set send data.
std::array<uint8_t,48> GetTelegramData()
{
    std::array<uint8_t,48> mcb_ctrl_telegram;
    #pragma region  
    mcb_ctrl_telegram[0] = 0x01;
    mcb_ctrl_telegram[1] = 0x00;
    mcb_ctrl_telegram[2] = 0x00;
    mcb_ctrl_telegram[3] = 0x00;
    mcb_ctrl_telegram[4] = 0x00;
    mcb_ctrl_telegram[5] = 0x00;
    mcb_ctrl_telegram[6] = 0x00;
    mcb_ctrl_telegram[7] = 0x00;
    mcb_ctrl_telegram[8] = 0x01;
    mcb_ctrl_telegram[9] = 0x24;
    mcb_ctrl_telegram[10] = 0x00;
    mcb_ctrl_telegram[11] = 0x00;
    mcb_ctrl_telegram[12] = 0xa0;
    mcb_ctrl_telegram[13] = 0x0f;
    mcb_ctrl_telegram[14] = 0x01;
    mcb_ctrl_telegram[15] = 0x00;
    mcb_ctrl_telegram[16] = 0xe8;
    mcb_ctrl_telegram[17] = 0x03;
    mcb_ctrl_telegram[18] = 0xe8;
    mcb_ctrl_telegram[19] = 0x03;
    mcb_ctrl_telegram[20] = 0x74;
    mcb_ctrl_telegram[21] = 0x01;
    mcb_ctrl_telegram[22] = 0xe8;
    mcb_ctrl_telegram[23] = 0x03;
    mcb_ctrl_telegram[24] = 0x00;
    mcb_ctrl_telegram[25] = 0x00;
    mcb_ctrl_telegram[26] = 0x00;
    mcb_ctrl_telegram[27] = 0x00;
    mcb_ctrl_telegram[28] = 0x08;
    mcb_ctrl_telegram[29] = 0x07;
    mcb_ctrl_telegram[30] = 0x00;
    mcb_ctrl_telegram[31] = 0x00;
    mcb_ctrl_telegram[32] = 0x00;
    mcb_ctrl_telegram[33] = 0x00;
    mcb_ctrl_telegram[34] = 0x00;
    mcb_ctrl_telegram[35] = 0x00;
    mcb_ctrl_telegram[36] = 0x00;
    mcb_ctrl_telegram[37] = 0x00;
    mcb_ctrl_telegram[38] = 0x00;
    mcb_ctrl_telegram[39] = 0x00;
    mcb_ctrl_telegram[40] = 0x00;
    mcb_ctrl_telegram[41] = 0x00;
    mcb_ctrl_telegram[42] = 0x00;
    mcb_ctrl_telegram[43] = 0x00;
    mcb_ctrl_telegram[44] = 0x46;   // DEV = 70
    mcb_ctrl_telegram[45] = 0xcf;   // DEV = 206
    mcb_ctrl_telegram[46] = 0xba;   // DEV = 186
    mcb_ctrl_telegram[47] = 0xbe;   // DEV = 190
    #pragma endregion

    return mcb_ctrl_telegram;
}

// Gtcs Telegram TDD.
int main()
{
    // Test Singleton.
    GtcsMCBProtocol* mcb = GtcsMCBProtocol::GetInstance();
    GtcsBulletin* bulletin = GtcsBulletin::GetInstance();
    std::cout << bulletin->McbBulletin.MID1.u8DmsCorel << std::endl;
    
    return 0;
}
#endif
