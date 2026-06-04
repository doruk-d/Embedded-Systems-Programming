#ifndef FLASH_REGS_H
#define FLASH_REGS_H

#include <stdint.h>

#define FLASH_MEM_BASE 0x08000000UL
#define FLASH_REG_BASE 0x40023C00UL
#define FLASH_ACR (*(volatile uint32_t *)FLASH_REG_BASE)

#endif
