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
#include "../include/GtcsBulletin.h"

// Signalton.
// Constructor.
GtcsBulletin::GtcsBulletin(/* args */)
{}
// Distructor.
GtcsBulletin::~GtcsBulletin()
{}
// SignleTon instance object.
GtcsBulletin* GtcsBulletin::instance = 0;
// Get Instance.
GtcsBulletin* GtcsBulletin::GetInstance()
{
    if(instance == 0){
        instance = new GtcsBulletin();
    }
    return instance;
}
