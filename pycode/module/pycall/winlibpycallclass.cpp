#ifndef GTCS_TLG_DECODER
#define GTCS_TLG_DECODER
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
#include <iostream>  
#include <string>
#include <array>
#include <vector>
#include <ctime>

//#include <sqlite3.h>

#pragma region CrcChecker
/* */
class CrcChecker{
private:
    /* data */
    uint32_t m_pu32CRC32[256] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};
    
    uint8_t m_pu8Revers[256] = {
        //0     1     2     3     4     5     6     7    8      9     A     B     C     D     E     F
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, // 0
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, // 1
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, // 2
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, // 3
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, // 4
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, // 5
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, // 6
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, // 7
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, // 8
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, // 9 
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, // A
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, // B
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, // C
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, // D
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, // E
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF, // F
    };
    uint32_t u32CRC32(uint32_t u32CRC, uint8_t *pData);
    uint32_t u32Bitrevers(uint32_t dwValue);
    std::array<uint8_t,4> u32CRC32_function(uint8_t* pData, uint32_t u32DWORDCount);
public:
    CrcChecker(/* args */){
        //std::cout<<"Initial CrcCheck!"<<std::endl;
    };
    ~CrcChecker(){
        //std::cout<<"Dispose CrcCheck!"<<std::endl;
    };
    int checkCrc32(uint8_t* pData);
    uint8_t crc32_value[4];
    void calculateCrc32(uint8_t* pData);
};

uint32_t CrcChecker::u32CRC32(uint32_t u32CRC, uint8_t *pData){
    uint32_t CRC32 = u32CRC;
    int i ;
	for ( i = 0; i < 4; i++)
	{
		CRC32 = (CRC32 >> 8) ^ m_pu32CRC32[(CRC32 ^ pData[i]) & 0xff];
	}
	return CRC32;
}

uint32_t CrcChecker::u32Bitrevers(uint32_t dwValue)
{
//		uint8_t* pu8;
	uint8_t pu8[4] ;
//		pu8 = new byte[4];
	uint32_t dwResult;
	uint32_t u32Buffer;
    
	pu8[0] = dwValue >> 24 ;
	pu8[1] = dwValue >> 16 ;
	pu8[2] = dwValue >> 8 ;
	pu8[3] = dwValue ;


	dwResult = 0;
	dwResult += m_pu8Revers[pu8[3]];
	u32Buffer = 0;
	u32Buffer = m_pu8Revers[pu8[2]];
	u32Buffer <<= 8;
	dwResult += u32Buffer;

	u32Buffer = 0;
	u32Buffer = m_pu8Revers[pu8[1]];
	u32Buffer <<= 16;
	dwResult += u32Buffer;

	u32Buffer = 0;
	u32Buffer = m_pu8Revers[pu8[0]];
	u32Buffer <<= 24;
	dwResult += u32Buffer;

	return dwResult;
}

std::array<uint8_t,4> CrcChecker::u32CRC32_function(uint8_t* pData, uint32_t u32DWORDCount)
{
	uint32_t u32CRC = 0xFFFFFFFF;
	uint32_t u32Data;
    int i ;
	uint8_t pu8[4];
	
	for ( i = 0; i < u32DWORDCount; i++)
	{
	    int n = i * 4;
		u32Data = pData[n];
		u32Data += (uint32_t)pData[n + 1] * 0x100;
		u32Data += (uint32_t)pData[n + 2] * 0x10000;
		u32Data += (uint32_t)pData[n + 3] * 0x1000000;
		u32Data = u32Bitrevers(u32Data);

		pu8[0] = u32Data >> 24 ;
		pu8[1] = u32Data >> 16 ;
		pu8[2] = u32Data >> 8 ;
		pu8[3] = u32Data ;			
		u32CRC = u32CRC32(u32CRC, pu8);
	}

    uint32_t u32_buf = u32Bitrevers(u32CRC);
    std::array<uint8_t,4> reslut;
    reslut[0] = u32_buf>>24;
    reslut[1] = u32_buf>>16;
    reslut[2] = u32_buf>>8;
    reslut[3] = u32_buf;

	return reslut;
}
int CrcChecker::checkCrc32(uint8_t* pData){
    int check_ok = 0;
    std::array<uint8_t,4> check_buf = u32CRC32_function(pData,11);
    
    if(pData[44]==check_buf[0]){
        if (pData[45]==check_buf[1]){
            if (pData[46]==check_buf[2]){
                if(pData[47]==check_buf[3]){
                    check_ok = 1;
                }
            }
        }
    }    
    return check_ok;
}

