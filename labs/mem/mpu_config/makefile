CC := arm-none-eabi-gcc

INCLUDES := -I../../boot/stm32f4x/

MCUFLAGS := -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
CFLAGS := $(MCUFLAGS) -Wall -O0 -g $(INCLUDES)
LDFLAGS := $(MCUFLAGS) -T ../../boot/stm32f4x/linker.ld -nostartfiles --specs=nano.specs --specs=nosys.specs
DEPFLAGS := -MMD -MP

BUILD_DIR := build

TARGET := $(BUILD_DIR)/test_mpu_config.elf

vpath %.c ../../boot/stm32f4x/ ../../tests/test_mpu/

SRC := mpu_config.c test_mpu.c startup.c fault_handlers.c system_clock.c
OBJS := $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< -c -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program $(TARGET) verify reset exit"

.PHONY: all clean flash
