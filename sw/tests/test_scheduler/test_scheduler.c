#include <stdint.h>
#include <stddef.h>
#include "dwt.h"
#include "measure_gpio.h"
#include "uart.h"
#include "scheduler.h"

void task1(void*);
void task2(void*);
void task3(void*);
void task4(void*);
void task5(void* ptr);
void task6(void*);
void task7(void*);
void task8(void* ptr);
void task9(void*);
void task10(void*);

int main(void){
    uart_init(115200);
    
    gpio_init();

    uint32_t x = 9;
    
    if (task_create(NULL,&task1) == FAIL) uart_printf("FAIL_1\n"); 
    if (task_create(NULL, &task2) == FAIL) uart_printf("FAIL_2\n"); 
    if (task_create(NULL, NULL) == FAIL) uart_printf("FAIL_3\n");
    if (task_create(NULL,&task4) == FAIL) uart_printf("FAIL_4\n"); 
    if (task_create((void*)"xyz",&task5) == FAIL) uart_printf("FAIL_5\n"); 
    if (task_create(NULL,&task6) == FAIL) uart_printf("FAIL_6\n"); 
    if (task_create(NULL,&task7) == FAIL) uart_printf("FAIL_7\n"); 
    if (task_create(&x,&task8) == FAIL) uart_printf("FAIL_8\n"); 
    if (task_create(NULL,&task9) == FAIL) uart_printf("FAIL_9\n"); 
    if (task_create(NULL,&task10) == FAIL) uart_printf("FAIL_10\n"); 

    if (dwt_init() == UNSUPPORTED_CYCLE_COUNTER) 
        while(1);

    scheduler_init();

}

void task1(void*){
    uart_printf("TASK_1 running...\n"); 
    yield();
}

void task2(void*){
    uart_printf("TASK_2 running...\n"); 
    while(1);
}

void task3(void*){
    uart_printf("TASK_3 running...\n"); 
    while(1);
}

void task4(void*){
    uart_printf("TASK_4 running...\n"); 
    while(1);
}

void task5(void* ptr){
    uart_printf("TASK_5 running...\n"); 
    char *p = (char *)ptr;
    uart_printf("%s\n", p);
    while(1);
}

void task6(void*){
    uart_printf("TASK_6 running...\n"); 
    while(1);
}

void task7(void*){
    uart_printf("TASK_7 running...\n"); 
    while(1);
}

void task8(void* ptr){
    uart_printf("TASK_8 running...\n");
    uint32_t p = *((uint32_t *)ptr);
    uart_printf("%d\n", p);
    while(1);
}

void task9(void*){
    uart_printf("TASK_9 running...\n"); 
    while(1);
}

void task10(void*){
    uart_printf("TASK_10 running...\n"); 
    while(1);
}