void CrcChecker::calculateCrc32(uint8_t* pData){
    std::array<uint8_t,4> check_buf = u32CRC32_function(pData,11);

    for(int idx=0;idx<4;idx++){
        crc32_value[idx]=check_buf[idx];
    }
}
#pragma endregion

#pragma region TlgHeader
/* */
class TlgHeader{
private:
    // tlg type.
    int _tlg_type=0;
    // Para type.
    int _Para_addr_2 = 0;
    int _Para_addr_3 = 0;
    int _Para_addr_4 = 0;
    int _Para_addr_5 = 0;
    int _Para_addr_6 = 0;
    bool Para_write_success  = false;
    
    // logging type.
    int _log_addr = 0;
    
    // step type.
    int step_addr = 0;
    int step_write_ok = false;
    // check function.
    int checkTlgType(uint8_t* pTlgh);
    void checkParaAddress(uint8_t* pTlgp);
    void checkLogAddress(uint8_t* pTlgh);
    void checkStepAddress(uint8_t* pTlgh);
public:
    enum Type:int{
        CYCLE_CTRL=1,
        CYCLE_STATUE=2,
        PAPA_WRITE=3,
        PARA_READ=4,
        PARA_RESPONSE=5,
        TMD_CMD_REQUESE=6,
        TMD_CMD_RESPONSE=7,
        TORQUE_REQUEST=10,
        TORQUE_RESPONSE=11,
        PROCESS_DATA_REQUEST=12,
        PROCESS_DATA_RESPONSE=13,
        STEP_DATA_REQUEST=14,
        STEP_DATA_RESPONSE=15,
    };

    TlgHeader();
    ~TlgHeader();
    // Telegram header type.
    int getTlgType();
    void setTlgType(int pTypeNum);
    // Para address.
    int getParaAddress(int aNum);
    void setParaAddress(uint8_t* pTlgp);
    // Logging addraee.
    int getLoggingAddress();
    void setLoggingAddress(uint8_t pLogAddr);
    // Step address.
    int getStepAddress();
    void setStepAddress(uint8_t pStepAddr);
    
    std::array<uint8_t,8> encodeTlgHeader();
    void decodeTlgHeader(uint8_t* pDate);
};
TlgHeader::TlgHeader(){}
TlgHeader::~TlgHeader(){}

