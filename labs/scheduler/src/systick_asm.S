.syntax unified
.thumb 

#define DWT_CYCCNT_ADDR 0xE0001004

.globl SysTick_Handler
.type  SysTick_Handler ,%function

SysTick_Handler:
    ldr r1, =DWT_CYCCNT_ADDR
    ldr r1, [r1]
    ldr r0, =wfi_latency_end
    str r1, [r0]
    b scheduler_run            

