#include "system_clock.h"
#include <stdint.h>

#define RCC_BASE 0x40023800
#define RCC_CR *(volatile uint32_t *)RCC_BASE
#define RCC_PLLCFGR *(volatile uint32_t *)(RCC_BASE + 0x04)
#define RCC_CFGR *(volatile uint32_t *)(RCC_BASE + 0x08)

#define FLASH_BASE 0x40023C00
#define FLASH_ACR *(volatile uint32_t *)FLASH_BASE

void system_clock_init(void){
    // turn the PLL clock off 
    RCC_CR &= ~(1 << 24);

    // make sure it is locked
    while(((RCC_CR >> 25) & 0x1));

    // set HSE on and wait for the signals to settle
    RCC_CR |= (1 << 16);

    while(!((RCC_CR >> 17) & 0x1));

    // configure PLL
    RCC_PLLCFGR = (8 << 0) | // PLLM 
                  (168 << 6) | // PLLN
                  (0 << 16) | // PLLP
                  (1 << 22) | // PLLSRC
                  (7 << 24); // PLLQ

    // enable data, instruction caches, prefetch for lower latency and flash latency for safe reads
    FLASH_ACR |= (1 << 10) | (1 << 9) | (1 << 8) | (2 << 0);

    // set the prescalers for APBx buses
    RCC_CFGR &= ~((7 << 13) | (7 << 10));
    RCC_CFGR |= (4 << 10);

    // set the PLL on wait for it to settle then set it as sysclk
    RCC_CR |= (1 << 24);

    while(!((RCC_CR >> 25) & 0x1));

    RCC_CFGR |= (2 << 0);

    // wait for it to lock
    while((((RCC_CFGR >> 2) & 0x3) != 0x2));

}

