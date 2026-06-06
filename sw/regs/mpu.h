#pragma once

#include <stdint.h>
#include "bit_ops.h"

typedef struct{
    volatile uint32_t TYPER;
    volatile uint32_t CTRL;
    volatile uint32_t RNR;
    volatile uint32_t RBAR;
    volatile uint32_t RASR;
}mpu_regs_t;

#define MPU_BASE 0xE000ED90UL
#define MPU ((volatile mpu_regs_t *)MPU_BASE)

// field descriptors
#define MPU_TYPER_DREGION FIELD(8,8)

#define MPU_CTRL_ENABLE FIELD(1,0)
#define MPU_CTRL_PRIVDEFENA FIELD(1,2)

#define MPU_RASR_ENABLE FIELD(1,0)
#define MPU_RASR_SIZE FIELD(5,1)
#define MPU_RASR_C FIELD(1,16)
#define MPU_RASR_B FIELD(1,17)
#define MPU_RASR_S FIELD(1,18)
#define MPU_RASR_TEX FIELD(3,19)
#define MPU_RASR_AP FIELD(3,24)
#define MPU_RASR_XN FIELD(1,28)