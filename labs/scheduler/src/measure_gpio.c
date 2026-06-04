#include "gpio.h"
#include "rcc.h"
#include "measure_gpio.h"

void gpio_init(void){
    RCC->AHB1ENR |= (1 << 0);

    GPIOA->MODER |= (1 << (PIN * 2));
}

