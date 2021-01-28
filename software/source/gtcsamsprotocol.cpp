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
#include "../include/gtcsamsprotocol.h"

#pragma region Signalton.
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Constructor.
GtcsAmsProtocol::GtcsAmsProtocol(/* args */)
{}
// Distructor.
GtcsAmsProtocol::~GtcsAmsProtocol()
{}
// SignleTon instance object.
GtcsAmsProtocol* GtcsAmsProtocol::instance = 0;
// Get Instance.
GtcsAmsProtocol* GtcsAmsProtocol::GetInstance()
{
    if(instance == 0){
        instance = new GtcsAmsProtocol();
    }
    return instance;
}
#pragma endregion
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Get AMS string 
int GtcsAmsProtocol::ConvertToProtocolString(std::string *ptr,std::string & result)
{
    result = "{"+*ptr;
    while(true)
    {
        ptr = (std::string *)(void *)(ptr+1);
        if (*ptr != "\n\r")
        {
            result += ","+*ptr;
        }
        else
        {
            break;
        }
    }
    result += "}";
    return 0;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
int GtcsAmsProtocol::GetAmsSymmetryCmdNumver(int amscmd)
{
    int result = 0;
    switch (amscmd)
    {
    #pragma region REQ
    case AMSCMD::REQ300:      
        break;
    #pragma endregion  

    #pragma region CMD
    case AMSCMD::CMD301:
        result = AMSCMD::ANS301;
        break;
    case AMSCMD::CMD302:
        result = AMSCMD::ANS302;
        break;
    case AMSCMD::CMD310:
        result = AMSCMD::ANS310;
        break;
    case AMSCMD::CMD311:
        result = AMSCMD::ANS311;
        break;
    case AMSCMD::CMD312:
        result = AMSCMD::ANS312;
        break;
    case AMSCMD::CMD320:
        result = AMSCMD::ANS320;
        break;
    case AMSCMD::CMD321:
        result = AMSCMD::ANS321;
        break;
    case AMSCMD::CMD322:
        result = AMSCMD::ANS322;
        break;
    case AMSCMD::CMD323:
        result = AMSCMD::ANS323;
        break;
    case AMSCMD::CMD330:
        result = AMSCMD::ANS330;
        break;
    case AMSCMD::CMD340:
        result = AMSCMD::ANS340;
        break;
    #pragma endregion
    }
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Get AMS Protocol string from bulletin.
std::string GtcsAmsProtocol::GetAmsBulletin(int amscmd)
{
    GtcsBulletin* gtcsbulletin = GtcsBulletin::GetInstance();
    AMSBulletin* amsbulletin =&gtcsbulletin->AmsBulletin;
    std::string result;      
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:     
        // ptr_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
        // ConvertToProtocolString(ptr_str,result);    
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->DATA300Struct,result);
        break; 
    case AMSCMD::DATA302:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->DATA302Struct,result);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->REQ300Struct,result);
        break;
    case AMSCMD::REQ301:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->REQ301Struct,result);
        break;
    case AMSCMD::REQ302:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->REQ302Struct,result);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD300Struct,result);
        break;
    case AMSCMD::CMD301:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD301Struct,result);
        break;
    case AMSCMD::CMD302:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD302Struct,result);
        break;
    case AMSCMD::CMD303:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD303Struct,result);
        break;
    case AMSCMD::CMD310:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD310Struct,result);
        break;
    case AMSCMD::CMD311:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD311Struct,result);
        break;
    case AMSCMD::CMD312:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD312Struct,result);
        break;
    case AMSCMD::CMD320:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD320Struct,result);
        break;
    case AMSCMD::CMD321:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD321Struct,result);
        break;
    case AMSCMD::CMD322:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD322Struct,result);
        break;
    case AMSCMD::CMD323:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD323Struct,result);
        break;
    case AMSCMD::CMD330:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD330Struct,result);
        break;
    case AMSCMD::CMD340:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->CMD340Struct,result);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS301Struct,result);
        break;
    case AMSCMD::ANS302:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS302Struct,result);
        break;
    case AMSCMD::ANS310:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS310Struct,result);
        break;
    case AMSCMD::ANS311:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS311Struct,result);
        break;
    case AMSCMD::ANS320:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS320Struct,result);
        break;
    case AMSCMD::ANS321:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS321Struct,result);
        break;
    case AMSCMD::ANS322:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS322Struct,result);
        break;
    case AMSCMD::ANS323:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS323Struct,result);
        break;
    case AMSCMD::ANS330:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS330Struct,result);
        break;
    case AMSCMD::ANS340:
        ConvertToProtocolString((std::string *)(void *)&amsbulletin->ANS340Struct,result);
        break;
    default:
        break;
    #pragma endregion
    }
    //  std::cout<<result<<std::endl;
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Split String.
std::vector<std::string> GtcsAmsProtocol::SplitString(const std::string & str,const std::string& pattern)
{
    std::vector<std::string> result;
	//string::size_type型別，left：左邊界位置  right：右邊界位置 
	std::string::size_type left, right;
	
	right = str.find(pattern);
	left = 0;
	
	while(right != std::string::npos)
  	{
  		//以免字串首部就是分割依據，壓入長度為0的字串 
  		if(right-left)
  		{
  			//壓入起始位置為left，長度為（right-left）的字串 
    		result.push_back(str.substr(left, right-left)); 
  		}	
    	left = right + pattern.size();     //右邊界右移分割依據的長度，作為新的左邊界 
    	right = str.find(pattern, left);   //從left這個位置開始find 
  	}  	
  	//退出迴圈時，左邊界不是最後一個元素 
  	if(left != str.length())
  	{
  		result.push_back(str.substr(left));
  	}
  	return result; 
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Get Ams SpliteArray.
std::vector<std::string> GtcsAmsProtocol::GetAmsSpliteArray(const std::string & str)
{
    std::vector<std::string> splite_1,splite_2,splite_3, result;
    splite_1 = SplitString(str,"{");        // 分第一包
    splite_2 = SplitString(splite_1[0],"}");// 分第二包
    result = SplitString(splite_2[0],",");  // 分結果
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Get CMD enum number. 
int GtcsAmsProtocol::GetAmsCmdNum(std::string amscmd)
{
    int result = 0;
    if (amscmd == "REQ300"){
        result = AMSCMD::REQ300;
    }
    else if (amscmd == "DATA300")
    {
        result = AMSCMD::DATA300;
    }    
    else if (amscmd =="CMD301")
    {
        result = AMSCMD::CMD301;
    }
    else if(amscmd == "CMD302")
    {
        result = AMSCMD::CMD302;
    }
    else if(amscmd == "CMD303")
    {
        result = AMSCMD::CMD303;
    }
    else if(amscmd == "CMD340")
    {
        result = AMSCMD::CMD340;
    }
    else 
    {
        result = AMSCMD::REQ300;    //  
    }
    
    return result;    
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Update Protocol Struct.
int GtcsAmsProtocol::UpdateProtocolStruct(std::string* ptr,std::vector<std::string>& ams_array)
{
    // std::cout << std::to_string(ams_array.size()) << std::endl;
    // std::cout << ams_array[0] << std::endl;
    int index = 0;
    *ptr = ams_array[index];
    while(true)
    {
        index += 1;
        ptr = (std::string *)(void *)(ptr+1);
        if (*ptr!="\n\r")
        {
            *ptr = ams_array[index];
        }
        else
        {
            break;
        }
    }
    return 0;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
// Set AMS Protocol struct to bulletin.
std::string GtcsAmsProtocol::SetAmsBulletin(std::string ams_string)
{
    std::string result = "-";
    GtcsBulletin* gtcsbulletin = GtcsBulletin::GetInstance();
    AMSBulletin* amsbulletin =& gtcsbulletin->AmsBulletin;

    std::vector<std::string> ams_arry = GetAmsSpliteArray(ams_string);
    int amscmd = GetAmsCmdNum(ams_arry[0]);
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:      
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->DATA300Struct,ams_arry);
        break; 
    case AMSCMD::DATA302:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->DATA302Struct,ams_arry);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->REQ300Struct,ams_arry);
        break;
    case AMSCMD::REQ301:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->REQ301Struct,ams_arry);
        break;
    case AMSCMD::REQ302:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->REQ302Struct,ams_arry);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD300Struct,ams_arry);
        break;
    case AMSCMD::CMD301:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD301Struct,ams_arry);
        break;
    case AMSCMD::CMD302:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD302Struct,ams_arry);
        break;
    case AMSCMD::CMD303:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD303Struct,ams_arry);
        break;
    case AMSCMD::CMD310:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD310Struct,ams_arry);
        break;
    case AMSCMD::CMD311:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD311Struct,ams_arry);
        break;
    case AMSCMD::CMD312:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD312Struct,ams_arry);
        break;
    case AMSCMD::CMD320:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD320Struct,ams_arry);
        break;
    case AMSCMD::CMD321:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD321Struct,ams_arry);
        break;
    case AMSCMD::CMD322:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD322Struct,ams_arry);
        break;
    case AMSCMD::CMD323:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD323Struct,ams_arry);
        break;
    case AMSCMD::CMD330:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD330Struct,ams_arry);
        break;
    case AMSCMD::CMD340:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->CMD340Struct,ams_arry);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS301Struct,ams_arry);
        break;
    case AMSCMD::ANS302:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS302Struct,ams_arry);
        break;
    case AMSCMD::ANS310:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS310Struct,ams_arry);
        break;
    case AMSCMD::ANS311:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS311Struct,ams_arry);
        break;
    case AMSCMD::ANS320:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS320Struct,ams_arry);
        break;
    case AMSCMD::ANS321:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS321Struct,ams_arry);
        break;
    case AMSCMD::ANS322:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS322Struct,ams_arry);
        break;
    case AMSCMD::ANS323:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS323Struct,ams_arry);
        break;
    case AMSCMD::ANS330:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS330Struct,ams_arry);
        break;
    case AMSCMD::ANS340:
        UpdateProtocolStruct((std::string *)(void *)&amsbulletin->ANS340Struct,ams_arry);
        break;
    default:
        break;
    #pragma endregion
    }        
    result = ams_arry[0];
    return result;
} 

