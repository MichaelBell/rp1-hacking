#include <stdint.h>

int uart0_putc(int c) {
    // spin while FIFO is full
    while (*((volatile uint32_t*)(0x40030018)) & (1<<5));

    // send character to UART0
    *((volatile uint32_t*)(0x40030000)) = c & 0xFF;

    return 1;
}

void uart0_puts(const char* s) {
    while (*s) {
        uart0_putc(*s++);
    }
}

void main() {
    // func: SYS_RIO, output and output enable from peripheral
    *((volatile uint32_t*)(0x400d008c)) = 0x85;

    // pad control
    *((volatile uint32_t*)(0x400f0048)) = 0x56;

    // output enable = 1
    *((volatile uint32_t*)(0x400e2004)) = (1 << 17);

    // toggle output
    *((volatile uint32_t*)(0x400e1000)) = (1 << 17);

    const char* hello = "Hello, from RP1!\r\n";
    for (int i = 0; i < 20; ++i) {
        uart0_puts(hello);
    }
}
