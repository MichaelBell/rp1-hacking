#include <assert.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

uint64_t mmio;

static void setup_io() {
  int fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    printf("Unable to open /dev/mem. Run as root using sudo?\n");
    exit(-1);
  }

  void *gpio_map = mmap(NULL, 0x10000000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x1f00000000);

  close(fd);
  if (gpio_map == MAP_FAILED) {
    perror("mmap failed, errno");
    exit(-1);
  }
  mmio = (uint64_t)gpio_map;
}

#define REG32a(addr) ((volatile uint32_t*)(((addr) - 0x40000000) + mmio))
#define REG32b(addr) ((volatile uint32_t*)(((addr) - 0x20000000 + 0x400000) + mmio))

int main(int argc, char**argv) {
  int opt;
  uint32_t load_addr = 0x20008000;
  char *payload = NULL;

  while ((opt = getopt(argc, argv, "a:f:")) != -1) {
    switch (opt) {
    case 'a':
      load_addr = strtol(optarg, NULL, 0);
      break;
    case 'f':
      payload = optarg;
      break;
    }
  }

  assert(payload);

  printf("loading %s to 0x%x and executing on core1\n", payload, load_addr);

  setup_io();
  *REG32a(0x40014000 | 0x2000) = 1 << 31; // set core1 reset
  sleep(1);

  FILE* f = fopen("launch_core1.bin", "rb");
  uint32_t a = 0x20007000;
  while (!feof(f)) {
    uint32_t val;
    fread(&val, sizeof(val), 1, f);
    *REG32b(a) = val;
    a += 4;
  }
  fclose(f);

  f = fopen(payload, "rb");
  a = load_addr;
  while (!feof(f)) {
    uint32_t val;
    fread(&val, sizeof(val), 1, f);
    *REG32b(a) = val;
    a += 4;
  }
  fclose(f);

  uint32_t initial_sp = *REG32b(0x20008000);
  uint32_t initial_pc = *REG32b(0x20008004);

  *REG32a(0x4015401c) = initial_sp;
  *REG32a(0x40154014) = initial_pc ^ 0x4FF83F2D;
  *REG32a(0x4015400c) = 0xb007c0de;

  *REG32a(0x40014000 | 0x3000) = 1 << 31; // clear core1 reset
  sleep(1);

  printf("old value 0x%x\n", *REG32b(0x200007c8));
  if (*REG32b(0x200007c8) == 0xf04f281f) {
    *REG32b(0x200007c8) = 0xbc1af006;
  }
}
