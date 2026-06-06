#include "system_clock.h"
#include "rcc.h"
#include "flash.h"

void system_clock_init(void){
    // turn the PLL clock off 
    RCC->CR &= FIELD_CLEAR_MASK(RCC_CR_PLLON);

    // make sure it is locked
    while(FIELD_READ(RCC->CR, RCC_CR_PLLRDY));

    // set HSE on and wait for the signals to settle
    RCC->CR |= FIELD_VAL(RCC_CR_HSEON, 1);

    while(!(FIELD_READ(RCC->CR, RCC_CR_HSERDY)));

    // configure PLL
    RCC->PLLCFGR = FIELD_VAL(RCC_PLLCFGR_PLLM, 8) |
                FIELD_VAL(RCC_PLLCFGR_PLLN, 168) |
                FIELD_VAL(RCC_PLLCFGR_PLLSRC, 1) |
                FIELD_VAL(RCC_PLLCFGR_PLLQ, 7);
                // PLLP set to 0 by direct assignment
    // enable data, instruction caches, prefetch for lower latency and flash latency for safe reads
    FLASH_ACR |= FIELD_VAL(FLASH_ACR_DCEN, 1) | FIELD_VAL(FLASH_ACR_ICEN, 1) | FIELD_VAL(FLASH_ACR_PRFTEN, 1) |
                 FIELD_VAL(FLASH_ACR_LATENCY, 2);

    // set the prescalers for APBx buses
    RCC->CFGR &= FIELD_CLEAR_MASK(RCC_CFGR_PPRE1) & FIELD_CLEAR_MASK(RCC_CFGR_PPRE2);
    RCC->CFGR |= FIELD_VAL(RCC_CFGR_PPRE1, 4);

    // set the PLL on wait for it to settle then set it as sysclk
    RCC->CR |= FIELD_VAL(RCC_CR_PLLON, 1);

    while(!(FIELD_READ(RCC->CR, RCC_CR_PLLRDY)));

    RCC->CFGR |= FIELD_VAL(RCC_CFGR_SW, 2);

    // wait for it to lock
    while(FIELD_READ(RCC->CFGR, RCC_CFGR_SWS) != 0x2);
}
