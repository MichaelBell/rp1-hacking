#include "uart.h"

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_SNPRINTF_SAFE_TRIM_STRING_ON_OVERFLOW 1

// Compile nanoprintf in this translation unit.
#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

static void uart1_putc(int c, void* p) {
    // spin while FIFO is full
    while (*((volatile uint32_t*)(0x40034018)) & (1<<5));

    // send character to UART0
    *((volatile uint32_t*)(0x40034000)) = c & 0xFF;
}

int putc(int c) {
    uart1_putc(c, NULL);
    return 1;
}

int printf(const char *fmt, ...) {
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vpprintf(&uart1_putc, NULL, fmt, val);
    va_end(val);
    return rv;
}
