#pragma once

#include <stdint.h>
#include "bit_ops.h"

typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t AHB1ENR;
    volatile uint32_t APB1ENR;
}rcc_regs_t;

#define RCC_BASE 0x40023800UL
#define RCC ((volatile rcc_regs_t *)RCC_BASE)

// field descriptors
#define RCC_CR_HSEON FIELD(1,16)
#define RCC_CR_HSERDY FIELD(1,17)
#define RCC_CR_PLLON FIELD(1,24)
#define RCC_CR_PLLRDY FIELD(1,25)

#define RCC_PLLCFGR_PLLM FIELD(6,0)
#define RCC_PLLCFGR_PLLN FIELD(9,6)
#define RCC_PLLCFGR_PLLP FIELD(2,16)
#define RCC_PLLCFGR_PLLSRC FIELD(1,22)
#define RCC_PLLCFGR_PLLQ FIELD(4,24)

#define RCC_CFGR_SW FIELD(2,0)
#define RCC_CFGR_SWS FIELD(2,2)
#define RCC_CFGR_PPRE1 FIELD(3,10)
#define RCC_CFGR_PPRE2 FIELD(3,13)

#define RCC_AHB1ENR_GPIOAEN FIELD(1,0)

#define RCC_APB1ENR_USART2EN FIELD(1,17)