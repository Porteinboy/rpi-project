# Makefile for logic layer

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2
EXTRA_CFLAGS += -I$(PWD)/../include

# Paths
BIN_DIR = ../../bin
TARGET = $(BIN_DIR)/sensor_led_controller
SOURCE = sensor_led_controller.c

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

run: $(TARGET)
	sudo $<
