// sensor_led_controller.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <termios.h>

#define IRS90_DEVICE    "/dev/irs90_all"
#define LED_DEVICE      "/dev/led_all"
#define UART_DEVICE     "/dev/serial0"     // GPIO14(TXD) & GPIO15(RXD)
#define BUFFER_SIZE     16
#define SLEEP_INTERVAL  100000  // 100ms in microseconds

// Global variables
static volatile int running = 1;
static int led_fd = -1;
static int uart_fd = -1;

/**
 * @brief Signal handler for Ctrl+C
 */
void signal_handler(int sig)
{
    printf("\nReceived signal %d, exiting...\n", sig);
    running = 0;
}

/**
 * @brief Initialize UART communication
 * GPIO14 = TXD (Transmit)
 * GPIO15 = RXD (Receive) 
 */
int init_uart(void)
{
    struct termios tty;

    // Open UART device
    uart_fd = open(UART_DEVICE, O_RDWR | O_NOCTTY);
    if (uart_fd < 0) {
        perror("Cannot open UART device");
        printf("Please check:\n");
        printf("1. UART enabled: sudo raspi-config -> Interface Options -> Serial Port\n");
        printf("2. Device exists: ls -l %s\n", UART_DEVICE);
        printf("3. Permissions: sudo chmod 666 %s\n", UART_DEVICE);
        printf("4. GPIO14(TXD) and GPIO15(RXD) properly configured\n");
        return -1;
    }

    // Get current terminal attributes
    if (tcgetattr(uart_fd, &tty) != 0) {
        perror("Failed to get UART attributes");
        close(uart_fd);
        uart_fd = -1;
        return -1;
    }

    // Configure UART settings
    // Baud rate: 9600
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    // 8 data bits, no parity, 1 stop bit (8N1)
    tty.c_cflag &= ~PARENB;        // No parity
    tty.c_cflag &= ~CSTOPB;        // 1 stop bit
    tty.c_cflag &= ~CSIZE;         // Clear data size bits
    tty.c_cflag |= CS8;            // 8 data bits
    tty.c_cflag &= ~CRTSCTS;       // No hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable reading, ignore modem control lines

    // Input flags
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // No software flow control
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Raw input

    // Output flags
    tty.c_oflag &= ~OPOST;  // Raw output

    // Control characters
    tty.c_cc[VMIN] = 0;     // Non-blocking read
    tty.c_cc[VTIME] = 1;    // 0.1 seconds read timeout

    // Apply settings
    if (tcsetattr(uart_fd, TCSANOW, &tty) != 0) {
        perror("Failed to set UART attributes");
        close(uart_fd);
        uart_fd = -1;
        return -1;
    }

    // Flush any existing data
    tcflush(uart_fd, TCIOFLUSH);

    printf("UART initialization successful: %s (fd=%d, 9600 8N1)\n", UART_DEVICE, uart_fd);
    printf("GPIO14(TXD) and GPIO15(RXD) ready for communication\n");
    return 0;
}

/**
 * @brief Send data via UART with enhanced error handling
 */
int send_uart(const char *data)
{
    ssize_t bytes_written;

    if (uart_fd < 0) {
        printf("Error: UART not initialized\n");
        return -1;
    }

    if (data == NULL || strlen(data) == 0) {
        printf("Error: Invalid UART data\n");
        return -1;
    }

    bytes_written = write(uart_fd, data, strlen(data));
    if (bytes_written < 0) {
        perror("Failed to send UART data");
        return -1;
    }

    // Ensure data is sent immediately
    tcdrain(uart_fd);

    printf("UART sent: \"%s\" (%zd bytes) via GPIO14(TXD)\n", data, bytes_written);
    return 0;
}

/**
 * @brief Initialize devices
 */
int init_devices(void)
{
    // Test IRS-90 sensor device (don't keep it open)
    int test_fd = open(IRS90_DEVICE, O_RDONLY);
    if (test_fd < 0) {
        perror("Cannot open IRS-90 device");
        printf("Please check:\n");
        printf("1. Driver loaded: lsmod | grep irs_90a\n");
        printf("2. Device exists: ls -l %s\n", IRS90_DEVICE);
        printf("3. Permissions: sudo chmod 666 %s\n", IRS90_DEVICE);
        return -1;
    }
    close(test_fd);

    // Open LED control device (keep it open)
    led_fd = open(LED_DEVICE, O_WRONLY);
    if (led_fd < 0) {
        perror("Cannot open LED device");
        printf("Please check:\n");
        printf("1. Driver loaded: lsmod | grep led\n");
        printf("2. Device exists: ls -l %s\n", LED_DEVICE);
        printf("3. Permissions: sudo chmod 666 %s\n", LED_DEVICE);
        return -1;
    }

    // Initialize UART
    if (init_uart() < 0) {
        close(led_fd);
        led_fd = -1;
        return -1;
    }

    printf("Device initialization successful:\n");
    printf("  - IRS-90 sensor: %s\n", IRS90_DEVICE);
    printf("  - LED controller: %s (fd=%d)\n", LED_DEVICE, led_fd);
    printf("  - UART: %s (fd=%d, GPIO14/15)\n", UART_DEVICE, uart_fd);

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

    if (uart_fd >= 0) {
        close(uart_fd);
        uart_fd = -1;
    }

    printf("Devices closed, LEDs turned off\n");
}

