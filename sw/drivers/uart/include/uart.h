#pragma once
#include "common.h"
#include <stdint.h>

system_status_t uart_init(uint32_t baud_rate);
uint8_t uart_getc(void);
system_status_t uart_putc(char c);
void uart_printf(const char *fmt, ...);