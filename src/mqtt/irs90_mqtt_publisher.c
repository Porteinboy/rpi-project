// irs90_mqtt_publisher.c
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "irs90_mqtt_publisher.h"

#define BUF_SIZE 32

int irs90_mqtt_run(void) {
    char buf[BUF_SIZE] = {0};
    char prev_buf[BUF_SIZE] = {0};

    // 初始化 MQTT (只需要處理一次)
    if(mqtt_init() != 0) {
        fprintf(stderr, "Failed to init MQTT\n");
	return -1;
    }

    while(1) {
        int fd = open(SENSOR_DEVICE, O_RDONLY);
        if ( fd < 0 ) {
            perror("open sensor device");
	    return -1;
        }

        ssize_t n = read(fd, buf, BUF_SIZE-1);
        close(fd); // 每次讀完立刻關閉檔案
	
	if (n > 0) {
	    buf[n] = '\0'; // 確保字串結尾
            // 發布資料到 MQTT
	    // 只在資料有變化的時候發送
	    if (strcmp(buf, prev_buf) != 0) { // 如果資料有變化
	        if (mqtt_publish(MQTT_TOPIC_PARKING, buf) == 0) {
		    printf("Published: %s\n", buf);
		} else {
		    fprintf(stderr, "Failed to publish data\n");
		}
		strcpy(prev_buf, buf); // 更新上一筆資料
	    }
	} else {
	    perror("read sensor");
	}

	usleep(50000); // 每0.5秒讀一次
    }

    // 關閉 MQTT
    mqtt_close();
    return 0;
}

// main function for standalone run
int main(void) {
    return irs90_mqtt_run();
}
