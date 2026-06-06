#include "dwt.h"
#include "dwt_regs.h"

system_status_t dwt_init(void){
    DEMCR |= FIELD_VAL(DEMCR_TRCENA, 1); // enables dwt and imt units

    if (FIELD_READ(DWT->CTRL, DWT_CTRL_NOCYCCNT))
        return ERROR;
    
    DWT->CYCCNT = 0; // reset the value of cyccnt to ensure a clean start

    DWT->CTRL |= FIELD_VAL(DWT_CTRL_CYCCNTENA, 1); // enables cyccnt
                          
    return OK;
}
