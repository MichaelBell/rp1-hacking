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

    /* Clear bss */
    ldr     r4, =__bss_start__
    ldr     r5, =__bss_end__
    cmp     r4, r5
    bge     1f
    mov     r6, #0
    mov     r7, r6

_bss_loop:
    stmia   r4!, {r6,r7}
    cmp     r4, r5
    blt     _bss_loop
1:

    /* Call main */
    bl main
    pop {r4,r5,r6,r7,lr}

/* Clear interrupt implementation */
    cmp r0, #0x1f
    mov r3, #1
    mov.w r2, #0xe000e000
    bls 1f
    subs r0, #0x20
    add r3, #4
1:  lsls r3, r0
    str.w r3,[r2,#0x180]
    bx lr

