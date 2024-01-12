#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



//#define MMIOADDR 0x601100000
//#define MMIOADDR 0x1f000f0000
#define   MMIOADDR 0x1f00400000
#define   MMIOLEN  0x10000
#define   BASE 0x8000
#define   LEN 0x100

volatile unsigned int *mmio;
#define REG32(m,x) ((volatile uint64_t *)((uint64_t)(m)+(uint64_t)(x)))

static void setup_io() {
  int fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    printf("Unable to open /dev/mem. Run as root using sudo?\n");
    exit(-1);
  }

  void *gpio_map =
      mmap(NULL,                   // Any adddress in our space will do
           MMIOLEN,      // Map length
           PROT_READ | PROT_WRITE, // Enable reading & writting to mapped memory
           MAP_SHARED,             // Shared with other processes
           fd,                     // File to map
           MMIOADDR       // Offset to GPIO peripheral
      );

  close(fd);

  if (gpio_map == MAP_FAILED) {
    perror("mmap failed, errno");
    exit(-1);
  }

  mmio = ((volatile unsigned *)gpio_map) + BASE/4;
}



volatile unsigned int test;
int main(void) {


setup_io();

printf("Read mmio address: %10lx \n", MMIOADDR + BASE);

unsigned int a=0;
volatile unsigned int b=0;
unsigned int i = 0;

for(a=0; a<LEN/4; a++) {
   if (!(a & 0x7)) printf("\n%04x: ", a*4);
   b= *(mmio + a);
   //*(mmio + a) = 0;
   printf("%08x ", b);
}
printf("\n");

}

