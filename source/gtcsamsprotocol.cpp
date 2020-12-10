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
// Signalton.
// Constructor.
GtcsAMSProtocol::GtcsAMSProtocol(/* args */)
{}
// Distructor.
GtcsAMSProtocol::~GtcsAMSProtocol()
{}
// SignleTon instance object.
GtcsAMSProtocol* GtcsAMSProtocol::instance = 0;
// Get Instance.
GtcsAMSProtocol* GtcsAMSProtocol::GetInstance()
{
    if(instance == 0){
        instance = new GtcsAMSProtocol();
    }
    return instance;
}
// Get AMS string 
int GtcsAMSProtocol::ConvertToProtocolString(std::string* prt,std::string & result)
{
    result = "{"+*prt;
    while(true){
        prt = (std::string *)(void *)(prt+1);
        if (*prt != "\n\r"){
            result += ","+*prt;
        }
        else{
            break;
        }
    }
    result += "}";
    return 0;
}
// Get AMS Protocol string from bulletin.
std::string GtcsAMSProtocol::GetAMSBulletin(AMSCMD amscmd)
{
    std::string result;      
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:     
        // prt_str = (std::string *)(void *)&amsBulletin.DATA300Struct;
        // ConvertToProtocolString(prt_str,result);    
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA300Struct,result);
        break; 
    case AMSCMD::DATA302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.DATA302Struct,result);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ300Struct,result);
        break;
    case AMSCMD::REQ301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ301Struct,result);
        break;
    case AMSCMD::REQ302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.REQ302Struct,result);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD300Struct,result);
        break;
    case AMSCMD::CMD301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD301Struct,result);
        break;
    case AMSCMD::CMD302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD302Struct,result);
        break;
    case AMSCMD::CMD303:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD303Struct,result);
        break;
    case AMSCMD::CMD310:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD310Struct,result);
        break;
    case AMSCMD::CMD311:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD311Struct,result);
        break;
    case AMSCMD::CMD312:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD312Struct,result);
        break;
    case AMSCMD::CMD320:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD320Struct,result);
        break;
    case AMSCMD::CMD321:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD321Struct,result);
        break;
    case AMSCMD::CMD322:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD322Struct,result);
        break;
    case AMSCMD::CMD323:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD323Struct,result);
        break;
    case AMSCMD::CMD330:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD330Struct,result);
        break;
    case AMSCMD::CMD340:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.CMD340Struct,result);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS301Struct,result);
        break;
    case AMSCMD::ANS302:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS302Struct,result);
        break;
    case AMSCMD::ANS310:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS310Struct,result);
        break;
    case AMSCMD::ANS311:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS311Struct,result);
        break;
    case AMSCMD::ANS320:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS320Struct,result);
        break;
    case AMSCMD::ANS321:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS321Struct,result);
        break;
    case AMSCMD::ANS322:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS322Struct,result);
        break;
    case AMSCMD::ANS323:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS323Struct,result);
        break;
    case AMSCMD::ANS330:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS330Struct,result);
        break;
    case AMSCMD::ANS340:
        ConvertToProtocolString((std::string *)(void *)&amsBulletin.ANS340Struct,result);
        break;
    default:
        break;
    #pragma endregion
    }
    //  std::cout<<result<<std::endl;
    return result;
}

#pragma region  not used
// Calaculate hash code.
// Tdd.
// typedef std::uint64_t hash_t;
// constexpr hash_t prime = 0x100000001B3ull;  
// constexpr hash_t basis = 0xCBF29CE484222325ull;  
// hash_t hash_(char const* str)  
// {  
//     hash_t ret{basis};  
   
//     while(*str){  
//         ret ^= *str;  
//         ret *= prime;  
//         str++;  
//     }  
//     return ret;  
// }  

// constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)  
// {  
//     return *str ? hash_compile_time(str+1, (*str ^ last_value) * prime) : last_value;  
// }
#pragma endregion

