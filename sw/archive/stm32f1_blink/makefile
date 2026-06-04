CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -O0 -g
LDFLAGS = -T linker.ld -nostdlib


BUILD_DIR = build


TARGET = $(BUILD_DIR)/output.elf
SRCS = main.c startup.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)


all: $(TARGET)


$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $^ -o $@


$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


clean:
	rm -rf $(BUILD_DIR)


.PHONY: all clean


