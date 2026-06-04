#ifndef USART_REGS_H
#define USART_REGS_H

#include <stdint.h>

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
}usart_regs_t;

#define USART2_BASE 0x40004400UL
#define USART2 ((volatile usart_regs_t *)USART2_BASE)

#endif
