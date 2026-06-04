#include <stdint.h>

#define STACK_TOP (0x20000000 + 0x5000) // base plus ram size
                                        
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void Reset_Handler(void);

__attribute__((section(".isr_vector")))

const uint32_t vector_table[] = {
    (uint32_t)STACK_TOP,
    (uint32_t)Reset_Handler
};

void Reset_Handler(){
    uint32_t *src_data = &_sidata;
    uint32_t *dest_sdata = &_sdata;
    uint32_t *dest_edata = &_edata;

    while (dest_sdata < dest_edata)
        *dest_sdata++ = *src_data++;


    uint32_t *p_sbss = &_sbss;
    uint32_t *p_ebss = &_ebss;

    while (p_sbss < p_ebss)
        *p_sbss++ = 0;

    extern int main(void);
    main();
}