// Check tcs500 telegram type. 
int TlgHeader::checkTlgType(uint8_t* pTlgh){
    _tlg_type = (int)pTlgh[0];
    return _tlg_type;
}
//
void TlgHeader::checkParaAddress(uint8_t* pData){
     
    _Para_addr_2 = (int)pData[1];
    _Para_addr_3 = (int)pData[2];
    _Para_addr_4 = (int)pData[3];
    _Para_addr_5 = (int)pData[4];
    _Para_addr_6 = (int)pData[5];
    if ((int)pData[6]==1)
        Para_write_success=true;
}
//
void TlgHeader::checkLogAddress(uint8_t* pData){
    _log_addr = (int)pData[0];
}
//
void TlgHeader::checkStepAddress(uint8_t* pData){
    step_addr = (int)pData[0];
}
//
int TlgHeader::getTlgType(){
    return (uint8_t)_tlg_type;
}
void TlgHeader::setTlgType(int pTypeNum){
    _tlg_type=pTypeNum;
}
//
int TlgHeader::getParaAddress(int aNum){
    int _result = 0;
    switch(aNum){
        case 2: _result=_Para_addr_2; break;
        case 3: _result=_Para_addr_3; break;
        case 4: _result=_Para_addr_4; break;
        case 5: _result=_Para_addr_5; break;
        case 6: _result=_Para_addr_6; break;
    }
    return _result;
}
void TlgHeader::setParaAddress(uint8_t* pData){
    _Para_addr_2=pData[1];
    _Para_addr_3=pData[2];
    _Para_addr_4=pData[3];
    _Para_addr_5=pData[4];
    _Para_addr_6=pData[5];
}
//
int TlgHeader::getLoggingAddress(){
    return _log_addr;
}
void TlgHeader::setLoggingAddress(uint8_t pLogAddr){
    _log_addr=pLogAddr;
}
//
int TlgHeader::getStepAddress(void){
    return step_addr;
}
void TlgHeader::setStepAddress(uint8_t pStepAddr){
    step_addr=pStepAddr;
}
// 
void TlgHeader::decodeTlgHeader(uint8_t* pData){
    int Htype = checkTlgType(pData);
    switch(Htype){
        case Type::CYCLE_CTRL:              // 1
            break;
        case Type::CYCLE_STATUE:            // 2
            break;
        case Type::PAPA_WRITE:              // 3
            checkParaAddress(pData);
            break;
        case Type::PARA_READ:               // 4
            checkParaAddress(pData);
            break;
        case Type::PARA_RESPONSE:           // 5
            checkParaAddress(pData);
            break;
        case Type::TMD_CMD_REQUESE:         // 6
            break;
        case Type::TMD_CMD_RESPONSE:        // 7
            break;
        case Type::TORQUE_REQUEST:          // 10
            checkParaAddress(pData);
            break;
        case Type::TORQUE_RESPONSE:         // 11
            checkParaAddress(pData);
            break;
        case Type::PROCESS_DATA_REQUEST:    // 12
            break;
        case Type::PROCESS_DATA_RESPONSE:   // 13
            break;
        case Type::STEP_DATA_REQUEST:       // 14
            checkStepAddress(pData);
            break;
        case Type::STEP_DATA_RESPONSE:      // 15
            checkStepAddress(pData);
            break;
    }
}
#pragma endregion

#pragma region TlgPayload
/* */
class TlgPayload{
private:
#pragma region ID1
    // ID=1
    class IdentificationPara{
    private: 
    public:
        // properity.
        int MAIN_ID = 1;
        enum SUB_ID:int {
            MCB_SW_VERSION = 1,
            MCB_CORE_ID,
            MCB_SER_NUM,
            DMS_SW_VERSION,
            DMS_CORE_ID,
            DMS_SER_NUM,
        };
        // MCB Software Version Motorcontroller.
        std::string MCB_software_version="";
        // MCB Core ID Motorcontroller.
        std::string MCB_core_ID="";        
        // MCB Serial Number Motorcontroller.
        std::string MCB_serial_number="";        
        // DMS Software Version Motorcontroller.
        std::string DMS_software_version="";        
        // DMS Core ID Motorcontroller.
        std::string DMS_core_ID="";
        // DMS Serial Number Motorcontroller.
        std::string DMS_serial_number="";              
    };
#pragma endregion
#pragma region ID2
    // ID =2
    class BasicPara{
    private:
    public:
        int MAIN_ID = 2;
        enum SUB_ID : int {
            MIN_TEMPERATURE=1,
            MAC_TEMPERATURE,
            MAX_CURRENT,
            MAX_PEAK_CURRENT,
            TORQUE_SENSOR_TYPE,
            MAX_PWM_CYCLE,
            MAX_TORQUE,
            PWM_FRQUENCE,
            MAX_RPM,
            MAX_SLOPE,
            MIN_BUS_VOLTAGE,
            MAX_BUS_VOLTAGE,
            MIN_BUS_VOLTAGE_IDEL_TIME,
            GEAR_BOX_RATION,
            START_INPUT,
            REVERSE_INPUT,
            REVERSE_PWM,
            REVERSR_SLOP,
            REVERSE_MAX_CURRENT,
            REVERSE_MAX_TORQUE,
            ERROR_IDLE_TIME,
            BACKLASH,
            PROPORTIONAL_GAIN,
            INTEGRAL_GAIN,
            NCODER
        };
        // Minimal Temperature.
        uint16_t min_temperature=0;
        // Maximal Temperature.
        uint16_t max_temperature=0;
        // Motor maximal Current.
        uint16_t max_current =0;        
        // Motor maximal Peak Current.
        uint16_t max_peak_current=0;        
        // Torque Sensor Type.
        uint16_t torque_sensor_type=0;
        // Maximal PWM Duty Cycle.
        uint16_t max_pwm_cycle=0;        
        // Max Torque.
        uint16_t max_torque=0;
        // PWM Frequency.
        uint16_t pwm_frequence=0;
        // Max RPM.
        uint16_t max_RPM=0;
        // Max Slope.
        uint16_t max_slope=0;
        // Minimum Bus Voltage.
        uint16_t min_bus_voltage=0;
        // Maximum Bus Voltage.
        uint16_t max_bus_voltage=0;        
        // Minimum Bus Voltage Idle Time.
        uint16_t min_bus_voltage_idle_time=0;                
        // Gear Box Ratio.
        uint16_t gear_box_ratio=0;        
        // Start Input.
        uint16_t start_input=0;
        // Reverse Input.
        uint16_t reverse_input=0;
        // Reverse RPM.
        uint16_t reverse_RPM=0;        
        // Reverse Slope.
        uint16_t reverse_slope=0;
        // Reverse maximum Current.
        uint16_t reverse_max_current=0;        
        // Reverse maximum Torque.
        uint16_t reverse_max_torque = 0;
        // Error idle Time.
        uint16_t error_idle_time = 0;
        // Backlash.
        uint16_t backlash=0;        
        // Proportional Gain.
        uint16_t proportional_gain=0;
        // Integral Gain.
        uint16_t integral_gain=0;
        // Encoder.
        uint16_t encoder=0;
    };
#pragma endregion
#pragma region ID3
    // ID =3 ,3XXX
    class StepPara{
    private:
    public:
        // MAIN ID
        int  MIAN_ID = 3;
        enum SUB_ID : int {
            STEP_NAME=1,
            STEP_RPM,
            STEP_SLOPE,
            MAX_CURRENT,
            MAX_TORQUE,
            MAX_REVOLUTION,
            EXECUTION_TIME,
            ANGLE,
            STEP_ANGLE_WINDOW,
            STEP_TORQUE_WINDOW,
            MIN_DUTY_CYCLE,
            STEP_FLAGS
        };

