#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

/////////////////////////////////////////////
// Use GPIO2_IO19 (/dev/OUT12) as an example
/////////////////////////////////////////////

// GPIO Data Register Address
#define GPIO2_DR                         0x020A0000
//#define GPIO3_DR                         0x020A4000
//#define GPIO5_DR                         0x020AC000
//#define GPIO6_DR                         0x020B0000

// IO Bit Index
#define GPIO_IO                          19

static volatile uint32_t *gpio;

int main(int argc, char **argv)
{
  int fd ;
  int i;

  // Obtain handle to physical memory
  if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
    printf("Unable to open /dev/mem: %s\n", strerror(errno));
    exit(1);
  }

  // Map the GPIO2 Data Register
  gpio = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO2_DR);

  if (gpio == MAP_FAILED){
    printf("mmap failed: %s\n", strerror(errno));
    exit(2);
  }

  // Change GPIO2_IO19 (GPIO2 Data Register Bit[19] status every 50 milliseconds
  for (i = 0; i < 100; i++) {
    *gpio = *gpio ^ (1 << GPIO_IO);

    // Dump the contents of gpio register (32bit)
    printf("GPIO2 Data Register: 0x%08X\n" , *gpio);

    usleep(50000);
  }

  // Unmap 
  if((munmap((void *)gpio, (getpagesize() * 250))) < 0){
    printf("munmap failed: %s\n", strerror(errno));
    exit(3);
  }

  close(fd);
  
  return 0;
}
