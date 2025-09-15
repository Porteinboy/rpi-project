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

// �����ܼ�
static volatile int running = 1;
static int irs_fd = -1;
static int led_fd = -1;

/**
 * @brief �H���B�z��� (Ctrl+C)
 */
void signal_handler(int sig)
{
    printf("\n����H�� %d�A���b�h�X...\n", sig);
    running = 0;
}

/**
 * @brief ��l�Ƴ]��
 */
int init_devices(void)
{
    // �}�� IRS-90 �P�����]��
    irs_fd = open(IRS90_DEVICE, O_RDONLY);
    if (irs_fd < 0) {
        perror("�L�k�}�� IRS-90 �]��");
        printf("�нT�{:\n");
        printf("1. �X�ʵ{���w���J: lsmod | grep irs_90a\n");
        printf("2. �]�Ƹ`�I�s�b: ls -l %s\n", IRS90_DEVICE);
        printf("3. �v�����T: sudo chmod 666 %s\n", IRS90_DEVICE);
        return -1;
    }

    // �}�� LED ����]��
    led_fd = open(LED_DEVICE, O_WRONLY);
    if (led_fd < 0) {
        perror("�L�k�}�� LED �]��");
        printf("�нT�{:\n");
        printf("1. �X�ʵ{���w���J: lsmod | grep led\n");
        printf("2. �]�Ƹ`�I�s�b: ls -l %s\n", LED_DEVICE);
        printf("3. �v�����T: sudo chmod 666 %s\n", LED_DEVICE);
        close(irs_fd);
        return -1;
    }

    printf("�]�ƪ�l�Ʀ��\:\n");
    printf("  - IRS-90 �P����: %s (fd=%d)\n", IRS90_DEVICE, irs_fd);
    printf("  - LED ���: %s (fd=%d)\n", LED_DEVICE, led_fd);

    return 0;
}

/**
 * @brief �M�z�귽
 */
void cleanup_devices(void)
{
    if (irs_fd >= 0) {
        close(irs_fd);
        irs_fd = -1;
    }
    
    if (led_fd >= 0) {
        // �����Ҧ� LED
        write(led_fd, "000\n", 4);
        close(led_fd);
        led_fd = -1;
    }
    
    printf("�]�Ƥw�����ALED �w����\n");
}

/**
 * @brief Ū���P�������A
 */
int read_sensors(char *sensor_status)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    // ���]�ɮצ�m
    lseek(irs_fd, 0, SEEK_SET);
    
    // Ū���P�������
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(irs_fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            printf("ĵ�i: �P�����L��ƥiŪ (EOF)\n");
        } else {
            perror("Ū���P��������");
        }
        return -1;
    }
    
    // ���������
    if (buffer[bytes_read - 1] == '\n') {
        buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }
    
    // ���Ҹ�Ʈ榡
    if (bytes_read < 3) {
        printf("���~: �P������Ʈ榡�L�� '%s' (����: %zd)\n", buffer, bytes_read);
        return -1;
    }
    
    // �ƻs�P�������A
    strncpy(sensor_status, buffer, 3);
    sensor_status[3] = '\0';
    
    return 0;
}

/**
 * @brief �]�m LED ���A
 */
int set_leds(const char *led_status)
{
    char command[8];
    ssize_t bytes_written;
    
    // �ǳƩR�O�r��
    snprintf(command, sizeof(command), "%s\n", led_status);
    
    // �g�J LED ����R�O
    bytes_written = write(led_fd, command, strlen(command));
    
    if (bytes_written != (ssize_t)strlen(command)) {
        perror("�]�m LED ����");
        return -1;
    }
    
    return 0;
}

/**
 * @brief �P������ LED ���޿�M�g
 */
