#ifndef GPIO_REGS_H
#define GPIO_REGS_H

#define GPIOA_BSRR_ADDR 0x40020018UL

#ifndef __ASSEMBLER__

#include <stdint.h>

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

#endif

#endif
