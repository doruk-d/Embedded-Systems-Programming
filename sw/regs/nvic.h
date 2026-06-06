#pragma once

#include <stdint.h>

typedef struct {
    volatile uint32_t ISER[8];
    uint32_t _reserved[184];
    volatile uint32_t IPR[60];
}nvic_regs_t;

#define NVIC_BASE 0xE000E100UL
#define NVIC ((volatile nvic_regs_t *)NVIC_BASE)

static inline void nvic_enable_irq(uint32_t irq_num) {
    uint32_t offset = irq_num / 32;
    uint32_t irq_bit = irq_num % 32;
    NVIC->ISER[offset] |= (1U << irq_bit);
}

static inline void nvic_set_priority(uint32_t irq_pri, uint32_t irq_num) {
    uint32_t offset = irq_num / 4;
    uint32_t shift = (irq_num % 4) * 8;

    NVIC->IPR[offset] &= ~(0xFF << shift);
    NVIC->IPR[offset] |= ((irq_pri & 0xF) << (shift + 4)); // first 4 bits on this board is ignored
}