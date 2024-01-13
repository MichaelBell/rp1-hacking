#include <stdint.h>

void main() {
    // func: SYS_RIO, output and output enable from peripheral
    *((volatile uint32_t*)(0x400d008c)) = 0x85;

    // pad control
    *((volatile uint32_t*)(0x400f0048)) = 0x56;

    // output enable = 1
    *((volatile uint32_t*)(0x400e2004)) = (1 << 17);

    // toggle output
    *((volatile uint32_t*)(0x400e1000)) = (1 << 17);    
}
