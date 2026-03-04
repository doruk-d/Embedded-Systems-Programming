#include "system_clock.h"
#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void ResetHandler(void);

__attribute__((section(".isr_vector")))

const uint32_t vector_table[]={
    (uint32_t)&_estack,
    (uint32_t)ResetHandler
};

void ResetHandler(void){
    uint32_t *src_fdata = &_sidata;
    uint32_t *dest_sdata = &_sdata;
    uint32_t *dest_edata = &_edata;
    uint32_t *s_bss = &_sbss;
    uint32_t *e_bss = &_ebss;

    while (dest_sdata < dest_edata)
        *dest_sdata++ = *src_fdata++;

    while (s_bss < e_bss)
        *s_bss++ = 0;

    system_clock_init();

    extern int main(void);
    main();


}
