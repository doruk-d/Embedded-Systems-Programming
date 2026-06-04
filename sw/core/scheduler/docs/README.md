# Preemptive Round-Robin Scheduler

## Overview

A preemptive round-robin scheduler with cooperative yield support, built bare-metal on the STM32F401RE with no HAL or vendor abstractions.

## Hardware & Tooling
* MCU: STM32F401RE (ARM Cortex-M4 @ 84MHz)
* Debug: OpenOCD + GDB
* Measurement: SparkFun logic analyzer at 24MHz, DWT cycle counter via GDB

## Project Structure

```bash
    scheduler/
    ├── docs/
    │   ├── img/
    │   │   ├── first.png
    │   │   ├── pattern.png
    │   │   └── steady_state.png
    │   └── README.md
    ├── include/
    │   ├── asm_offsets.h
    │   ├── dwt.h
    │   ├── measure_gpio.h
    │   ├── scheduler.h
    │   └── systick.h
    ├── src/
    │   ├── context_switch.S
    │   ├── dwt.c
    │   ├── measure_gpio.c
    │   ├── scheduler.c
    │   ├── svc.c
    │   └── systick.c
    └── makefile
```

## Architecture

### Cold Start
The cold start is triggered by `scheduler_init` in `main` and implemented by NULL checks (`cbz` and `cbnz`) in PendSV to create a safe and unified context switch logic. First check ensures the register save step is skipped to prevent stack corruption. The second ensures SysTick is initialized and returned to the PSP by EXC_RETURN value of `0xFFFFFFFD`.

### Context Switch
* Context switch is implemented under two policies, preemptive switch owned by SysTick, cooperative switch owned by SVC call. PendSV owns the switch mechanism which is triggered by either policy.

* SysTick, fires every 1ms and calls `scheduler_run` to advance the circular linked list and pends the PendSV.

* SVC/Yield, fires SVC and `SVC_Handler_c` extracts call number from saved PC offset, dispatches to the `scheduler_run`.

* Switch mechanism in PendSV, saves the current task's software frame by pushing it into the task stack, and restores the software frame of the next task by popping it from the task stack. Finally, updates the current task pointer to the former next task.

### Lifecycle

* Tasks are scheduled in a fixed round-robin order using a circular linked list. 

* Lifecycle of a task starts with `task_create` setting up the full stack frame with LR seeded with `task_remove` to auto-remove returning tasks. `task_create` rejects NULL function pointers and returns `FAIL`.

* `scheduler_register` adds tasks to the circular linked list as part of task creation. Task pool is statically allocated with a hard limit of `MAX_TASKS`. 

* `task_remove` acts as an auto-reaping tool on task return. The target task is removed from the list; if no tasks remain MCU halts in low power, otherwise PendSV is pended to trigger the next switch. The linked list traversal is vulnerable to interrupt-driven corruption, which is solved by disabling interrupts (`cpsid`/`cpsie`) around the critical section.


## Measurements

### Context Switch
Instrumented `DWT_CYCCNT` to calculate context switch latency. Values are stored at the start and end of the context switch blocks in PendSV as `sw_start_cyccnt` and `sw_end_cyccnt`, which gives a result of ~63 cycles (*around 0.75µs at 84MHz*).

```bash
(gdb) p sw_start_cyccnt 
$11 = 2152
(gdb) p sw_end_cyccnt 
$12 = 2215
(gdb) 
```

Compared to FreeRTOS's latency of ~80 cycles, however not a fair comparison due to its advanced features such as, priority scheduling, stack overflow checks, and trace hooks. This scheduler is more lightweight and includes none of these.

### Exception Dispatch

<p align="center">
  <b>First Exception Dispatch</b><br/>
  <img width="800" alt="First Exception Dispatch" src="https://github.com/user-attachments/assets/3b7dfa22-d71b-4d27-b4e0-b3dcbadcfe8e" />
</p>

<p align="center">
  <b>Steady State Exception Dispatch</b><br/>
  <img width="800" alt="Steady State Exception Dispatch" src="https://github.com/user-attachments/assets/6ea4ea05-cb47-455a-96df-56e68ec48585" />
</p>

<p align="center">
  <b>Repeated Context Switch Pattern</b><br/>
  <img width="800" alt="Repeated Context Switch Pattern" src="https://github.com/user-attachments/assets/62c1538a-41eb-4a63-aa3c-d1ddd68e3e0c" />
</p>

As seen above, first dispatch takes ~250ns and steady state ~292ns, however the tool used during measurement samples at 24MHz, giving ±3 cycles margin of error. The 42ns difference between the two measurements is within the analyzer's resolution, so it may or may not be architectural.

## System Clock

System Clock configuration was rewritten to deeply understand the clock tree and PLL. This included enabling the ART accelerator (*instruction cache, data cache, prefetch*) which directly impacted cycle count measurements.

## Build & Flash

```bash
make
make flash
```
