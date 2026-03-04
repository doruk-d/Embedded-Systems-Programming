#ifndef UART_H
#define UART_H

#include <stdint.h>

typedef enum{
    UART_OK,
    UART_ERR_INVALID_BAUD
}uart_err_t;

uart_err_t uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(char *s);
char uart_getc(void);
void uart_gets(char *s, uint32_t max_len);

#endif
