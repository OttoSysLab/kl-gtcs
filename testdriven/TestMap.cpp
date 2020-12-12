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
#include <string>
#include <map>
#include <iostream>
#include <vector>

typedef struct {
    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;
}CMD300;
// main.
int main()
{
    std::map<std::string,std::string> aa;
    aa["aa"]="FUCK";
    // std::map<std::string,std::string> aa
    // {
    //     "aa":"FUCK";
    // }
    std::cout<<aa["aa"]<<std::endl;

    CMD300 cmd300={
        .str1 = "OTTO",
        .str2 = "1234",
        .str3 = "etjnl",
        .str4 = "etjnl",
    };
    
    CMD300 *prt;
    std::cout<<cmd300.str1<<std::endl;
    
    prt = &cmd300;
    std::cout<<sizeof(cmd300)<<std::endl;
    return 0;
}