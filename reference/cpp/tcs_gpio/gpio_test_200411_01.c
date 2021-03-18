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
#define GPIO3_DR                         0x020A4000
#define GPIO5_DR                         0x020AC000
#define GPIO6_DR                         0x020B0000

// IO Bit Index
// OUTPUT
#define GPIO2_OUT_14                         17
#define GPIO2_OUT_13                         18

#define GPIO3_OUT_01                         0
#define GPIO3_OUT_02                         01
#define GPIO3_OUT_03                         02
#define GPIO3_OUT_04                         03

#define GPIO3_OUT_05                         8
#define GPIO3_OUT_06                         9
#define GPIO3_OUT_07                         10
#define GPIO3_OUT_08                         15

#define GPIO2_OUT_09                         22
#define GPIO2_OUT_10                         21
#define GPIO2_OUT_11                         20
#define GPIO2_OUT_12                         19

// INPUT 
#define GPIO6_IN_01                          6
#define GPIO5_IN_02                          4
#define GPIO5_IN_03                          0
#define GPIO2_IN_04                          27

static volatile uint32_t *gpio2;
static volatile uint32_t *gpio3;
static volatile uint32_t *gpio5;
static volatile uint32_t *gpio6;

int main(int argc, char **argv)
{
  int fd ;
  int i;

  // Obtain handle to physical memory
  if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
    //printf("Unable to open /dev/mem: %s\n", strerror(errno));
    printf("Unable to open /dev/mem: %s\n", "1");
    exit(1);
  }

  // Map the GPIO2 Data Register
  gpio2 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO2_DR);
  gpio3 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO3_DR);
  gpio5 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO5_DR);
  gpio6 = (uint32_t *)mmap(0, (getpagesize() * 250), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO6_DR);

#pragma region mmap
  if (gpio2 == MAP_FAILED){
    //printf("mmap failed: %s\n", strerror(errno));
    printf("mmap failed: %s\n", "2");
    exit(2);
  }

  if (gpio3 == MAP_FAILED){
    //printf("mmap failed: %s\n", strerror(errno));
    printf("mmap failed: %s\n", "3");
    exit(2);
  }

  if (gpio5 == MAP_FAILED){
    //printf("mmap failed: %s\n", strerror(errno));
    printf("mmap failed: %s\n", "5");
    exit(2);
  }

  if (gpio6 == MAP_FAILED){
    //printf("mmap failed: %s\n", strerror(errno));
    printf("mmap failed: %s\n", "6");
    exit(2);
  }
#pragma endregion
  
  // Change GPIO2_IO19 (GPIO2 Data Register Bit[19] status every 50 milliseconds
//  while(1){
/*  for (i = 0; i < 100; i++) {
    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_14);
    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_13);

    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_01);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_02);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_03);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_04);

    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_05);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_06);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_07);
    *gpio3 = *gpio3 ^ (1 << GPIO3_OUT_08);

    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_09);
    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_10);
    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_11);
    *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_12);
    

//    if(*gpio5 ^ (1<<GPIO6_IN_01)){
//      *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_14);
  
    // Dump the contents of gpio register (32bit)
    //printf("GPIO2 Data Register: 0x%08X\n" , *gpio);
    printf("GPIO2 Data Register: 0x%s\n" , "s");

    usleep(50000);
  }*/

  *gpio2 = *gpio2 ^ (1 << GPIO2_OUT_14);
  usleep(500000);
  printf("gpio2 = %d \n",(uint32_t)*gpio2);
  //*gpio2 = *gpio2 ^ (1 << GPIO2_OUT_14);
  *gpio2 = *gpio2 & (0 << GPIO2_OUT_14);
  usleep(500000);
  printf("gpio2 = %d \n",(uint32_t)*gpio2);

  while (1){
    if (!(*gpio6 & (1 << GPIO6_IN_01))){
      printf("gpio6 = %d \n",(uint32_t)*gpio6);

      break;
    }

    
    if (!(*gpio5 & (1 << GPIO5_IN_02))){
      break;
    }
    usleep(50000);
  }
  {
    /* code */
  }
  

  //*gpio2 = *gpio2 ^ (0 << GPIO2_OUT_14);
  //usleep(500000);

#pragma region munmap
  // Unmap 
  if((munmap((void *)gpio2, (getpagesize() * 250))) < 0){
    //printf("munmap failed: %s\n", strerror(errno));
    printf("munmap failed: %s\n", "2");
    exit(3);
  }

  // Unmap 
  if((munmap((void *)gpio3, (getpagesize() * 250))) < 0){
    //printf("munmap failed: %s\n", strerror(errno));
    printf("munmap failed: %s\n", "3");
    exit(3);
  }

  // Unmap 
  if((munmap((void *)gpio5, (getpagesize() * 250))) < 0){
    //printf("munmap failed: %s\n", strerror(errno));
    printf("munmap failed: %s\n", "5");
    exit(3);
  }

  // Unmap 
  if((munmap((void *)gpio6, (getpagesize() * 250))) < 0){
    //printf("munmap failed: %s\n", strerror(errno));
    printf("munmap failed: %s\n", "6");
    exit(3);
  }
#pragma endregion

  close(fd);
  
  return 0;
}
