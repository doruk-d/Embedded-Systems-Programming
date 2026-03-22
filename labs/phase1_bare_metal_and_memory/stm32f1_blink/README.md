## Blinking LED 

### Overview

- A bare-metal blinking LED firmware for ARM, flashed and tested using Renode.

### Hardware/Platform

- MCU: `STM32F103 ARM Cortex-M3`
- Simulated on Renode with `stm32f103` platform
- GPIO: PA5 toggled via BSRR register
- Flash: 0x08000000, RAM: 0x20000000

### Project Structure
```
    blink/
    ├── linker.ld
    ├── main.c
    ├── makefile
    ├── setup.resc
    ├── startup.c
    ├── README.md
    └── build/
```
### How to Run

```bash
make
renode setup.resc
```
- In Renode:
```
sysbus LogPeripheralAccess sysbus.gpioPortA true
start
```

### What I Learned 

- Direct register manipulation without HAL abstractions
- Setting up and running Renode simulation for STM32
- Reading peripheral access logs to verify hardware behavior

