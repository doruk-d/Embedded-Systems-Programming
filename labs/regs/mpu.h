#ifndef MPU_REGS_H
#define MPU_REGS_H

#include <stdint.h>

typedef struct{
    volatile uint32_t TYPER;
    volatile uint32_t CTRL;
    volatile uint32_t RNR;
    volatile uint32_t RBAR;
    volatile uint32_t RASR;
}mpu_regs_t;

#define MPU_BASE 0xE000ED90UL
#define MPU ((volatile mpu_regs_t *)MPU_BASE)

#endif
