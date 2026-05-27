#include "uart.h"
#include <stdint.h>
#include <stdarg.h>

#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (*(volatile uint32_t *)GPIOA_BASE)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_AFRL (*(volatile uint32_t *)(GPIOA_BASE + 0x20))


#define USART2_BASE 0x40004400
#define USART_SR (*(volatile uint32_t *)USART2_BASE)
#define USART_DR (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART_BRR (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART_CR1 (*(volatile uint32_t *)(USART2_BASE + 0x0C))

#define NVIC_ISERx 0xE000E100
#define NVIC_IPRx 0xE000E400

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
    RCC_AHB1ENR |= (1 << 0);
    RCC_APB1ENR |= (1 << 17);

    // configure gpio pins
    // af mode
    GPIOA_MODER &= ~((0x3 << (TX_PIN * 2)) | (0x3 << (RX_PIN * 2)));
    GPIOA_MODER |= (0x2 << (TX_PIN * 2)) | (0x2 << (RX_PIN * 2));

    // speed of pins set to low
    GPIOA_OSPEEDR &= ~((0x3 << (TX_PIN * 2)) | (0x3 << (RX_PIN * 2)));


    // af values for usart use
    GPIOA_AFRL &= ~((0xF << (TX_PIN * 4)) | (0xF << (RX_PIN * 4)));
    GPIOA_AFRL |= (0x7 << (TX_PIN * 4)) | (0x7 << (RX_PIN * 4));


    // configure USART2
    // baud rate 
    USART_BRR = (APB1_FREQ + (baud_rate / 2)) / baud_rate;

    // oversampling set tp 16 and word length set to 8 bits
    USART_CR1 &= ~((1 << 15) | (1 << 12));

    // rxne interrupt, usart, transmitter and receiver enable
    USART_CR1 |= (1 << 13) | (1 << 5) | (1 << 3) | (1 << 2);

    // enable and set priority of the interrupt
    uint32_t offset_iser = 0x04 * (USART2_IRQ_N / 32);
    volatile uint32_t *reg_iser = (volatile uint32_t *)(NVIC_ISERx + offset_iser);
    *reg_iser |= (1 << (USART2_IRQ_N % 32));

    uint32_t offset_ipr = 0x04 * (USART2_IRQ_N / 4);
    volatile uint32_t *reg_ipr = (volatile uint32_t *)(NVIC_IPRx + offset_ipr);
    *reg_ipr |=  (USART2_IRQ_PRI << ((USART2_IRQ_N % 4) * 8 + 4));

    return UART_OK;
}

void USART2_IRQHandler(void){
    if (((USART_SR >> 7) & 1) && ((USART_CR1 >> 7) & 1)){
        if (tx_head != tx_tail){

            uint8_t c = tx_buffer[tx_tail & (MAX_SIZE - 1)];
            tx_tail++;

            USART_DR = c;

            if (tx_head == tx_tail)
                USART_CR1 &= ~(1 << 7);
        }
    }

    if (((USART_SR >> 5) & 1) && ((USART_CR1 >> 5) & 1)){
        uint8_t c = USART_DR;
    
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
    USART_CR1 |= (1 << 7);

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
                case '%':
                    (void)uart_putc('%');
                    break;
            }
            fmt++;
        }
        else
            (void)uart_putc(*fmt++);
    }

    va_end(ap);
}
