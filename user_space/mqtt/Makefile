# MQTT 通訊測試

CC = gcc
CFLAGS = -Wall -O2
LIBS = -lmosquitto

# 目標檔案
TARGET = irs90_mqtt_publisher
SOURCE = irs90_mqtt_publisher.c mqtt_client.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	sudo ./$(TARGET)

clean:
	rm -f $(TARGET)
