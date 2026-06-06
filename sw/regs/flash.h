#pragma once

#include <stdint.h>
#include "bit_ops.h"

#define FLASH_MEM_BASE 0x08000000UL
#define FLASH_REG_BASE 0x40023C00UL
#define FLASH_ACR (*(volatile uint32_t *)FLASH_REG_BASE)

#define FLASH_ACR_LATENCY FIELD(4,0)
#define FLASH_ACR_PRFTEN FIELD(1,8)
#define FLASH_ACR_ICEN FIELD(1,9)
#define FLASH_ACR_DCEN FIELD(1,10)