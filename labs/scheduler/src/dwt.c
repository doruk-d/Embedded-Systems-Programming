#include "dwt.h"
#include "dwt_regs.h"

cyc_cnt_t dwt_init(void){
    DEMCR |= (1 << 24); // enables dwt and imt units 

    if ((DWT->CTRL >> 25) & 0x1)
        return UNSUPPORTED_CYCLE_COUNTER;
    
    DWT->CYCCNT = 0; // reset the value of cyccnt to ensure a clean start

    DWT->CTRL |= (1 << 0); // enables cyccnt
                          
    return SUPPORTED_CYCLE_COUNTER;
}
