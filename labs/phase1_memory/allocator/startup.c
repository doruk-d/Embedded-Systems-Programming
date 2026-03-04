#include <stdint.h>

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

void ResetHandler(void);
void HardFaultHandler(void);

__attribute__((section(".isr_vector")))

const uint32_t vector_table[]={
    (uint32_t)&_estack,
    (uint32_t)ResetHandler,
    0, // in cpu this slot is reserved for NMI, since I don't have it implemented I will put a placeholder.
    (uint32_t)HardFaultHandler
};

void ResetHandler(){
    uint32_t *f_data = &_sidata;
    uint32_t *s_data = &_sdata;
    uint32_t *e_data = &_edata;
    uint32_t *s_bss = &_sbss;
    uint32_t *e_bss = &_ebss;

    while (s_data < e_data)
        *s_data++ = *f_data++;

    while (s_bss < e_bss)
        *s_bss++ = 0;

    extern int main(void);
    main();
}

void HardFaultHandler(){
    while (1);
}
