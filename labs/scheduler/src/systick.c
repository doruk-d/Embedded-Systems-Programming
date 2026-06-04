#include "systick.h"
#include "systick_regs.h"
#include "scheduler.h"

#define CPU_FREQ 84000000
#define SYSTICK_FREQ 1000
#define RELOAD_VAL ((CPU_FREQ / SYSTICK_FREQ) - 1)

void systick_init(void){
    STK->LOAD = RELOAD_VAL;

    STK->VAL = 0;

    STK->CTRL = (1 << 2) | (1 << 1) | (1 << 0);
    
}

void SysTick_Handler(void){
    scheduler_run();
}
