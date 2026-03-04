MEMORY
{
    ROM (rx) : ORIGIN = 0x08000000, LENGTH = 64K

    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    .text :
    {

        KEEP(*(".isr_vector"))
        
        *(.text*)
        . = ALIGN(4);
    } > ROM

    .rodata :
    {
        *(.rodata*)
        . = ALIGN(4);
    } > ROM

    
    _sidata = LOADADDR(.data);

    .data :
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > ROM

    .bss :
    {
        _sbss = .;
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

}
