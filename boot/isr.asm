.code32

.extern isr_handler
.extern keyboard_handler

/* Macro for exceptions that do NOT push an error code automatically:
   we push a 0 as a placeholder so the stack layout stays identical
   in every case (see struct regs in idt.h) */
.macro ISR_NOERR num
.global isr\num
isr\num:
    push $0
    push $\num
    jmp isr_common_stub
.endm

/* Macro for exceptions where the CPU already pushes an error code
   (8, 10, 11, 12, 13, 14, 17) */
.macro ISR_ERR num
.global isr\num
isr\num:
    push $\num
    jmp isr_common_stub
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

isr_common_stub:
    pusha              # save edi,esi,ebp,esp,ebx,edx,ecx,eax
    push %esp          # pass a pointer to struct regs as the argument
    call isr_handler
    add  $4, %esp       # remove the pushed argument
    popa
    add  $8, %esp       # remove int_no + err_code pushed by the stub
    iret

/* IRQ1 (keyboard), calls keyboard_handler() */
.global irq1_stub
irq1_stub:
    pusha
    call keyboard_handler
    popa
    iret
