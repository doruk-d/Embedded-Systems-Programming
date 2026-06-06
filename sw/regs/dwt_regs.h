#pragma once

#define DWT_CYCCNT_ADDR 0xE0001004UL

#ifndef __ASSEMBLER__

#include <stdint.h>
#include "bit_ops.h"

#define DEMCR (*(volatile uint32_t *)0xE000EDFCUL)

typedef struct{
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
}dwt_regs_t;

#define DWT_BASE 0xE0001000UL
#define DWT ((volatile dwt_regs_t *)DWT_BASE)

// position bit and width packing
#define DEMCR_TRCENA FIELD(1, 24)

#define DWT_CTRL_CYCCNTENA FIELD(1,0)
#define DWT_CTRL_NOCYCCNT FIELD(1,25)

#endif