void sensor_to_led_logic(const char *sensor_status, char *led_status)
{
    // ���� 1: �����M�g (�P�������A = LED ���A)
    strcpy(led_status, sensor_status);
    
    /* 
    // ���� 2: �ϦV�M�g (������� LED ����)
    for (int i = 0; i < 3; i++) {
        led_status[i] = (sensor_status[i] == '1') ? '0' : '1';
    }
    led_status[3] = '\0';
    */
    
    /*
    // ���� 3: �S���޿�M�g
    if (strcmp(sensor_status, "000") == 0) {
        strcpy(led_status, "000");  // �L����A����
    } else if (strcmp(sensor_status, "111") == 0) {
        strcpy(led_status, "111");  // �������A���}
    } else if (strcmp(sensor_status, "010") == 0) {
        strcpy(led_status, "010");  // ���������A�u�}����
    } else {
        strcpy(led_status, "101");  // ��L���p�A�}�ⰼ
    }
    */
}

/**
 * @brief ��ܪ��A
 */
void display_status(const char *sensor_status, const char *led_status)
{
    time_t now;
    struct tm *timeinfo;
    char timestamp[32];
    
    // ����ɶ��W
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    
    printf("[%s] �P����: %s -> LED: %s | ", timestamp, sensor_status, led_status);
    
    // �����P�������A
    if (strcmp(sensor_status, "000") == 0) {
        printf("�L���鰻��");
    } else if (strcmp(sensor_status, "111") == 0) {
        printf("�e�観����");
    } else if (strcmp(sensor_status, "100") == 0) {
        printf("����������");
    } else if (strcmp(sensor_status, "010") == 0) {
        printf("����������");
    } else if (strcmp(sensor_status, "001") == 0) {
        printf("�k��������");
    } else if (strcmp(sensor_status, "110") == 0) {
        printf("����������");
    } else if (strcmp(sensor_status, "011") == 0) {
        printf("���k������");
    } else if (strcmp(sensor_status, "101") == 0) {
        printf("���k������");
    } else {
        printf("�������A");
    }
    
    printf("\n");
}

/**
 * @brief �D���
 */
int main(int argc, char *argv[])
{
    char sensor_status[4];
    char led_status[4];
    char prev_sensor_status[4] = "";
    int error_count = 0;
    int cycle_count = 0;
    
    printf("===========================================\n");
    printf("IRS-90 �P���� LED ���\n");
    printf("===========================================\n");
    printf("�\��: Ū���P�������A�ñ������ LED\n");
    printf("�� Ctrl+C ����{��\n");
    printf("===========================================\n");
    
    // �]�m�H���B�z
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // ��l�Ƴ]��
    if (init_devices() < 0) {
        return EXIT_FAILURE;
    }
    
    // ��l���A�G�����Ҧ� LED
    if (set_leds("000") < 0) {
        printf("ĵ�i: �L�k�]�m��l LED ���A\n");
    } else {
        printf("��l���A: �Ҧ� LED �w����\n");
    }
    
    printf("\n�}�l�ʱ�...\n");
    
    // �D�`��
    while (running) {
        // Ū���P�������A
        if (read_sensors(sensor_status) < 0) {
            error_count++;
            if (error_count > 10) {
                printf("���~: �s��Ū�����ѶW�L 10 ���A�h�X�{��\n");
                break;
            }
            usleep(SLEEP_INTERVAL * 2);  // ���~�ɵ��ݧ���ɶ�
            continue;
        }
        
        error_count = 0;  // ���]���~�p��
        cycle_count++;
        
        // �u�b�P�������A���ܮɧ�s LED �M���
        if (strcmp(sensor_status, prev_sensor_status) != 0) {
            // �p������� LED ���A
            sensor_to_led_logic(sensor_status, led_status);
            
            // �]�m LED
            if (set_leds(led_status) < 0) {
                printf("ĵ�i: �]�m LED ����\n");
            }
            
            // ��ܪ��A
            display_status(sensor_status, led_status);
            
            // �O����e���A
            strcpy(prev_sensor_status, sensor_status);
        } else if (cycle_count % 100 == 0) {
            // �C 100 �Ӷg����ܤ@���߸�
            printf(".");
            fflush(stdout);
        }
        
        // ���ݤU�@�Ӷg��
        usleep(SLEEP_INTERVAL);
    }
    
    printf("\n�{������\n");
    
    // �M�z�귽
    cleanup_devices();
    
    return EXIT_SUCCESS;
}
