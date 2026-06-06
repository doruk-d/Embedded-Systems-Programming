#include "uart.h"
#include "usart.h"
#include "rcc.h"
#include "gpio.h"
#include "nvic.h"
#include <stdint.h>
#include <stdarg.h>

#define USART2_IRQ_N 38
#define USART2_IRQ_PRI 5

#define APB1_FREQ 42000000
#define RX_PIN 2
#define TX_PIN 3

#define MAX_SIZE 256

static volatile uint8_t tx_buffer[MAX_SIZE]; // one byte per slot 
static volatile uint32_t tx_head, tx_tail;

static volatile uint8_t rx_buffer[MAX_SIZE];
static volatile uint32_t rx_head, rx_tail;

system_status_t uart_init(uint32_t baud_rate){
    if (baud_rate == 0 || baud_rate > 2625000) 
        return ERROR;
    
    // enable gpio and usart clocks
    RCC->AHB1ENR |= FIELD_VAL(RCC_AHB1ENR_GPIOAEN, 1);
    RCC->APB1ENR |= FIELD_VAL(RCC_APB1ENR_USART2EN, 1);

    // configure gpio pins
    // af mode
    GPIOA->MODER &= FIELD_CLEAR_MASK(GPIO_MODER(TX_PIN)) & FIELD_CLEAR_MASK(GPIO_MODER(RX_PIN));
    GPIOA->MODER |= FIELD_VAL(GPIO_MODER(TX_PIN), 0x2) | FIELD_VAL(GPIO_MODER(RX_PIN), 0x2);

    // speed of pins set to low
    GPIOA->OSPEEDR &= FIELD_CLEAR_MASK(GPIO_OSPEEDR(TX_PIN)) & FIELD_CLEAR_MASK(GPIO_OSPEEDR(RX_PIN));


    // af values for usart use
    GPIOA->AFR[(TX_PIN / 8)] &= FIELD_CLEAR_MASK(GPIO_AFR(TX_PIN));
    GPIOA->AFR[(TX_PIN / 8)] |= FIELD_VAL(GPIO_AFR(TX_PIN), 0x7);

    GPIOA->AFR[(RX_PIN / 8)] &= FIELD_CLEAR_MASK(GPIO_AFR(RX_PIN));
    GPIOA->AFR[(RX_PIN / 8)] |= FIELD_VAL(GPIO_AFR(RX_PIN), 0x7);


    // configure USART2
    // baud rate 
    USART2->BRR = (APB1_FREQ + (baud_rate / 2)) / baud_rate;

    // oversampling set tp 16 and word length set to 8 bits
    USART2->CR1 &= FIELD_CLEAR_MASK(USART_CR1_OVER8) & FIELD_CLEAR_MASK(USART_CR1_M);

    // rxne interrupt, usart, transmitter and receiver enable
    USART2->CR1 |= FIELD_VAL(USART_CR1_UE, 1) | FIELD_VAL(USART_CR1_RXNEIE, 1) | FIELD_VAL(USART_CR1_TE, 1) |
                   FIELD_VAL(USART_CR1_RE, 1);

    // enable and set priority of the interrupt
    nvic_set_priority(USART2_IRQ_PRI, USART2_IRQ_N);
    nvic_enable_irq(USART2_IRQ_N);

    return OK;
}

void USART2_IRQHandler(void){
    if (FIELD_READ(USART2->SR, USART_SR_TXE) && FIELD_READ(USART2->CR1,USART_CR1_TXEIE)){
        if (tx_head != tx_tail){

            uint8_t c = tx_buffer[tx_tail & (MAX_SIZE - 1)];
            tx_tail++;

            USART2->DR = c;

            if (tx_head == tx_tail)
                USART2->CR1 &= FIELD_CLEAR_MASK(USART_CR1_TXEIE);
        }
    }

    if (FIELD_READ(USART2->SR, USART_SR_RXNE) && FIELD_READ(USART2->CR1, USART_CR1_RXNEIE)){
        uint8_t c = USART2->DR;
    
        if ((rx_head - rx_tail) != MAX_SIZE){
        
            rx_buffer[rx_head & (MAX_SIZE - 1)] = c;
            rx_head++;
        }
    }

}

uint8_t uart_getc(void){
    while (rx_head == rx_tail);

    uint8_t c = rx_buffer[rx_tail & (MAX_SIZE - 1)];
    rx_tail++;

    return c;
}

system_status_t uart_putc(char c){
    __asm__ volatile("cpsid i");

    if (tx_head - tx_tail == MAX_SIZE){
        __asm__ volatile("cpsie i");
        return ERROR;
    }
 
    tx_buffer[tx_head & (MAX_SIZE - 1)] = (uint8_t)c;
    tx_head++;

    __asm__ volatile("cpsie i");

    // txeie enable
    USART2->CR1 |= FIELD_VAL(USART_CR1_TXEIE, 1);

    return OK;
}

static void uart_print_int(int val){
    if (val == 0){
        (void)uart_putc('0');
        return;
    }

    uint32_t uval;
    if (val < 0){
        (void)uart_putc('-');
        uval = -(uint32_t)val;
    }
    else
        uval = (uint32_t)val;

    char digits_arr[10];
    int i = 0;
    while (uval != 0){
        uint8_t digit = uval % 10;
        digits_arr[i++] = digit + '0';
        uval /= 10;
    }

    for (int j = i - 1; j >= 0; j--){
        char c = digits_arr[j];
        (void)uart_putc(c);
    }
}

static void uart_print_s(const char *str){
    while (*str != '\0')
        (void)uart_putc(*str++);
}

void uart_printf(const char *fmt, ...){
    // parsing macros
    va_list ap;
    va_start(ap, fmt);

    while (*fmt != '\0'){
        if (*fmt == '%'){
            switch (*(++fmt)){
                case 'd':{
                    int val = va_arg(ap, int);
                    uart_print_int(val);

                    break;
                }
                case 's':{
                    char *str = va_arg(ap, char*);
                    uart_print_s(str);
                    break;
                }
                default:
                    (void)uart_putc(*fmt);
                    break;
            }
            fmt++;
        }
        else
            (void)uart_putc(*fmt++);
    }

    va_end(ap);
}
