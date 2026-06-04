#ifndef SCB_REGS_H
#define SCB_REGS_H

#include <stdint.h>

typedef struct{
    uint32_t _reserved0;
    volatile uint32_t ICSR;
    uint32_t _reserved[6];
    volatile uint32_t SHPR3; 
    volatile uint32_t SHCSR;
}sys_regs_t;

#define SCB_BASE 0xE000ED00UL
#define SCB ((volatile sys_regs_t *)SCB_BASE)

#endif
