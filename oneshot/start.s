.cpu cortex-m3
.thumb
.syntax unified

.section .boot,"ax"
.align 2
.thumb_func
.globl _entry
_entry:
    /* Unpatch the hook */
    push {r4,r5,r6,r7,lr}
    ldr r6, =0x200007c8
    ldr r7, =0xf04f281f
    str r7, [r6]

    /* Setup UART 0 */
	ldr r2, =0x40030000   // UART0 SET BASE
	movs r1, #0x1b
	str r1, [r2, #0x24]   // Set divisor to 115200 baud
	movs r1, #0x8
	str r1, [r2, #0x28]
	movs r1, #0x70
	str r1, [r2, #0x2c]   // UART LCR_H. Enable FIFOs, 8 bits, 1 stop, no parity
	mov r1, #0x101
	str r1, [r2, #0x30]   // UART CR: Enable, transmit only

	mov r1, #0x84           // Function 4 (UART0)
    ldr r2, =0x400d0000
	str r1, [r2, #0x74]   // GPIO 14

	mov r1, #0x16           // Pads
    ldr r2, =0x400f0000
	str r1, [r2, #0x3c]   // GPIO 14

    /* Clear bss */
    ldr     r4, =__bss_start__
    ldr     r5, =__bss_end__
    cmp     r4, r5
    bge     1f
    movs    r6, #0
    mov     r7, r6

_bss_loop:
    stmia   r4!, {r6,r7}
    cmp     r4, r5
    blt     _bss_loop
1:

    /* Call main */
    bl main
    pop {r4,r5,r6,r7,lr}

    /* Resume to patched function */
    b 0x200007c8
