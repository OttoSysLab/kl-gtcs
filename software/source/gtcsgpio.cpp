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
#include "gtcsgpio.h"

/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GPIO::GPIO()
 *
 *  @brief   Constructor. 
 *
 *  @param   int num
 *
 *  @return  std::array<bool,8>
 *
 *  @note    none
 *
 *******************************************************************************************/
GPIO::GPIO()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/02/04
 *
 *  @fn      BitArray::To8BiteArray(int num)
 *
 *  @brief   Distructor. 
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GPIO::~GPIO()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsGPIO::GtcsGPIO()
 *
 *  @brief   Constructor
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsGPIO::GtcsGPIO()
{
	InitialGtcsGPIO();
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsGPIO::~GtcsGPIO()
 *
 *  @brief   Distructor. 
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsGPIO::~GtcsGPIO()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsGPIO::InitialGtcsGPIO()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsGPIO::InitialGtcsGPIO()
{
	// Obtain handle to physical memory
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        //printf("Unable to open /dev/mem: %s\n", strerror(errno));
        printf("Unable to open /dev/mem: %s\n", "1");
        // exit(1);
		return false;
    }
	// Map the GPIO2 Data Register
    gpio2 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (int)GTCS_GPIO_REGEIST::GPIO2_DR);
    gpio3 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (int)GTCS_GPIO_REGEIST::GPIO3_DR);
    gpio5 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (int)GTCS_GPIO_REGEIST::GPIO5_DR);
    gpio6 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, (int)GTCS_GPIO_REGEIST::GPIO6_DR);
	#pragma region mmap
    if (gpio2 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "2");
        // exit(2);
		return false;
    }

    if (gpio3 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "3");
        // exit(2);
		return false;
    }

    if (gpio5 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "5");
        // exit(2);
		return false;
    }

    if (gpio6 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "6");
        // exit(2);
		return false;
    }
	#pragma endregion
	return true;
}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/05
 *
 *  @fn      GtcsGPIO::GtcsGPIOPorcessHandler()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
void GtcsGPIO::GtcsGPIOPorcessHandler()
{
	while (true)
	{
		std::cout << "Date time now = " << DateTime::GetCurrentSystemDateTime()<< std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); 							// Thread sleep 1s.
	}
}