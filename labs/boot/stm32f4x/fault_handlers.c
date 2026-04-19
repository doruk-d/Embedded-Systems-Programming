#include "fault_handlers.h"
#include <stdint.h>

void MemManageHandler(void){
    __asm__ volatile("bkpt #0");
}

__attribute__((naked)) void HardFaultHandler(void){
    __asm__ volatile(
            "tst lr, #4     \n\t" // check the bit 2 for stack
            "ite eq         \n\t" // if 0
            "mrseq r0, msp  \n\t" // move the msp address to r0
            "mrsne r0, psp  \n\t" // move the psp address to r0
            "b hard_fault_dump  \n\t" // branch into the function to examine
            );
}

void hard_fault_dump(uint32_t *stack_frame){
    uint32_t pc = stack_frame[6]; // where the fault happened
    uint32_t lr = stack_frame[5]; // what called the faulting code
    uint32_t xPSR = stack_frame[7]; // processor state at the fault time

    // use the values to silence the gcc warnings until i implement a uart_printf function 
    (void)pc;
    (void)lr;
    (void)xPSR;
    
    __asm__ volatile("bkpt #1");

}
