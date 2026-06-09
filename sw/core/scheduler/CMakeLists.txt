add_library(scheduler OBJECT
            src/context_switch.S
            src/measure_gpio.c
            src/scheduler.c
            src/svc.c
            src/systick.c)

target_include_directories(scheduler PUBLIC include/)
target_link_libraries(scheduler PRIVATE stm32_regs common_hdrs)
