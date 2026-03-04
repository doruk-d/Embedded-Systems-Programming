# lang rules
CC:= arm-none-eabi-gcc

MCU_FLAGS:= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb 
CFLAGS:= $(MCU_FLAGS) -Wall -O0 -g -I/usr/local/include/cmsis/CMSIS/Core/Include/ 
LDFLAGS:= $(MCU_FLAGS) -T linker.ld -nostartfiles --specs=nano.specs --specs=nosys.specs
DEPFLAGS:= -MMD -MP

# files
BUILD_DIR:= build

TARGET:= $(BUILD_DIR)/my_malloc.elf


SRC:= my_malloc.c test_driver.c
OBJS:= $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS:= $(OBJS:%.o=%.d) 

#rules
all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@ 

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< -c -o $@ 

# mkdir step
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)


-include $(DEPS)

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program $(TARGET) verify reset exit"

.PHONY: all  clean
