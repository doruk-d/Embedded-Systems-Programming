#include "system_clock.h"
#include "rcc.h"
#include "flash.h"

void system_clock_init(void){
    // turn the PLL clock off 
    RCC->CR &= ~(1 << 24);

    // make sure it is locked
    while(((RCC->CR >> 25) & 0x1));

    // set HSE on and wait for the signals to settle
    RCC->CR |= (1 << 16);

    while(!((RCC->CR >> 17) & 0x1));

    // configure PLL
    RCC->PLLCFGR = (8 << 0) | // PLLM
                  (168 << 6) | // PLLN
                  (0 << 16) | // PLLP
                  (1 << 22) | // PLLSRC
                  (7 << 24); // PLLQ

    // enable data, instruction caches, prefetch for lower latency and flash latency for safe reads
    FLASH_ACR |= (1 << 10) | (1 << 9) | (1 << 8) | (2 << 0);

    // set the prescalers for APBx buses
    RCC->CFGR &= ~((7 << 13) | (7 << 10));
    RCC->CFGR |= (4 << 10);

    // set the PLL on wait for it to settle then set it as sysclk
    RCC->CR |= (1 << 24);

    while(!((RCC->CR >> 25) & 0x1));

    RCC->CFGR |= (2 << 0);

    // wait for it to lock
    while((((RCC->CFGR >> 2) & 0x3) != 0x2));

}

