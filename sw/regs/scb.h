#pragma once

#include <stdint.h>
#include "bit_ops.h"

typedef struct{
    uint32_t _reserved0;
    volatile uint32_t ICSR;
    uint32_t _reserved[6];
    volatile uint32_t SHPR3; 
    volatile uint32_t SHCSR;
}sys_regs_t;

#define SCB_BASE 0xE000ED00UL
#define SCB ((volatile sys_regs_t *)SCB_BASE)

// field descriptors
#define SCB_ICSR_PENDSVSET FIELD(1,28)

#define SCB_SHPR3_PRI_14 FIELD(8,16)

#define SCB_SHCSR_MEMFAULTENA FIELD(1,16)
