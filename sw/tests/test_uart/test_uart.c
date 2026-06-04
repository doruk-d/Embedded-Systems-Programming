#include "uart.h"

int main(void){
    if (uart_init(115200) != UART_OK)
        goto exit_main;

    uart_printf("enter the input: \r\n");

    int c;
    while (1){
        c = uart_getc();
        uart_putc((char)c);
    }

exit_main:
    while(1);
}
