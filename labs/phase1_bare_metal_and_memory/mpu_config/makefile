CC := arm-none-eabi-gcc

INCLUDES := -I../../shared

MCUFLAGS := -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb
CFLAGS := $(MCUFLAGS) -Wall -O0 -g $(INCLUDES)
LDFLAGS := $(MCUFLAGS) -T ../../shared/linker.ld -nostartfiles --specs=nano.specs --specs=nosys.specs
DEPFLAGS := -MMD -MP

BUILD_DIR := build

TARGET := $(BUILD_DIR)/mpu_config.elf

vpath %.c ../../shared

SRC := mpu_config.c test_mpu.c startup.c fault_handlers.c system_clock.c
OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.c=.o)))
DEPS := $(OBJS:%.o=%.d)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $< -c -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

-include $(DEPS)

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "init" -c "reset halt" -c "program $(TARGET) verify reset exit"

.PHONY: all clean flash