        // Screwing step name.
        std::string  step_name="";
        // Screwing step RPM.
        uint16_t step_rpm=0;
        // Screwing step slope.
        uint16_t step_slope=0;
        // Screwing step maximum current.
        uint16_t max_current=0;
        // Screwing step maximum torque.
        uint16_t max_torque=0;
        // Screwing step maximum revolutions.
        uint16_t max_revolutions=0;
        // Screwing step execution time.
        uint16_t execution_time=0;
        // Screwing step angle.
        uint16_t angle=0;
        // Screwing step angle window.
        uint16_t step_angle_window=0;
        // Screwing step torque window.
        uint16_t step_torque_window=0;
        // Minimum Duty Cycle.
        uint16_t min_duty_cycle=0;
        // Screwing step flags
        uint16_t step_flags=0;
    };
#pragma endregion
#pragma region ID4
    // ID4
    class ProcessPara{
    private:
    public:
        int MAIN_ID=4;
        enum SUB_ID:int{
            PROCESS_NAME=1,
            PROCESS_PROPORTIONAL_GAIN,
            PROCESS_MAX_TIME,
            PROCESS_MIN_TIME,
            REVERSE_FUNCTION,
            PROCESS_NUMBER_OF_STEPS,
            PROCESS_STEP_NUMBERS,
            PROCESS_STEP_ID,
        };
        // Screwing Process Name.
        std::string process_name="";
        // Screwing process Proportional Gain
        uint16_t process_proportional_gain=0;
        // Screwing Process Integral Gain
        uint16_t process_integral_gain=0;
        // Screwing Process Max Time
        uint16_t process_max_time=0;
        // Screwing Process Min Time.
        uint16_t process_min_time=0;
        // Screwing Process Reverse Function.
        uint16_t process_reverse_function=0;
        // Screwing Program Number of Processess. 
        uint16_t process_step_numbers=0;
        // Screwing Process Step Numbers.
        uint16_t process_step_ID=0;
    };
#pragma endregion
#pragma region ID5
    class ProgramPara{
    private:
    public:
        int MAIN_ID=5;
        enum SUB_ID:int {
            PROGRAM_NAME=1,
            PROGRAM_RESET_FUNCTION,
            PROGRAM_REVERSE_FUNCTION,
            PROGRAM_REVERSE_TIME,
            PROGRAM_ERROR_RESPONSE,
            PROGRAM_PROCESS_NUMBER,
            PROGRAM_PROCESS_ID,
        };
        // Screwing Program Name.
        std::string  program_name="";
        // Screwing Program Reset Function.
        uint16_t program_reset_function=0;
        // Screwing Program Reverse Drive Function.
        uint16_t program_reverse_function=0;
        // Screwing Program Reverse Drive Time.
        uint16_t program_reverse_time=0;
        // Screwing Program Error Response.
        uint16_t program_error_response=0;
        // Screwing Program Number of Processes.
        uint16_t program_process_number=0;        
        // Screwing Program Processes.
        uint16_t program_process_ID=0;
    };
#pragma endregion
#pragma region ID6
    class StatistcPara{
    private:
    public:
        int MIAN_ID =6;
        enum SUB_ID:int{
            CNT_PROCESS_START=1,
            CNT_PROCESS_TIGHTENING,
            CNT_OVER_CURRENT,
            CNT_OVER_TEMPERATURE,
            CNT_REVERSE,
            CNT_LAST_TEN_ERRORS
        }; 
        // Startcounter.
        uint32_t CNT_process_start=0;
        // Tightening- Counter.
        uint32_t CNT_process_Tightening=0;
        // Overcurrent- Counter.
        uint32_t CNT_over_current=0;
        // Overtemperature- Counter.
        uint32_t CNT_over_temperature=0;
        // Reverse- Counter.
        uint32_t CNT_reverse=0;
        // Last 10 Errors.
        uint32_t CNT_Last_ten_erroes=0;
    };
#pragma endregion
#pragma region ID7
    class ContrlPara{
    private:
    public:
        int MAIN_ID =7;
        enum SUB_ID:int{
            CONTROL_MODE,
            CONTROL_PROCESS,
            CONTROL_WRITE_PARA,
            CONTROL_WIRTE_STEP,
            CONTROL_WRITE_PROCESS,
            CONTROL_WRITE_PROGRAM
        };
        // Control Mode.
        uint16_t control_mode=0;
        // Control Process.
        uint16_t control_process=0;
        // Control write Para to Flash.
        uint16_t control_write_para=0;
        // Control write Step to Flash.
        uint16_t control_write_step=0;
        // Control write Process to Flash.
        uint16_t control_write_process=0;
        // Control write Program to Flash.
        uint16_t control_write_program=0;
    };
#pragma endregion
#pragma region ID8
    class ManualPara{
    private:
    public:
        int MAIN_ID = 8;
        enum SUB_ID : int {
            MANUAL_RPM=1,
            MANUAL_SLOPE,
            MANUAL_MAX_CURRENT,
            MAX_TORQUE,
        };
        // Manual RPM.
        uint16_t manual_RPM=0;
        // Manual Slope.
        uint16_t manual_slope=0;
        // Manual maximum Current.
        uint16_t manual_max_currnet=0;
        // Manual max. Torque.
        uint16_t max_torque=0;
    };
#pragma endregion
#pragma region ID9 
    class StatusPara{
    private:
    public:
        int MAIN_ID = 9;
        enum SUB_ID : int {
            ACTUAL_ERROR_FLG,
            ACTUAL_PROCESS_ID,
            ACTUAL_PROCESS_NUMBER,
            ACTUAL_STEP_ID,
            ACTUAL_STEP_NUMBER,
            ACTUAL_CURRENT,
            ACTUAL_RPM,
            ACTUAL_TORQUE,
            ACTUAL_BUS_VLOTAGE,
            ACTUAL_MCB_TEMPERATURE,
            ACTUAL_MRT_TEMPERATURE,
            ACTUAL_MAX_CURRENT,
            ACTUAL_MAX_TORQUE,
            ACTUAL_ANGLE,
            ACTUAL_REVOLUTIONS,
        };
        // Actual Error.
        uint32_t actual_error_flg=0;
        // Actual Process ID.
        uint16_t actual_process_ID=0;
        // Actual Process Nr.
        uint16_t actual_process_number=0;
        // Actual Program Step ID.
        uint16_t actual_step_ID=0;
        // Actual Program Step Number.
        uint16_t actual_step_number=0;        
        // Actual Motor Current.
        uint16_t actual_current=0;
        // Actual Motor RPM.
        uint16_t actual_RPM=0;        
        // Actual Torque.
        uint16_t actual_torque=0;        
        // Actual Bus Voltage.
        uint16_t actual_bus_voltage=0;
        // Actual Motor Controller Temperature.
        uint16_t actual_MCB_temperature=0;
        // Actual Motor Temperature.
        uint16_t actual_MRT_temperature=0;
        // Actual max current.
        uint16_t actual_max_current=0;
        // Actual max torque.
        uint16_t actual_max_torque=0;
        // Actual angle.
        uint16_t actual_angle=0;
        // Actual revolutions.
        uint16_t actual_revolutions=0;
    };
#pragma endregion

#pragma region Control Mode
    class ControlFlags{
    private:
    public:
    };
#pragma endregion

#pragma region StatusFlages
    class StatusFlags{
    private:
        uint16_t REVERVED_0=0x0001;
        uint16_t REVERVED_1=0x0002;
        uint16_t REVERVED_2=0x0004;
        uint16_t REVERVED_3=0x0008;
        uint16_t REVERVED_4=0x0010;
        uint16_t REVERVED_5=0x0020;
        uint16_t REVERVED_6=0x0040;
        uint16_t TORQUE_MCU_PRSESENT=0x0080;
        uint16_t REVERVED_8=0x0100;
        uint16_t TORQUE_MCU_CYCLIC_OFF=0x0200;
        uint16_t CYCLIC_TIMEOUT_ENABLE=0x0400;
        uint16_t ERROR_OCCURED=0x0800;
        uint16_t PROGRAM_FINISHED=0x1000;
        uint16_t PROCESS_FIHISHED=0x2000;
        uint16_t SCREWDRIVER_REVERSE=0x4000;
        uint16_t SCREWDRIVER_START=0x8000;

