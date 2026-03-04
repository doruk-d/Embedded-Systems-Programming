#include <stdint.h>

#define GPIOA_BASE 0x40020000
#define RCC_BASE 0x40023800

#define RCC_CR (*(volatile uint32_t *)RCC_BASE)
#define RCC_PLLCFGR (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_APB1ENR_PWREN (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define FLASH_ACR (*(volatile uint32_t *)0x40023C00)

void system_clock_init(void){
    // step 1 enable the clock to the power controller
    RCC_APB1ENR_PWREN |= (1 << 28);

    // step 2 enable HSE
    RCC_CR |= (1 << 16);
    // wait for it to lock
    while (!((RCC_CR >> 17) & 0x1));

    // step 3 set the flash latency
    FLASH_ACR = (2 << 0);
    
    // step 4 set the source for clock in this case HSE
    RCC_PLLCFGR = (8 << 0) |    // PLLM = 8
                  (336 << 6) |  // PLLN = 336
                  (1 << 16) |   // PLLP = 4
                  (1 << 22) |   // HSE
                  (7 << 24);    // PLLQ = 7  

    // step 5 set PLL on in RCC_CR
    RCC_CR |= (1 << 24);
    // wait for it to lock
    while (!((RCC_CR >> 25) & 0x1));
    
    // set APB1 prescaler on
    RCC_CFGR |= (4 << 10);

    // step 6 change the system clock to PLL
    RCC_CFGR |= (2 << 0);
    // wait for switch to complete
    while (((RCC_CFGR >> 2) & 0x3) != 0x2);

}
