#include "gpio.h"
#include "rcc.h"
#include "measure_gpio.h"

void gpio_init(void){
    RCC->AHB1ENR |= FIELD_VAL(RCC_AHB1ENR_GPIOAEN, 1);

    GPIOA->MODER &= FIELD_CLEAR_MASK(GPIO_MODER(PIN));
    GPIOA->MODER |= FIELD_VAL(GPIO_MODER(PIN),1);
}

