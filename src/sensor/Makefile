# Makefile for logic layer (sensor test programs)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2
EXTRA_CFLAGS += -I$(PWD)/../include

# Project structure
BIN_DIR = ../../bin
TARGET = $(BIN_DIR)/test_irs90_all
SOURCE = test_irs90_all.c

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCE) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build files
clean:
	rm -f $(TARGET)

# Run program (require sudo)
run: $(TARGET)
	sudo $(TARGET)
