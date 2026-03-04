#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021000 + 0x18))
#define GPIOA_CRL (*(volatile uint32_t *)(0x40010800 + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(0x40010800 + 0x0C))
#define GPIOA_BSRR (*(volatile uint32_t *)(0x40010800 + 0x10))
#define LED 5

void delay(volatile int count);

int main(){
    RCC_APB2ENR |= (1 << 2);

    GPIOA_CRL &= ~(0xF << (LED * 4));
    GPIOA_CRL |= (1 << (LED * 4));

    uint8_t status = 0;
    while (1){
        if (status)
            GPIOA_BSRR = (1 << LED);
        else
            GPIOA_BSRR = (1 << (LED + 16));

        status = !status;

        delay(1000);
    }
}

void delay(volatile int count){
    while (count > 0)
        count--;
}

