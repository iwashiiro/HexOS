; IRQ1 stub — chiama keyboard_handler() in C
global irq1_stub
extern keyboard_handler

irq1_stub:
    pusha
    call keyboard_handler
    popa
    iret