// 
std::vector<std::string> GtcsAMSProtocol::SplitString(const std::string & str,const std::string& pattern)
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
    	left = right + pattern.size();   //右邊界右移分割依據的長度，作為新的左邊界 
    	right = str.find(pattern, left);   //從left這個位置開始find 
  	}  	
  	//退出迴圈時，左邊界不是最後一個元素 
  	if(left != str.length())
  	{
  		result.push_back(str.substr(left));
  	}
  	return result; 
}
//
std::vector<std::string> GtcsAMSProtocol::GetAMSSpliteArray(const std::string & str)
{
    std::vector<std::string> splite_1,splite_2,splite_3, result;
    splite_1 = SplitString(str,"{");        // 分第一包
    splite_2 = SplitString(splite_1[0],"}");// 分第二包
    result = SplitString(splite_2[0],",");  // 分結果
    return result;
}
// Get CMD enum number. 
int GtcsAMSProtocol::GetAMSCmdNum(std::string amscmd)
{
    int result = -1;
    if (amscmd == "DATA300"){
        result = AMSCMD::DATA300;
    }
    else if (amscmd =="CMD300")
    {
        result = AMSCMD::CMD300;
    }
    else if(amscmd == "CMD301")
    {
        result = AMSCMD::CMD301;
    }
    else if(amscmd == "CMD302")
    {
        result = AMSCMD::CMD302;
    }
    else
    {
        result = -1;
    }   

    return result;    
}
// 
int GtcsAMSProtocol::UpdateProtocolStruct(std::string* prt,std::vector<std::string>& ams_array)
{
    std::cout << std::to_string(ams_array.size()) << std::endl;
    std::cout << ams_array[0] << std::endl;
    int index = 0;
    *prt = ams_array[index];
    while(true){
        index += 1;
        prt = (std::string *)(void *)(prt+1);
        if (*prt!="\n\r"){
            *prt = ams_array[index];
        }
        else{
            break;
        }
    }
    return 0;
}
// Set AMS Protocol struct to bulletin.
int GtcsAMSProtocol::SetAMSBulletin(std::string ams_string)
{
    std::vector<std::string> ams_arry = GetAMSSpliteArray(ams_string);
    int amscmd = GetAMSCmdNum(ams_arry[0]);
    // std::string result = ams_arry[0];
    switch (amscmd)
    {
    #pragma region DATA 
    case AMSCMD::DATA300:      
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.DATA300Struct,ams_arry);
        break; 
    case AMSCMD::DATA302:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.DATA302Struct,ams_arry);
        break;     
    #pragma endregion
    #pragma region REQ
    case AMSCMD::REQ300:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ300Struct,ams_arry);
        break;
    case AMSCMD::REQ301:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ301Struct,ams_arry);
        break;
    case AMSCMD::REQ302:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.REQ302Struct,ams_arry);
        break;
    #pragma endregion  
    #pragma region CMD
    case AMSCMD::CMD300:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD300Struct,ams_arry);
        break;
    case AMSCMD::CMD301:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD301Struct,ams_arry);
        break;
    case AMSCMD::CMD302:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD302Struct,ams_arry);
        break;
    case AMSCMD::CMD303:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD303Struct,ams_arry);
        break;
    case AMSCMD::CMD310:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD310Struct,ams_arry);
        break;
    case AMSCMD::CMD311:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD311Struct,ams_arry);
        break;
    case AMSCMD::CMD312:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD312Struct,ams_arry);
        break;
    case AMSCMD::CMD320:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD320Struct,ams_arry);
        break;
    case AMSCMD::CMD321:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD321Struct,ams_arry);
        break;
    case AMSCMD::CMD322:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD322Struct,ams_arry);
        break;
    case AMSCMD::CMD323:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD323Struct,ams_arry);
        break;
    case AMSCMD::CMD330:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD330Struct,ams_arry);
        break;
    case AMSCMD::CMD340:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.CMD340Struct,ams_arry);
        break;
    #pragma endregion
    #pragma region ANS
    case AMSCMD::ANS301:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS301Struct,ams_arry);
        break;
    case AMSCMD::ANS302:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS302Struct,ams_arry);
        break;
    case AMSCMD::ANS310:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS310Struct,ams_arry);
        break;
    case AMSCMD::ANS311:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS311Struct,ams_arry);
        break;
    case AMSCMD::ANS320:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS320Struct,ams_arry);
        break;
    case AMSCMD::ANS321:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS321Struct,ams_arry);
        break;
    case AMSCMD::ANS322:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS322Struct,ams_arry);
        break;
    case AMSCMD::ANS323:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS323Struct,ams_arry);
        break;
    case AMSCMD::ANS330:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS330Struct,ams_arry);
        break;
    case AMSCMD::ANS340:
        UpdateProtocolStruct((std::string *)(void *)&amsBulletin.ANS340Struct,ams_arry);
        break;
    default:
        break;
    #pragma endregion
    }    
    
    return 0;
} 

// main.
int main()
{
    // Test Singleton.
    GtcsAMSProtocol* ams = GtcsAMSProtocol::GetInstance();
    std::string data300 = ams->GetAMSBulletin(AMSCMD::DATA300);
    std::cout<<data300<<std::endl;

    // Assinge new value to struct.
    std::string cmd = "{DATA300,yyyyMMdd HH:mm:ss,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,FUCK CPP,0,0,0,0,0,0,0,0,0}"; 
    ams->SetAMSBulletin(cmd);
    data300 = ams->GetAMSBulletin(AMSCMD::DATA300);
    std::cout<<data300<<std::endl;    
    return 0;
}
