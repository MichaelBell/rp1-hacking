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

    /* Set the SP for core1 from 0x20008000 in the watchdog */
    ldr r0, =0x20008000
    ldr r7, =0x40154014
    ldr r1, [r0]
    str r1, [r7, #8]

    /* Load the adress to boot core1 at from 0x20008004 */
    ldr r1, [r0, #4]

    /* XOR with magic number and set to watchdog */
    ldr r6, =0x4FF83F2D
    eor r1, r1, r6
    str r1, [r7]

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
