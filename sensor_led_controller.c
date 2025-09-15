// sensor_led_controller.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define IRS90_DEVICE    "/dev/irs90_all"
#define LED_DEVICE      "/dev/led_all"
#define BUFFER_SIZE     16
#define SLEEP_INTERVAL  100000  // 100ms in microseconds

// 全域變數
static volatile int running = 1;
static int irs_fd = -1;
static int led_fd = -1;

/**
 * @brief 信號處理函數 (Ctrl+C)
 */
void signal_handler(int sig)
{
    printf("\n收到信號 %d，正在退出...\n", sig);
    running = 0;
}

/**
 * @brief 初始化設備
 */
int init_devices(void)
{
    // 開啟 IRS-90 感測器設備
    irs_fd = open(IRS90_DEVICE, O_RDONLY);
    if (irs_fd < 0) {
        perror("無法開啟 IRS-90 設備");
        printf("請確認:\n");
        printf("1. 驅動程式已載入: lsmod | grep irs_90a\n");
        printf("2. 設備節點存在: ls -l %s\n", IRS90_DEVICE);
        printf("3. 權限正確: sudo chmod 666 %s\n", IRS90_DEVICE);
        return -1;
    }

    // 開啟 LED 控制設備
    led_fd = open(LED_DEVICE, O_WRONLY);
    if (led_fd < 0) {
        perror("無法開啟 LED 設備");
        printf("請確認:\n");
        printf("1. 驅動程式已載入: lsmod | grep led\n");
        printf("2. 設備節點存在: ls -l %s\n", LED_DEVICE);
        printf("3. 權限正確: sudo chmod 666 %s\n", LED_DEVICE);
        close(irs_fd);
        return -1;
    }

    printf("設備初始化成功:\n");
    printf("  - IRS-90 感測器: %s (fd=%d)\n", IRS90_DEVICE, irs_fd);
    printf("  - LED 控制器: %s (fd=%d)\n", LED_DEVICE, led_fd);

    return 0;
}

/**
 * @brief 清理資源
 */
void cleanup_devices(void)
{
    if (irs_fd >= 0) {
        close(irs_fd);
        irs_fd = -1;
    }
    
    if (led_fd >= 0) {
        // 關閉所有 LED
        write(led_fd, "000\n", 4);
        close(led_fd);
        led_fd = -1;
    }
    
    printf("設備已關閉，LED 已關閉\n");
}

/**
 * @brief 讀取感測器狀態
 */
int read_sensors(char *sensor_status)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    // 重設檔案位置
    lseek(irs_fd, 0, SEEK_SET);
    
    // 讀取感測器資料
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(irs_fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            printf("警告: 感測器無資料可讀 (EOF)\n");
        } else {
            perror("讀取感測器失敗");
        }
        return -1;
    }
    
    // 移除換行符
    if (buffer[bytes_read - 1] == '\n') {
        buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }
    
    // 驗證資料格式
    if (bytes_read < 3) {
        printf("錯誤: 感測器資料格式無效 '%s' (長度: %zd)\n", buffer, bytes_read);
        return -1;
    }
    
    // 複製感測器狀態
    strncpy(sensor_status, buffer, 3);
    sensor_status[3] = '\0';
    
    return 0;
}

/**
 * @brief 設置 LED 狀態
 */
int set_leds(const char *led_status)
{
    char command[8];
    ssize_t bytes_written;
    
    // 準備命令字串
    snprintf(command, sizeof(command), "%s\n", led_status);
    
    // 寫入 LED 控制命令
    bytes_written = write(led_fd, command, strlen(command));
    
    if (bytes_written != (ssize_t)strlen(command)) {
        perror("設置 LED 失敗");
        return -1;
    }
    
    return 0;
}

/**
 * @brief 感測器到 LED 的邏輯映射
 */