/**
 * @brief Read sensor status (reopen device each time)
 */
int read_sensors(char *sensor_status)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int temp_fd;

    // Reopen device file each time
    temp_fd = open(IRS90_DEVICE, O_RDONLY);
    if (temp_fd < 0) {
        perror("Failed to open sensor device");
        return -1;
    }

    // Read data
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(temp_fd, buffer, sizeof(buffer) - 1);

    // Close immediately
    close(temp_fd);

    if (bytes_read <= 0) {
        printf("Failed to read sensor data\n");
        return -1;
    }

    // Remove newline character
    if (buffer[bytes_read - 1] == '\n') {
        buffer[bytes_read - 1] = '\0';
        bytes_read--;
    }

    // Validate data format
    if (bytes_read < 3) {
        printf("Invalid sensor data format: '%s' (length: %zd)\n", buffer, bytes_read);
        return -1;
    }

    // Copy sensor status
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

    // Prepare command string
    snprintf(command, sizeof(command), "%s\n", led_status);

    // Write LED control command
    bytes_written = write(led_fd, command, strlen(command));

    if (bytes_written != (ssize_t)strlen(command)) {
        perror("Failed to set LEDs");
        return -1;
    }

    return 0;
}

/**
 * @brief Check for individual sensor activation and send UART commands
 */
void check_sensor_activation(const char *current_status)
{
    // Check if A sensor (left) detects object
    if (current_status[0] == '1') {
        static int a_sensor_active = 0;
        if (!a_sensor_active) {
            printf(">>> A sensor (Left) activated - ");
            send_uart("ZA");
            a_sensor_active = 1;
        }
    } else {
        static int a_sensor_active = 0;
        a_sensor_active = 0;
    }

    // Check if B sensor (center) detects object
    if (current_status[1] == '1') {
        static int b_sensor_active = 0;
        if (!b_sensor_active) {
            printf(">>> B sensor (Center) activated - ");
            send_uart("ZB");
            b_sensor_active = 1;
        }
    } else {
        static int b_sensor_active = 0;
        b_sensor_active = 0;
    }

    // Check if C sensor (right) detects object
    if (current_status[2] == '1') {
        static int c_sensor_active = 0;
        if (!c_sensor_active) {
            printf(">>> C sensor (Right) activated - ");
            send_uart("ZC");
            c_sensor_active = 1;
        }
    } else {
        static int c_sensor_active = 0;
        c_sensor_active = 0;
    }
}

/**
 * @brief Check sensor state transition and send UART commands (Original logic)
 */
void check_state_transition(const char *prev_status, const char *current_status)
{
    // Only send commands when transitioning from no detection (000) to detection
    if (strcmp(prev_status, "000") == 0) {
        if (strcmp(current_status, "100") == 0) {
            // Object detected on left sensor (A)
            printf(">>> State transition: 000->100 - ");
            send_uart("ZA");
        } else if (strcmp(current_status, "010") == 0) {
            // Object detected on center sensor (B)
            printf(">>> State transition: 000->010 - ");
            send_uart("ZB");
        } else if (strcmp(current_status, "001") == 0) {
            // Object detected on right sensor (C)
            printf(">>> State transition: 000->001 - ");
            send_uart("ZC");
        } else if (strcmp(current_status, "110") == 0) {
            // Multiple sensors - prioritize left
            printf(">>> State transition: 000->110 - ");
            send_uart("ZA");
        } else if (strcmp(current_status, "011") == 0) {
            // Multiple sensors - prioritize center
            printf(">>> State transition: 000->011 - ");
            send_uart("ZB");
        } else if (strcmp(current_status, "101") == 0) {
            // Multiple sensors - prioritize left
            printf(">>> State transition: 000->101 - ");
            send_uart("ZA");
        } else if (strcmp(current_status, "111") == 0) {
            // All sensors - prioritize center
            printf(">>> State transition: 000->111 - ");
            send_uart("ZB");
        }
    }
}

/**
 * @brief Sensor to LED mapping logic
 */
