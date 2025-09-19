# MQTT 通訊測試 Makefile

CC = gcc
CFLAGS = -Wall -O2 -I$(PWD)/../include
LIBS = -lmosquitto

# Project structure
BIN_DIR = ../../bin
OBJ_DIR = ../../build/obj

TARGET = $(BIN_DIR)/irs90_mqtt_publisher
SOURCES = irs90_mqtt_publisher.c mqtt_client.c
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Link .o -> executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile .c -> .o
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure directories exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Run program (require sudo)
run: $(TARGET)
	sudo $(TARGET)
