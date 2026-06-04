#ifndef RCC_REGS_H
#define RCC_REGS_H

#include <stdint.h>

typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t AHB1ENR;
    volatile uint32_t APB1ENR;
}rcc_regs_t;

#define RCC_BASE 0x40023800UL
#define RCC ((volatile rcc_regs_t *)RCC_BASE)

#endif
