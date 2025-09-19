TARGET1=testr
TARGET2=testw
TARGET3=irs90_uart

all: $(TARGET1) $(TARGET2) $(TARGET2)-line $(TARGET3)

CC = gcc

CFLAGS = -g -O2 -Wall

$(TARGET1): $(TARGET1).c
	$(CC) $(TARGET1).c  -o $(TARGET1)

$(TARGET2): $(TARGET2).c
	$(CC) $(TARGET2).c  -o $(TARGET2)

$(TARGET2)-line:$(TARGET2).c
	$(CC) -D__LINE_BUF__ $(TARGET2).c  -o $(TARGET2)-line

$(TARGET3): $(TARGET3).c
	$(CC) $(TARGET3).c  -o $(TARGET3)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET2)-line $(TARGET3)
