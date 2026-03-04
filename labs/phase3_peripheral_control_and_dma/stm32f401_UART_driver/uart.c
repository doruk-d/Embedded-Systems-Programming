#include "uart.h"
#include <stdint.h>

#define PCLK1_FREQ 42000000UL
#define GPIOA_BASE 0x40020000
#define USART_BASE 0x40004400
#define RCC_BASE 0x40023800
#define STK_BASE 0xE000E010

#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_MODER (*(volatile uint32_t *)GPIOA_BASE)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)(GPIOA_BASE + 0x08))

#define USART_SR (*(volatile uint32_t *)USART_BASE)
#define USART_DR (*(volatile uint32_t *)(USART_BASE + 0x04))
#define USART_BRR (*(volatile uint32_t *)(USART_BASE + 0x08))
#define USART_CR1 (*(volatile uint32_t *)(USART_BASE + 0x0C))

#define PIN 2
#define PIN2 3


static inline void gpio_set_af(uint32_t pin){
    GPIOA_MODER &= ~(3 << (pin * 2));
    GPIOA_MODER |= (2 << (pin * 2));

    GPIOA_OSPEEDR |= (1 << (pin * 2));

    uint32_t offset = (pin < 8) ? 0x20 : 0x24;
    uint32_t shift = (pin % 8) * 4;
    
    volatile uint32_t *reg_AF = (volatile uint32_t *)(GPIOA_BASE + offset);
    
    *reg_AF &= ~(15 << shift);
    *reg_AF |= (7 << shift);
}

uart_err_t uart_init(uint32_t baud){
    if (baud == 0 || baud > 2625000) 
        return UART_ERR_INVALID_BAUD; 

    // set the GPIOA clock on 
    RCC_AHB1ENR |= (1 << 0);

    // set the USART2 clock on 
    RCC_APB1ENR |= (1 << 17);

    // GPIOA settings
    gpio_set_af(PIN);
    gpio_set_af(PIN2);

    // set AF 
    // set baud rate
    USART_BRR = (PCLK1_FREQ + (baud / 2)) / baud;

    // set TE, RE on
    USART_CR1 |= (1 << 2) | (1 << 3);

    // UE enable 
    USART_CR1 |= (1 << 13);

    return UART_OK;
}

void uart_putc(char c){
    // wait for TXE 
    while (!((USART_SR >> 7) & 0x1));

    USART_DR = c;
}

void uart_puts(char *s){
    while (*s)
        uart_putc(*s++);

    // make sure all data has been transmitted before returning
    while (!((USART_SR >> 6) & 0x1));
}

char uart_getc(void){
    // wait if RXNE has data to read
    while (!((USART_SR >> 5) & 0x1));
    
    char c = USART_DR;

    return c;

}
void uart_gets(char *s, uint32_t max_len){
    uint32_t i = 0;
    while (i < max_len - 1){
        char ch = uart_getc();

        uart_putc(ch);

        if (ch == '\r' || ch == '\n'){
            uart_putc('\r');
            uart_putc('\n');
            break;
        }
        
        s[i++] = ch;
    }

    s[i] = '\0';
}
