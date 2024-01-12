.cpu cortex-m3
.thumb
.syntax unified

.section .text
.align 2
.thumb_func
.globl _entry
_entry:
    /* Func SYS_RIO, output enable */
    ldr r0, =0x400d008c
    mov r1, #0x85
    str r1, [r0]

    /* pad control */
    ldr r0, =0x400f0048
    mov r1, #0x56
    str r1, [r0]
    
    /* output enable */
    ldr r0, =0x400e2000
    mov r1, #0x20000
    str r1, [r0, #4]
    
    /* output high */
    str r1, [r0]

    sub r0, 0x1000
    mov r5, #0x500000
    adr r6, count
    mov r7, #0
1:
    add r7, #1
    str r7, [r6]
    cmp r7, r5
    bls 1b
    str r1, [r0]
    mov r7, #0
    b 1b

.align 4
count: .word 0
