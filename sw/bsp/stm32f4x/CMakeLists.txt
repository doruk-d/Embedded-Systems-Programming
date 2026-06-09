add_library(bsp_stm32f4x STATIC
            system_clock.c
            mpu_config.c
            fault_handlers.c)

target_include_directories(bsp_stm32f4x PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bsp_stm32f4x PRIVATE stm32_regs)
