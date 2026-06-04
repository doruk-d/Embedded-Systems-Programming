#include "mpu_config.h"
#include "mpu.h"
#include "flash.h"
#include "sram.h"
#include "peripheral.h"
#include "scb.h"
#include <stdint.h>

#define FLASH_SIZE (512 * 1024U)
#define STACK_G_SIZE 128 // an average number to catch possible overflows
#define PERIPHERAL_SIZE (512 * 1024U * 1024U)

extern uint32_t _sdata;
extern uint32_t _ebss_mpu;
extern uint32_t _estack;
extern uint32_t _sstack;

static inline uint32_t mpu_size_val(uint32_t size){
    if (size <= 32)
        return 4;
    return (32 - __builtin_clz(size - 1)) - 1;
}

void mpu_init(void){
    // to avoid unexpected behavior disable the interrupts
    __asm__ volatile("cpsid i" ::: "memory");

    // check if mpu is present
    if (((MPU->TYPER >> 8) & 0xFF) == 0x00){
        __asm__ volatile("bkpt #0");
        while(1); // safety net in case execution continues
    }

    // disable mpu and its features for now
    MPU->CTRL = 0;

    // wait for store to be committed
    __asm__ volatile("dsb" ::: "memory");

    // flush the pipeline to ensure subsequent instructions are fetched
    __asm__ volatile("isb" ::: "memory");

    // 0: flash
    MPU->RNR = 0;

    // set base address
    MPU->RBAR = FLASH_MEM_BASE;

    uint32_t val = 0;
    // set TEX, S, C, B according to the datasheet
    val |= (0b000 << 19) | (0 << 18) | (1 << 17) | (0 << 16);
    // set XN, AP, SIZE, ENABLE
    val |= (0 << 28) | (0b111 << 24) | (mpu_size_val(FLASH_SIZE) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;


    // 1: SRAM size is not a power of 2 
    // split it into two regions to match the physical size 
    MPU->RNR = 1;

    MPU->RBAR = SRAM_BASE;

    val |= (0b000 << 19) | (1 << 18) | (1 << 17) | (0 << 16); 
    val |= (1 << 28) | (0b011 << 24) | (mpu_size_val(64 * 1024U) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;

    // 2: RAM second region remaining 32KB
    MPU->RNR = 2;

    MPU->RBAR = SRAM_BASE + (64 * 1024U);

    val |= (0b000 << 19) | (1 << 18) | (1 << 17) | (0 << 16); 
    val |= (1 << 28) | (0b011 << 24) | (mpu_size_val(32 * 1024U) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;


    // 3: stack
    MPU->RNR = 3;

    uint32_t size_stack = (uint32_t)&_estack - (uint32_t)&_sstack;

    MPU->RBAR = (uint32_t)&_sstack;

    val |= (0b000 << 19) | (1 << 18) | (1 << 17) | (0 << 16);
    val |= (1 << 28) | (0b011 << 24) | (mpu_size_val(size_stack) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;


    // 4: .data + .bss section
    MPU->RNR = 4;

    uint32_t size_data_bss = (uint32_t)&_ebss_mpu - (uint32_t)&_sdata;

    MPU->RBAR = (uint32_t)&_sdata;

    val |= (0b000 << 19) | (1 << 18) | (1 << 17) | (0 << 16);
    val |= (1 << 28) | (0b011 << 24) | (mpu_size_val(size_data_bss) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;


    // 5: stack guard  to detect collision between .stack and .data/.bss
    // triggers a fault on overflow from either side 
    MPU->RNR = 5;

    MPU->RBAR = (uint32_t)&_sstack;

    val |= (0b000 << 19) | (1 << 18) | (1 << 17) | (0 << 16);
    val |= (1 << 28) | (0b000 << 24) | (mpu_size_val(STACK_G_SIZE) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;


    // 6: peripherals 
    MPU->RNR = 6;

    MPU->RBAR = PERIPHERAL_BASE;

    // configure as device
    val |= (0b000 << 19) | (1 << 18) | (0 << 17) | (1 << 16);
    val |= (1 << 28) | (0b011 << 24) | (mpu_size_val(PERIPHERAL_SIZE) << 1) | (1 << 0);

    MPU->RASR = val;
    val = 0;

    // if buffer delay is a concern add strongly ordered configurations here
    
    // enable PRIVDEFENA for fallback in case mpu mapping is not enough and enable mpu
    MPU->CTRL |= (1 << 2) | (1 << 0);

    // ensure all data synchronization is complete
    __asm__ volatile("dsb" ::: "memory");
    // flush cpu pipeline and discard already fetched then re-fetch from memory
    __asm__ volatile("isb" ::: "memory");

    __asm__ volatile("cpsie i" ::: "memory");

    // enable memory management fault
    SCB->SHCSR |= (1 << 16);

}


