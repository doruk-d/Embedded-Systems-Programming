#pragma once

#include <stdint.h>
#include "bit_ops.h"

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
}usart_regs_t;

#define USART2_BASE 0x40004400UL
#define USART2 ((volatile usart_regs_t *)USART2_BASE)

// field descriptors
#define USART_SR_RXNE FIELD(1,5)
#define USART_SR_TXE FIELD(1,7)

#define USART_CR1_RE FIELD(1,2)
#define USART_CR1_TE FIELD(1,3)
#define USART_CR1_RXNEIE FIELD(1,5)
#define USART_CR1_TXEIE FIELD(1,7)
#define USART_CR1_M FIELD(1,12)
#define USART_CR1_UE FIELD(1,13)
#define USART_CR1_OVER8 FIELD(1,15)