        uint16_t check_array[16]={ REVERVED_0,
                                    REVERVED_1,
                                    REVERVED_2,
                                    REVERVED_3,
                                    REVERVED_4,
                                    REVERVED_5,
                                    REVERVED_6,
                                    TORQUE_MCU_PRSESENT,
                                    REVERVED_8,
                                    TORQUE_MCU_CYCLIC_OFF,
                                    CYCLIC_TIMEOUT_ENABLE,
                                    ERROR_OCCURED,
                                    PROGRAM_FINISHED,
                                    PROCESS_FIHISHED,
                                    SCREWDRIVER_REVERSE,
                                    SCREWDRIVER_START,
                                }; 

        uint8_t status_flg_array[16];        

    public:
        void decodeStatusFlags(uint16_t pStatus){
            for(int idx=0;idx<16;idx++){
                if(pStatus&check_array[idx])
                    status_flg_array[idx] = 1;  
                else 
                    status_flg_array[idx] = 0;
            }
        }

        uint16_t encodeStatusFlags(uint8_t* pData){
            uint16_t result=0;
            for(int idx =0;idx<16;idx++){
                if(pData[idx]==1)
                    result += check_array[idx];
                bool trace = true;
            }
            return result;
        }

        uint8_t* getStatusFlagsArray(){
            return status_flg_array;
        } 
    }; 
#pragma endregion

#pragma region 
#pragma endregion
public:
    TlgPayload();
    ~TlgPayload();
    IdentificationPara identificationPara; // ID1
    BasicPara basicPara;                   // ID2
    StepPara stepPara;                     // ID3
    ProcessPara processPara;               // ID4
    ProgramPara programPara;               // ID5
    StatistcPara statistcPara;             // ID6
    ContrlPara contrlPara;                 // ID7
    ManualPara manualPara;                 // ID8
    StatusPara statusPara;                 // ID9

