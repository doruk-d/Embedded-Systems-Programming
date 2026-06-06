#pragma once

#include <stdint.h>
#include "bit_ops.h"

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
}stk_regs_t;

#define SYSTICK_BASE 0xE000E010UL
#define STK ((volatile stk_regs_t *)SYSTICK_BASE)

// field descriptors
#define SYSTICK_CTRL_ENABLE FIELD(1,0)
#define SYSTICK_CTRL_TICKINT FIELD(1,1)
#define SYSTICK_CTRL_CLKSOURCE FIELD(1,2)