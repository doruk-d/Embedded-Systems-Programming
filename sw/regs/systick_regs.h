#ifndef SYSTICK_REGS_H
#define SYSTICK_REGS_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
}stk_regs_t;

#define SYSTICK_BASE 0xE000E010UL
#define STK ((volatile stk_regs_t *)SYSTICK_BASE)

#endif SYSTICK_REGS_H
