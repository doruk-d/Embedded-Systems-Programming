#ifndef UART_HEADER_H
#define UART_HEADER_H

#include <stdint.h>

typedef enum{
    UART_OK = 0,
    UART_ERR_INVALID_BAUD,
    TX_OK, 
    TX_BUFFER_FULL
}uart_status_t;

uart_status_t uart_init(uint32_t baud_rate);   
uint8_t uart_getc(void);
uart_status_t uart_putc(char c);
void uart_printf(const char *fmt, ...);

#endif 
