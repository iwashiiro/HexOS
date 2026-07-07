# boot/multiboot.asm
.code32

.section .multiboot
.align 4
.long 0x1BADB002    # MAGIC
.long 0x00000003    # FLAGS
.long 0xE4524FFB    # CHECKSUM

.section .bss
.align 16
stack_bottom:
    .skip 16384     # 16 KB stack
stack_top:

.section .text
.global start
.extern kernel_main

start:
    cli
    mov $stack_top, %esp   # Set up the stack before entering C
    call kernel_main

.hang:
    hlt
    jmp .hang
