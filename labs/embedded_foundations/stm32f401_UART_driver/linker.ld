MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    .text :
    {
        KEEP(*(".isr_vector"))
        *(.text*)
        . = ALIGN(8);
    } > FLASH

    .rodata :
    {
        *(.rodata*)
        . = ALIGN(8);
    } > FLASH


    _sidata = LOADADDR(.data);

    .data : 
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(8);
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*);
        . = ALIGN(8);
        _ebss = .;
    } > RAM
}
