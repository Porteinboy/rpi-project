# Makefile for logic layer

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2
EXTRA_CFLAGS += -I$(PWD)/../include

TARGET = test_irs90_all
SOURCE = test_irs90_all.c

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(SOURCE)

# Default target
all: $(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)

# Run program (require sudo)
run: $(TARGET)
	sudo ./$(TARGET)