void sensor_to_led_logic(const char *sensor_status, char *led_status)
{
    // Strategy 1: Direct mapping (sensor state = LED state)
    strcpy(led_status, sensor_status);

    /*
    // Strategy 2: Inverse mapping (LED off when object detected)
    for (int i = 0; i < 3; i++) {
        led_status[i] = (sensor_status[i] == '1') ? '0' : '1';
    }
    led_status[3] = '\0';
    */

    /*
    // Strategy 3: Custom logic mapping
    if (strcmp(sensor_status, "000") == 0) {
        strcpy(led_status, "000");  // No objects, all off
    } else if (strcmp(sensor_status, "111") == 0) {
        strcpy(led_status, "111");  // All detected, all on
    } else if (strcmp(sensor_status, "010") == 0) {
        strcpy(led_status, "010");  // Center only
    } else {
        strcpy(led_status, "101");  // Other cases, sides on
    }
    */
}

/**
 * @brief Display status with enhanced information
 */
void display_status(const char *sensor_status, const char *led_status)
{
    time_t now;
    struct tm *timeinfo;
    char timestamp[32];

    // Get timestamp
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);

    printf("[%s] Sensor: %s -> LED: %s | ", timestamp, sensor_status, led_status);

    // Interpret sensor status
    if (strcmp(sensor_status, "000") == 0) {
        printf("A0B0C0");
    } else if (strcmp(sensor_status, "111") == 0) {
        printf("A1B1C1");
    } else if (strcmp(sensor_status, "100") == 0) {
        printf("A1B0C0");
    } else if (strcmp(sensor_status, "010") == 0) {
        printf("A0B1C0");
    } else if (strcmp(sensor_status, "001") == 0) {
        printf("A0B0C1");
    } else if (strcmp(sensor_status, "110") == 0) {
        printf("A1B1C0");
    } else if (strcmp(sensor_status, "011") == 0) {
        printf("A0B1C1");
    } else if (strcmp(sensor_status, "101") == 0) {
        printf("A1B0C1");
    } else {
        printf("Unknown pattern");
    }

    printf("\n");
}

/**
 * @brief Main function
 */
int main(int argc, char *argv[])
{
    char sensor_status[4];
    char led_status[4];
    char prev_sensor_status[4] = "";
    int error_count = 0;
    int cycle_count = 0;

    printf("===========================================\n");
    printf("IRS-90 Sensor LED Controller with UART\n");
    printf("Enhanced Version with Individual Sensor Detection\n");
    printf("===========================================\n");
    printf("Hardware Configuration:\n");
    printf("  GPIO14 (TXD) - UART Transmit\n");
    printf("  GPIO15 (RXD) - UART Receive\n");
    printf("\nFunction: Read sensor status, control LEDs, and send UART commands\n");
    printf("UART Commands sent when sensors activate:\n");
    printf("  A sensor (Left):   Send \"ZA\"\n");
    printf("  B sensor (Center): Send \"ZB\"\n");
    printf("  C sensor (Right):  Send \"ZC\"\n");
    printf("Press Ctrl+C to stop\n");
    printf("===========================================\n");

    // Setup signal handling
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Initialize devices
    if (init_devices() < 0) {
        return EXIT_FAILURE;
    }

    // Initial state: turn off all LEDs
    if (set_leds("000") < 0) {
        printf("Warning: Cannot set initial LED state\n");
    } else {
        printf("Initial state: All LEDs turned off\n");
    }

    printf("\nStarting monitoring...\n");

    // Main loop
    while (running) {
        // Read sensor status
        if (read_sensors(sensor_status) < 0) {
            error_count++;
            if (error_count > 10) {
                printf("Error: Failed to read sensors 10 times consecutively, exiting\n");
                break;
            }
            usleep(SLEEP_INTERVAL * 2);  // Wait longer on error
            continue;
        }

        error_count = 0;  // Reset error count
        cycle_count++;

        // Update LED and display only when sensor status changes
        if (strcmp(sensor_status, prev_sensor_status) != 0) {
            // Check for state transitions (original logic)
            check_state_transition(prev_sensor_status, sensor_status);

            // Also check for individual sensor activations (enhanced logic)
            // Uncomment the line below if you want individual sensor activation detection
            // check_sensor_activation(sensor_status);

            // Calculate corresponding LED status
            sensor_to_led_logic(sensor_status, led_status);

            // Set LEDs
            if (set_leds(led_status) < 0) {
                printf("Warning: Failed to set LEDs\n");
            }

            // Display status
            display_status(sensor_status, led_status);

            // Record current status
            strcpy(prev_sensor_status, sensor_status);
        } else if (cycle_count % 100 == 0) {
            // Show heartbeat every 100 cycles
            printf(".");
            fflush(stdout);
        }

        // Wait for next cycle
        usleep(SLEEP_INTERVAL);
    }

    printf("\nProgram terminated\n");

    // Cleanup resources
    cleanup_devices();

    return EXIT_SUCCESS;
}