void sensor_to_led_logic(const char *sensor_status, char *led_status)
{
    // 策略 1: 直接映射 (感測器狀態 = LED 狀態)
    strcpy(led_status, sensor_status);
    
    /* 
    // 策略 2: 反向映射 (有物體時 LED 關閉)
    for (int i = 0; i < 3; i++) {
        led_status[i] = (sensor_status[i] == '1') ? '0' : '1';
    }
    led_status[3] = '\0';
    */
    
    /*
    // 策略 3: 特殊邏輯映射
    if (strcmp(sensor_status, "000") == 0) {
        strcpy(led_status, "000");  // 無物體，全關
    } else if (strcmp(sensor_status, "111") == 0) {
        strcpy(led_status, "111");  // 全偵測，全開
    } else if (strcmp(sensor_status, "010") == 0) {
        strcpy(led_status, "010");  // 中央偵測，只開中央
    } else {
        strcpy(led_status, "101");  // 其他情況，開兩側
    }
    */
}

/**
 * @brief 顯示狀態
 */
void display_status(const char *sensor_status, const char *led_status)
{
    time_t now;
    struct tm *timeinfo;
    char timestamp[32];
    
    // 獲取時間戳
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    
    printf("[%s] 感測器: %s -> LED: %s | ", timestamp, sensor_status, led_status);
    
    // 解釋感測器狀態
    if (strcmp(sensor_status, "000") == 0) {
        printf("無物體偵測");
    } else if (strcmp(sensor_status, "111") == 0) {
        printf("前方有物體");
    } else if (strcmp(sensor_status, "100") == 0) {
        printf("左側有物體");
    } else if (strcmp(sensor_status, "010") == 0) {
        printf("中央有物體");
    } else if (strcmp(sensor_status, "001") == 0) {
        printf("右側有物體");
    } else if (strcmp(sensor_status, "110") == 0) {
        printf("左中有物體");
    } else if (strcmp(sensor_status, "011") == 0) {
        printf("中右有物體");
    } else if (strcmp(sensor_status, "101") == 0) {
        printf("左右有物體");
    } else {
        printf("未知狀態");
    }
    
    printf("\n");
}

/**
 * @brief 主函數
 */
int main(int argc, char *argv[])
{
    char sensor_status[4];
    char led_status[4];
    char prev_sensor_status[4] = "";
    int error_count = 0;
    int cycle_count = 0;
    
    printf("===========================================\n");
    printf("IRS-90 感測器 LED 控制器\n");
    printf("===========================================\n");
    printf("功能: 讀取感測器狀態並控制對應 LED\n");
    printf("按 Ctrl+C 停止程式\n");
    printf("===========================================\n");
    
    // 設置信號處理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 初始化設備
    if (init_devices() < 0) {
        return EXIT_FAILURE;
    }
    
    // 初始狀態：關閉所有 LED
    if (set_leds("000") < 0) {
        printf("警告: 無法設置初始 LED 狀態\n");
    } else {
        printf("初始狀態: 所有 LED 已關閉\n");
    }
    
    printf("\n開始監控...\n");
    
    // 主循環
    while (running) {
        // 讀取感測器狀態
        if (read_sensors(sensor_status) < 0) {
            error_count++;
            if (error_count > 10) {
                printf("錯誤: 連續讀取失敗超過 10 次，退出程式\n");
                break;
            }
            usleep(SLEEP_INTERVAL * 2);  // 錯誤時等待更長時間
            continue;
        }
        
        error_count = 0;  // 重設錯誤計數
        cycle_count++;
        
        // 只在感測器狀態改變時更新 LED 和顯示
        if (strcmp(sensor_status, prev_sensor_status) != 0) {
            // 計算對應的 LED 狀態
            sensor_to_led_logic(sensor_status, led_status);
            
            // 設置 LED
            if (set_leds(led_status) < 0) {
                printf("警告: 設置 LED 失敗\n");
            }
            
            // 顯示狀態
            display_status(sensor_status, led_status);
            
            // 記錄當前狀態
            strcpy(prev_sensor_status, sensor_status);
        } else if (cycle_count % 100 == 0) {
            // 每 100 個週期顯示一次心跳
            printf(".");
            fflush(stdout);
        }
        
        // 等待下一個週期
        usleep(SLEEP_INTERVAL);
    }
    
    printf("\n程式結束\n");
    
    // 清理資源
    cleanup_devices();
    
    return EXIT_SUCCESS;
}
