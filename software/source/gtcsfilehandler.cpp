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
#include "gtcsfilehandler.h"

/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      FileHandler::FileHandler()
 *
 *  @brief   FileHandler constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
FileHandler::FileHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      FileHandler::~FileHandler()
 *
 *  @brief   FileHandler distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
FileHandler::~FileHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsFileHandler::GtcsFileHandler()
 *
 *  @brief   GtcsFileHandler constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsFileHandler::GtcsFileHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsFileHandler::~GtcsFileHandler()
 *
 *  @brief   GtcsFileHandler distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsFileHandler::~GtcsFileHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsFileHandler::~GtcsFileHandler()
 *
 *  @brief   GtcsFileHandler distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsFileHandler::ClearTxtFile(std::string filePath)
{
	// Initial parameter.
	std::fstream txt_file;
	// Clear file.	
	txt_file.open(filePath,std::ios::out|std::ios::trunc); // std::ios::app,std::ios::out, std::ios::ate.
	txt_file.close();
	return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/23
 *
 *  @fn      GtcsFileHandler::~GtcsFileHandler()
 *
 *  @brief   GtcsFileHandler distructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsFileHandler::WriteToTxtFile(std::string filePath,std::string data)
{
	// Initial parameter.
	std::fstream txt_file;
	// Open Txt file.
	txt_file.open(filePath,std::ios::app); // std::ios::out, std::ios::ate
	if (txt_file.is_open())
	{
		// Write data to .txt file. 
		txt_file << data;
		txt_file << "\n";
		// close .txt file.
		txt_file.close();
	}
	else
	{
		std::cout << "File not created txt file !" << std::endl;
		return false;	
	}
	return true;
}

