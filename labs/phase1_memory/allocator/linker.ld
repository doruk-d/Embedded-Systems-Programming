_stack_size = 0x1000;

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K

    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
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
        . = ALIGN(8);
        _sdata = . ;
        *(.data*)
        . = ALIGN(8);
        _edata = . ;

    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(8);
        _sbss = . ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _ebss = . ;

    } > RAM

    _sheap = _ebss;

    _stack_start = ORIGIN(RAM) + LENGTH(RAM) - _stack_size;

    .stack _stack_start (NOLOAD) : 
    {
        . = ALIGN(8);
        _sstack = . ;
        . = . + _stack_size;
        . = ALIGN(8);
        _estack = . ;

    } > RAM

}

ASSERT(_ebss <= _sstack, "ERROR: BSS has collided with stack")
