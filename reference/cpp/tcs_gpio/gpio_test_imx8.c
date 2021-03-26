#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

// /dev/IN1 -> GPIO4_IO28
// /dev/IN2 -> GPIO4_IO29
// /dev/IN3 -> GPIO4_IO30
// /dev/IN4 -> GPIO5_IO5
// /dev/IN5 -> GPIO5_IO6
// /dev/IN6 -> GPIO5_IO8
// /dev/IN7 -> GPIO5_IO7
// /dev/IN8 -> GPIO5_IO9
// /dev/IN9 -> GPIO5_IO10
// /dev/IN10 -> GPIO5_IO12
// /dev/IN11 -> GPIO5_IO11
// /dev/IN12 -> GPIO5_IO13
// /dev/OUT1 -> GPIO3_IO24
// /dev/OUT2 -> GPIO3_IO23
// /dev/OUT3 -> GPIO3_IO22
// /dev/OUT4 -> GPIO3_IO20
// /dev/OUT5 -> GPIO3_IO19
// /dev/OUT6 -> GPIO2_IO11
// /dev/OUT7 -> GPIO2_IO10
// /dev/OUT8 -> GPIO2_IO9
// /dev/OUT9 -> GPIO2_IO8
// /dev/OUT10 -> GPIO2_IO7
// /dev/OUT11 -> GPIO2_IO6
// /dev/OUT12 -> GPIO5_IO3
// /dev/OUT13 -> GPIO3_IO25
// /dev/OUT14 -> GPIO3_IO21

/////////////////////////////////////////////
// Use GPIO5_IO3 (/dev/OUT12) as an example
/////////////////////////////////////////////

// GPIO Data Register Address
//#define GPIO2_DR                         0x30210000
//#define GPIO3_DR                         0x30220000
//#define GPIO4_DR                         0x30230000
#define GPIO5_DR                         0x30240000

// IO Bit Index
#define GPIO_IO                          3

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

  // Map the GPIO5 Data Register
  gpio = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO5_DR);

  if (gpio == MAP_FAILED){
    printf("mmap failed: %s\n", strerror(errno));
    exit(2);
  }

  // Change GPIO5_IO3 (GPIO5 Data Register Bit[3] status every 50 milliseconds
  for (i = 0; i < 100; i++) {
    *gpio = *gpio ^ (1 << GPIO_IO);

    // Dump the contents of gpio register (32bit)
    printf("GPIO5 Data Register: 0x%08X\n" , *gpio);

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
