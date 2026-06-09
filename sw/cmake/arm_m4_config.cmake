set(mcu_flags
   -mcpu=cortex-m4
   -mfloat-abi=soft
   -mthumb)

add_compile_options(${mcu_flags}
                    -ffunction-sections
                    -fdata-sections)

add_link_options(${mcu_flags}
                 -nostartfiles
                 -nostdlib
                 -Wl,-Map=$<TARGET_PROPERTY:NAME>.map
                 -Wl,--gc-sections)
