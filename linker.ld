ENTRY(start)

SECTIONS
{
    /* Il kernel viene caricato a 1MB */
    . = 1M;

    /* Uniamo la sezione multiboot e il codice eseguibile */
    .text BLOCK(4K) : ALIGN(4K)
    {
        /* Il comando KEEP impedisce al linker di eliminare questa sezione
           anche se pensa che non sia usata. La mettiamo in cima a tutto. */
        KEEP(*(.multiboot))
        *(.text*)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata*)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data*)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss*)
    }
}
