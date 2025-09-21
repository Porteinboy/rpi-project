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
#define SLEEP_INTERVAL  100000  // 100ms

// Global variables
static volatile int running = 1;
static int led_fd = -1;

/**
 * @brief Signal handler for Ctrl+C
 */
void signal_handler(int sig)
{
    printf("\nReceived signal %d, exiting...\n", sig);
    running = 0;
}

/**
 * @brief Initialize devices
 */
int init_devices(void)
{
    // 測試 IRS-90 感測器
    int test_fd = open(IRS90_DEVICE, O_RDONLY);
    if (test_fd < 0) {
        perror("Cannot open IRS-90 device");
        return -1;
    }
    close(test_fd);

    // 打開 LED 裝置
    led_fd = open(LED_DEVICE, O_WRONLY);
    if (led_fd < 0) {
        perror("Cannot open LED device");
        return -1;
    }

    printf("Device initialization successful:\n");
    printf("  - IRS-90 sensor: %s\n", IRS90_DEVICE);
    printf("  - LED controller: %s (fd=%d)\n", LED_DEVICE, led_fd);
    return 0;
}

/**
 * @brief Cleanup resources
 */
void cleanup_devices(void)
{
    if (led_fd >= 0) {
        // Turn off all LEDs
        write(led_fd, "000\n", 4);
        close(led_fd);
        led_fd = -1;
    }
    printf("Devices closed, LEDs turned off\n");
}

/**
 * @brief Read sensor status
 */
int read_sensors(char *sensor_status)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int temp_fd;

    temp_fd = open(IRS90_DEVICE, O_RDONLY);
    if (temp_fd < 0) {
        perror("Failed to open sensor device");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(temp_fd, buffer, sizeof(buffer) - 1);
    close(temp_fd);

    if (bytes_read <= 0) {
        printf("Failed to read sensor data\n");
        return -1;
    }

    if (buffer[bytes_read - 1] == '\n') {
        buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }

    if (bytes_read < 3) {
        printf("Invalid sensor data: '%s'\n", buffer);
        return -1;
    }

    strncpy(sensor_status, buffer, 3);
    sensor_status[3] = '\0';

    return 0;
}

/**
 * @brief Set LED status
 */
int set_leds(const char *led_status)
{
    char command[8];
    ssize_t bytes_written;

    snprintf(command, sizeof(command), "%s\n", led_status);
    bytes_written = write(led_fd, command, strlen(command));

    if (bytes_written != (ssize_t)strlen(command)) {
        perror("Failed to set LEDs");
        return -1;
    }
    return 0;
}

/**
 * @brief Display status
 */
void display_status(const char *sensor_status, const char *led_status)
{
    time_t now;
    struct tm *timeinfo;
    char timestamp[32];

    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);

    printf("[%s] Sensor: %s -> LED: %s\n", timestamp, sensor_status, led_status);
}

/**
 * @brief Main function
 */
int main(void)
{
    char sensor_status[4];
    char led_status[4];
    char prev_sensor_status[4] = "";
    int error_count = 0;
    int cycle_count = 0;

    printf("===========================================\n");
    printf("IRS-90 Sensor LED Controller\n");
    printf("Function: Read sensor status and control LEDs\n");
    printf("===========================================\n");

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (init_devices() < 0) {
        return EXIT_FAILURE;
    }

    // 初始全滅
    set_leds("000");
    printf("Initial state: All LEDs off\n");

    while (running) {
        if (read_sensors(sensor_status) < 0) {
            error_count++;
            if (error_count > 10) {
                printf("Error: Failed to read sensors 10 times consecutively, exiting\n");
                break;
            }
            usleep(SLEEP_INTERVAL * 2);
            continue;
        }

        error_count = 0;
        cycle_count++;

        if (strcmp(sensor_status, prev_sensor_status) != 0) {
            strcpy(led_status, sensor_status);  // 直接對應
            if (set_leds(led_status) == 0) {
                display_status(sensor_status, led_status);
            }
            strcpy(prev_sensor_status, sensor_status);
        } else if (cycle_count % 100 == 0) {
            printf(".");
            fflush(stdout);
        }

        usleep(SLEEP_INTERVAL);
    }

    cleanup_devices();
    return EXIT_SUCCESS;
}
