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

    /* Resume to patched function */
    b 0x200007c8
