TARGET1=testr
TARGET2=testw

all: $(TARGET1) $(TARGET2) $(TARGET2)-line

CC = gcc

CFLAGS = -g -O2 -Wall

$(TARGET1): $(TARGET1).c
	$(CC) $(TARGET1).c  -o $(TARGET1)

$(TARGET2): $(TARGET2).c
	$(CC) $(TARGET2).c  -o $(TARGET2)

$(TARGET2)-line:$(TARGET2).c
	$(CC) -D__LINE_BUF__ $(TARGET2).c  -o $(TARGET2)-line

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET2)-line
