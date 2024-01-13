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

  mmio = ((volatile unsigned *)gpio_map);
}



volatile unsigned int test;
int main(void) {


setup_io();

unsigned int a=0;
volatile unsigned int b=0;
unsigned int i = 0;

a = 0x7000/4;
FILE* f = fopen("launch_core1.bin", "rb");
while (!feof(f)) {
  unsigned int val;
  fread(&val, sizeof(val), 1, f);
  *(mmio + a++) = val;
}
fclose(f);

f = fopen("blink_core1.bin", "rb");
a = 0x8000/4;
while (!feof(f)) {
  unsigned int val;
  fread(&val, sizeof(val), 1, f);
  *(mmio + a++) = val;
}
}

