CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror \
		 -std=c11 -O2 -g \
		 -D_POSIX_C_SOURCE=200809L
DEPFLAGS := -MMD -MP


TARGET := test_driver
BUILD_DIR := build


SRCS := test_driver.c tracked_malloc.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $^ -o $@


$(BUILD_DIR)/%.o : %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


test: $(TARGET)
	./$(TARGET)


clean:
	rm -rf $(TARGET) $(BUILD_DIR)


-include $(DEPS)	


.PHONY: all test clean
	

