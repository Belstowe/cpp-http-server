TARGET_EXEC := ./bin/http-server
BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-pthread

all: $(SRCS) $(TARGET_EXEC)
	
$(TARGET_EXEC): $(OBJS) 
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean
clean:
	rm -r ./build