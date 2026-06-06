#pragma once

#define GPIOA_BSRR_ADDR 0x40020018UL

#ifndef __ASSEMBLER__

#include <stdint.h>
#include "bit_ops.h"

typedef struct{
    volatile uint32_t MODER;
    uint32_t _reserved0;
    volatile uint32_t OSPEEDR;
    uint32_t _reserved[3];
    volatile uint32_t BSRR;
    uint32_t _reserved1;
    volatile uint32_t AFR[2];
}gpio_regs_t;

#define GPIOA_BASE 0x40020000UL
#define GPIOA ((volatile gpio_regs_t *)GPIOA_BASE)

#define GPIO_MODER(pin) FIELD(2, ((pin) * 2))
#define GPIO_BSRR(pin) FIELD(1, (pin))
#define GPIO_OSPEEDR(pin) FIELD(2, ((pin) * 2))
#define GPIO_AFR(pin) FIELD(4, ((pin) * 4))

#endif