// test_irs90_all.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main() {
    printf("IRS-90 Triple Sensor Test Started\n");
    printf("Format: A B C (0=No Object, 1=Object Detected)\n");
    printf("Press Ctrl+C to exit\n\n");
    
    while (1) {
        
    	// 讀取三感測器設備
    	int fd = open("/dev/irs90_all", O_RDONLY);
    	// printf("fd = %d\n", fd);
    	if(fd < 0){
            perror("open /dev/irs90_all");
	    usleep(500000);
	    continue;
        }
	    
	char buf[20];  // 修正：改為字符緩衝區，容納 "000\n" 格式
        memset(buf, 0, sizeof(buf));
        
        int ret = read(fd, buf, sizeof(buf)-1);
        printf("ret = %d\n", ret);
	
	// 立刻關閉檔案
	close(fd);
	
	if (ret > 0) {
            // 移除換行符
            if (buf[ret-1] == '\n') {
                buf[ret-1] = '\0';
            }
            
            // 解析三個感測器的狀態
            if (strlen(buf) >= 3) {
                int sensor_a = buf[0] - '0';
                int sensor_b = buf[1] - '0';
                int sensor_c = buf[2] - '0';
                
                printf("Sensors: A=%d B=%d C=%d -> ", sensor_a, sensor_b, sensor_c);
                
                // 邏輯判斷和狀態顯示
                if (sensor_a == 0 && sensor_b == 0 && sensor_c == 0) {
                    printf("All clear - No objects detected\n");
                } else if (sensor_a == 1 && sensor_b == 1 && sensor_c == 1) {
                    printf("All sensors triggered - Object in front\n");
                } else if (sensor_a == 1 && sensor_b == 0 && sensor_c == 0) {
                    printf("Left sensor only - Object on left side\n");
                } else if (sensor_a == 0 && sensor_b == 1 && sensor_c == 0) {
                    printf("Center sensor only - Object in center\n");
                } else if (sensor_a == 0 && sensor_b == 0 && sensor_c == 1) {
                    printf("Right sensor only - Object on right side\n");
                } else if (sensor_a == 1 && sensor_b == 1 && sensor_c == 0) {
                    printf("Left + Center - Object on left-center\n");
                } else if (sensor_a == 0 && sensor_b == 1 && sensor_c == 1) {
                    printf("Center + Right - Object on center-right\n");
                } else if (sensor_a == 1 && sensor_b == 0 && sensor_c == 1) {
                    printf("Left + Right - Objects on both sides\n");
                } else {
                    printf("Unknown pattern\n");
                }
                
                fflush(stdout);
            } else {
                printf("Invalid data format: %s\n", buf);
            }
        }
        else if (ret == 0) {
            printf("No data available (EOF)\n");
        }
        else if (ret < 0 && errno != EAGAIN) {
            perror("read");
            break;
        }
        
        usleep(500000);  // 0.5 秒間隔
    }
    
    return 0;
}
