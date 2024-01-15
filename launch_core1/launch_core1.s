.cpu cortex-m3
.thumb
.syntax unified

.section .text
.align 2
.thumb_func
.globl _entry
_entry:
    push {r0,r1,r6,r7}

    /* Flag that we have run */
    adr r6, count
    ldr r7, [r6]
    add r7, #1
    str r7, [r6]

    /* Launch core1 */
    sev

    /* Unpatch the hook */
    ldr r6, =0x200007c8
    ldr r7, =0xf04f281f
    str r7, [r6]
    pop {r0,r1,r6,r7}

    /* Resume to patched function */
    b 0x200007c8

.align 4
count: .word 0
