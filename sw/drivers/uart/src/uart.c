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

uart_status_t uart_init(uint32_t baud_rate){    
    if (baud_rate == 0 || baud_rate > 2625000) 
        return UART_ERR_INVALID_BAUD; 
    
    // enable gpio and usart clocks
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 17);

    // configure gpio pins
    // af mode
    GPIOA->MODER &= ~((0x3 << (TX_PIN * 2)) | (0x3 << (RX_PIN * 2)));
    GPIOA->MODER |= (0x2 << (TX_PIN * 2)) | (0x2 << (RX_PIN * 2));

    // speed of pins set to low
    GPIOA->OSPEEDR &= ~((0x3 << (TX_PIN * 2)) | (0x3 << (RX_PIN * 2)));


    // af values for usart use
    GPIOA->AFR[(TX_PIN / 8)] &= ~(0xF << (TX_PIN * 4));
    GPIOA->AFR[(TX_PIN / 8)] |= (0x7 << (TX_PIN * 4));

    GPIOA->AFR[(RX_PIN / 8)] &= ~(0xF << (RX_PIN * 4));
    GPIOA->AFR[(RX_PIN / 8)] |= (0x7 << (RX_PIN * 4));


    // configure USART2
    // baud rate 
    USART2->BRR = (APB1_FREQ + (baud_rate / 2)) / baud_rate;

    // oversampling set tp 16 and word length set to 8 bits
    USART2->CR1 &= ~((1 << 15) | (1 << 12));

    // rxne interrupt, usart, transmitter and receiver enable
    USART2->CR1 |= (1 << 13) | (1 << 5) | (1 << 3) | (1 << 2);

    // enable and set priority of the interrupt
    nvic_set_priority(USART2_IRQ_PRI, USART2_IRQ_N);
    nvic_enable_irq(USART2_IRQ_N);

    return UART_OK;
}

void USART2_IRQHandler(void){
    if (((USART2->SR >> 7) & 1) && ((USART2->CR1 >> 7) & 1)){
        if (tx_head != tx_tail){

            uint8_t c = tx_buffer[tx_tail & (MAX_SIZE - 1)];
            tx_tail++;

            USART2->DR = c;

            if (tx_head == tx_tail)
                USART2->CR1 &= ~(1 << 7);
        }
    }

    // implement ore check
    if (((USART2->SR >> 5) & 1) && ((USART2->CR1 >> 5) & 1)){
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

uart_status_t uart_putc(char c){
    __asm__ volatile("cpsid i");

    if (tx_head - tx_tail == MAX_SIZE){
        __asm__ volatile("cpsie i");
        return TX_BUFFER_FULL;
    }
 
    tx_buffer[tx_head & (MAX_SIZE - 1)] = (uint8_t)c;
    tx_head++;

    __asm__ volatile("cpsie i");

    // txeie enable
    USART2->CR1 |= (1 << 7);

    return TX_OK;
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
