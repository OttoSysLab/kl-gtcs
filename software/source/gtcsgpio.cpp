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
 *  @date    2021/03/07
 *
 *  @fn      GPIOHandler::GPIOHandler()
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
GPIOHandler::GPIOHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/07
 *
 *  @fn      GPIOHandler::~GPIOHandler()
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
GPIOHandler::~GPIOHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/07
 *
 *  @fn      GtcsGPIOHandler::GtcsGPIOHandler()
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
GtcsGPIOHandler::GtcsGPIOHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/07
 *
 *  @fn      GtcsGPIOHandler::~GtcsGPIOHandler()
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
GtcsGPIOHandler::~GtcsGPIOHandler()
{}
/******************************************************************************************
 *
 *  @author  Otto Chang
 *
 *  @date    2021/03/07
 *
 *  @fn      GtcsGPIOHandler::GtcsGPIOHandlerProcess()
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
void GtcsGPIOHandler::GtcsGPIOHandlerProcess()
{
    // Initials vlaue & object.
    GtcsManager manager;
    GtcsScrewSequenceHandler screwhandler;
    #pragma region initial linux gpio regiest.
    // Initial regiest.
    static volatile uint32_t *gpio2;
    static volatile uint32_t *gpio3;
    static volatile uint32_t *gpio5;
    static volatile uint32_t *gpio6;
    // Initial valuable.
    int fd;
    int i;
    // Obtain handle to physical memory
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0)
    {
        //printf("Unable to open /dev/mem: %s\n", strerror(errno));
        printf("Unable to open /dev/mem: %s\n", "1");
        // exit(1);
		// return false;
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
		// return false;
    }

    if (gpio3 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "3");
        // exit(2);
		// return false;
    }

    if (gpio5 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "5");
        // exit(2);
		// return false;
    }

    if (gpio6 == MAP_FAILED){
        //printf("mmap failed: %s\n", strerror(errno));
        printf("mmap failed: %s\n", "6");
        // exit(2);
		// return false;
    }
	#pragma endregion
    #pragma endregion
    
    // THread loop
	while (true)
	{
        // gpio IN_OUT_1
        // if (!(*gpio6 & (1 << (int)GTCS_GPIO_IN::IN_01))){
        //     *gpio3 = *gpio3 | (1 << (int)GTCS_GPIO_OUT::OUT_01);
        // }
        // else{
        //     *gpio3 = *gpio3 & ~(1 << (int)GTCS_GPIO_OUT::OUT_01);
        // }
        manager.GetGtcsScrewSequenceHandlerStatus(screwhandler);
        if (screwhandler.statusnum==(int)LOCKED_STATUS::RUNNING)
        {
            *gpio3 = *gpio3 | (1 << (int)GTCS_GPIO_OUT::OUT_01);
        }
        else
        {
            *gpio3 = *gpio3 & ~(1 << (int)GTCS_GPIO_OUT::OUT_01);
        }

		std::cout << "Date time now = " << DateTime::GetCurrentSystemDateTime()<< std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); 							// Thread sleep 1s.
	}
    close(fd);
}