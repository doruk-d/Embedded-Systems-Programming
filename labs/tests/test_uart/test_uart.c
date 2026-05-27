#include "uart.h"
#include <stdint.h>

int main(void){
    if (uart_init(115200) != UART_OK)
        goto exit_main;

    uart_printf("enter the input: \r\n");

    int c;
    while (1){
        if ((c = uart_getc())== -1)
            continue;
        uart_putc((char)c);
    }

exit_main:
    while(1);
}
