CC:= arm-none-eabi-gcc

INCLUDES:= -Iinclude/ \
           -I../drivers/uart/include/ \
		   -I../boot/stm32f4x/ 

MCU_FLAGS:= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
C_FLAGS:= $(MCU_FLAGS) -Wall -O0 -g $(INCLUDES)
LD_FLAGS:= $(MCU_FLAGS) -T ../boot/stm32f4x/linker.ld -nostartfiles --specs=nano.specs --specs=nosys.specs
DEP_FLAGS:= -MMD -MP

BUILD_DIR:= build
UART_LIB:= ../drivers/uart/build/libuart.a

vpath %.c src/ ../boot/stm32f4x/ ../tests/test_scheduler/
vpath %.S src/

TARGET:= $(BUILD_DIR)/scheduler.elf
SRCS:= test_scheduler.c startup.c mpu_config.c system_clock.c dwt.c  scheduler.c  svc.c  systick.c fault_handlers.c measure_gpio.c
ASM_SRCS:= context_switch.S
OBJS:= $(SRCS:%.c=$(BUILD_DIR)/%.o)
ASM_OBJS:= $(ASM_SRCS:%.S=$(BUILD_DIR)/%.o)
DEPS:= $(OBJS:.o=.d)
ASM_DEPS:= $(ASM_OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(ASM_OBJS) $(UART_LIB) | $(BUILD_DIR)
	$(CC) $(LD_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(DEP_FLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(DEP_FLAGS) $< -c -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
-include $(ASM_DEPS)

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program $(TARGET) verify reset exit"

.PHONY: all clean flash
