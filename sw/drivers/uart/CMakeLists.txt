add_library(uart STATIC src/uart.c)

target_include_directories(uart PUBLIC include/)
target_link_libraries(uart PRIVATE stm32_regs common_hdrs)
