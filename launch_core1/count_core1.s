.cpu cortex-m3
.thumb
.syntax unified

.section .text
.align 2
.thumb_func
.globl _entry
_entry:
    adr r6, count
    ldr r7, [r6]
    add r7, #1
    str r7, [r6]
    b _entry

.align 4
count: .word 0