    StatusFlags statusFlags;
    void decodePayload(uint8_t* pdata);

};

TlgPayload::TlgPayload(){
    //std::cout<<"Initial TlgPayload!"<<std::endl;
}
TlgPayload::~TlgPayload(){
    //std::cout<<"Dispose TlgPayload!"<<std::endl;
}

void TlgPayload::decodePayload(uint8_t* pData){
    
}

#pragma endregion

#pragma region TlgDecoder
/**/
class TlgDecoder{
private:    
    uint8_t arr_tlg[48];
    uint8_t  arr_tlgh[8];
    uint8_t arr_tlgp[36];
    uint8_t  arr_tlgc[4];

    int encodeTlgArray(uint8_t* pData);
    void decodeTlgArray(uint8_t *ptlg); 
 
public:
    TlgDecoder();
    ~TlgDecoder();
    CrcChecker Crc32Checker;
    TlgHeader TLgHeader;
    TlgPayload TLgPayload;
    
    uint8_t* getTlghArray();
    void setTlghArray(uint8_t pData);

    uint8_t* getTlgpArray();
    void setTlgpArray(uint8_t* pData);

    uint8_t* calculateCrc32Array();
    void setCrc32Array(uint8_t* pData);
    
    void decodeTlg(uint8_t* pData);    
    uint8_t* encodeTlg();    
};

