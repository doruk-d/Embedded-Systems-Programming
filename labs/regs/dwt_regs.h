#ifndef DWT_REGS_H
#define DWT_REGS_H

#define DWT_CYCCNT_ADDR 0xE0001004UL

#ifndef __ASSEMBLER__

#include <stdint.h>

#define DEMCR (*(volatile uint32_t *)0xE000EDFCUL)

typedef struct{
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
}dwt_regs_t;

#define DWT_BASE 0xE0001000UL
#define DWT ((volatile dwt_regs_t *)DWT_BASE)

#endif

#endif
