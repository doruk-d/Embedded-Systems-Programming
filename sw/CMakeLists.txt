cmake_minimum_required(VERSION 3.31)
project(firmware LANGUAGES C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(arm_m4_config)
include(warning_config)

add_subdirectory(bsp/stm32f4x)
add_subdirectory(core/mem/allocator)
add_subdirectory(core/scheduler)
add_subdirectory(drivers/uart)
add_subdirectory(regs)
add_subdirectory(include)

add_executable(firmware
               app/main.c
               bsp/stm32f4x/startup.c)

target_link_libraries(firmware PRIVATE
                      bsp_stm32f4x
                      allocator
                      scheduler
                      uart
                      stm32_regs
                      common_hdrs)

target_link_options(firmware PRIVATE
                    -T${CMAKE_SOURCE_DIR}/bsp/stm32f4x/linker.ld)

set_target_properties(firmware PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/bsp/stm32f4x/linker.ld)

add_custom_target(flash
                  COMMAND openocd -f interface/stlink.cfg
                          -f target/stm32f4x.cfg
                          -c "init" -c "reset halt"
                          -c "program $<TARGET_FILE:firmware> verify reset exit"
                  DEPENDS firmware
                  COMMENT "flashing to the board...")

add_custom_command(TARGET firmware POST_BUILD
                   COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware>)
