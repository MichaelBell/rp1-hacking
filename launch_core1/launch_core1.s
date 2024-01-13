.cpu cortex-m3
.thumb
.syntax unified

.section .text
.align 2
.thumb_func
.globl _entry
_entry:
    push {r6,r7}
    adr r6, count
    ldr r7, [r6]
    cmp r7, #0
    bne 1f
    push {r0,r1}
    ldr r0, =0x40154014
    ldr r1, =(0x4FF83F2D ^ 0x20008001)
    str r1, [r0]
    ldr r1, =0x100030d0
    str r1, [r0,#8]
    sev
    pop {r0,r1}
1:
    add r7, #1
    str r7, [r6]
    pop {r6,r7}

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
    

.align 4
count: .word 0
