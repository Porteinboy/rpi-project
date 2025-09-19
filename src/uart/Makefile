# output directory
BIN_DIR = ../../bin

TARGET1 = testr
TARGET2 = testw
TARGET3 = irs90_uart

# 最後輸出的檔案
TARGETS = $(BIN_DIR)/$(TARGET1) \
          $(BIN_DIR)/$(TARGET2) \
          $(BIN_DIR)/$(TARGET2)-line \
          $(BIN_DIR)/$(TARGET3)

CC = gcc
CFLAGS = -g -O2 -Wall

all: $(BIN_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/$(TARGET2)-line: $(TARGET2).c
	$(CC) $(CFLAGS) -D__LINE_BUF__ $< -o $@

$(BIN_DIR)/$(TARGET3): $(TARGET3).c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)
