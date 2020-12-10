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
#include "../include/gtcsmcbtelegram.h"
// Constructor.
GtcsMCBTelegram::GtcsMCBTelegram(/* args */)
{}
// Distructor.
GtcsMCBTelegram::~GtcsMCBTelegram()
{}
GtcsMCBTelegram* GtcsMCBTelegram::instance = 0;
// Get Instance.
GtcsMCBTelegram* GtcsMCBTelegram::GetInstance()
{
    if(instance == 0)
    {
        instance = new GtcsProtocol();
    }
    return instance;
}

// Gtcs Telegram TDD.
int main()
{
    
}