TlgDecoder::TlgDecoder(/* args */){
    //std::cout<<"Initial TlgDecoder!"<<std::endl;
}

TlgDecoder::~TlgDecoder(){
    //std::cout<<"Depose TlgDecoder!"<<std::endl;  
}

//std::array<uint8_t,48> TlgDecoder::encodeTlgArray(){
int TlgDecoder::encodeTlgArray(uint8_t* pData){
    return 0;
}

void TlgDecoder::decodeTlgArray(uint8_t* ptlg){          

    for(int idx=0;idx<48;idx++){
        arr_tlg[idx] =ptlg[idx];
        if(idx<8)
            arr_tlgh[idx] = ptlg[idx];
        else if(8<idx<36)
            arr_tlgp[idx-8]= ptlg[idx];
        else
            arr_tlgc[idx-44]=ptlg[idx];    
    }
}

uint8_t* TlgDecoder::getTlghArray(){ return arr_tlgh;}
uint8_t* TlgDecoder::getTlgpArray(){ return arr_tlgp;}
uint8_t* TlgDecoder::calculateCrc32Array(){ return arr_tlgc;}

void TlgDecoder::decodeTlg(uint8_t* ptlg){
    bool check_crc = Crc32Checker.checkCrc32(ptlg);
    if (check_crc ==true){
        decodeTlgArray(ptlg);
        TLgHeader.decodeTlgHeader(arr_tlgh);
    }    
}
uint8_t* TlgDecoder::encodeTlg(){

    return arr_tlg;
}
#pragma endregion

// main 

void tdd_01(){
    TlgDecoder tlg;
    uint32_t  crc_32bit = 0;
    //for example UART buffer 
    uint8_t CRC32_text[48] = {1,0,0,0,0,0,0,0,0,164,0,0,160,15,1,0,100,0,200,0,116,1,232,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,26,60,73};    

    tlg.decodeTlg(CRC32_text);
    bool traceFlg = true;
}

void tdd_02(){
    TlgDecoder tlg;
    uint8_t array[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    uint16_t result =tlg.TLgPayload.statusFlags.encodeStatusFlags(array);
    std::cout<<"Eocode status flags = "<<result<<std::endl;
}

/*
int main(){
    //tdd_01();
    //tdd_02();
    return 0;
}
*/
extern "C"{
    TlgDecoder tlg;

    void decodeTlg(uint8_t* pData){ 
        tlg.decodeTlg(pData); 
    }
    int getTlgType(){ 
        return tlg.TLgHeader.getTlgType();
    }
    int checkCrc32(uint8_t* pData){
        return tlg.Crc32Checker.checkCrc32(pData);
    } 
    int getTLGHeaderType(){
        return tlg.TLgHeader.getTlgType();
    }
    void calculateCrc32(uint8_t* pData){
        tlg.Crc32Checker.calculateCrc32(pData);
    }
    uint8_t getCrc32Value_0(){
        return tlg.Crc32Checker.crc32_value[0];
    }
    uint8_t getCrc32Value_1(){
        return tlg.Crc32Checker.crc32_value[1];
    }
    uint8_t getCrc32Value_2(){
        return tlg.Crc32Checker.crc32_value[2];
    }
    uint8_t getCrc32Value_3(){
        return tlg.Crc32Checker.crc32_value[3];
    }
    uint8_t* getCrc32Value_all(){
        return tlg.Crc32Checker.crc32_value;
    }
}

#endif // GTCS_TLG_DECODER