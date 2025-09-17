// test_uart_14_15.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// BCM2711 physical addresses
#define BCM2711_PERI_BASE   0xFE000000
#define GPIO_BASE           (BCM2711_PERI_BASE + 0x200000)
#define UART0_BASE          (BCM2711_PERI_BASE + 0x201000)

// GPIO register offsets
#define GPFSEL1             1
#define GPPUD               37
#define GPPUDCLK0           38

// UART0 register offsets
#define UART0_DR            0
#define UART0_FR            6
#define UART0_IBRD          9
#define UART0_FBRD          10
#define UART0_LCRH          11
#define UART0_CR            12

// UART status flags
#define UART_FR_TXFF        (1 << 5)  // TX FIFO full
#define UART_FR_TXFE        (1 << 7)  // TX FIFO empty

// Global variables
volatile unsigned int *gpio;
volatile unsigned int *uart;

// Initialize memory mapping
int init_peripherals() {
    int mem_fd;
    
    // Open /dev/mem
    mem_fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (mem_fd < 0) {
        printf("Error: Cannot open /dev/mem (need root permission)\n");
        return -1;
    }
    
    // Map GPIO
    gpio = (volatile unsigned int *)mmap(
        NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, GPIO_BASE);
    
    if (gpio == MAP_FAILED) {
        printf("Error: GPIO mmap failed\n");
        close(mem_fd);
        return -1;
    }
    
    // Map UART0
    uart = (volatile unsigned int *)mmap(
        NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, UART0_BASE);
    
    if (uart == MAP_FAILED) {
        printf("Error: UART mmap failed\n");
        munmap((void *)gpio, 4096);
        close(mem_fd);
        return -1;
    }
    
    close(mem_fd);
    return 0;
}

// Setup GPIO 14, 15 for UART mode
void setup_gpio_uart() {
    // Set GPIO 14, 15 to ALT0 (UART0 TXD0, RXD0)
    // GPFSEL1 controls GPIO 10-19
    unsigned int sel = gpio[GPFSEL1];
    
    // Clear GPIO 14, 15 settings (3 bits per GPIO)
    sel &= ~(7 << 12);  // GPIO 14 (bits 14-12)
    sel &= ~(7 << 15);  // GPIO 15 (bits 17-15)
    
    // Set to ALT0 (100)
    sel |= (4 << 12);   // GPIO 14 = ALT0
    sel |= (4 << 15);   // GPIO 15 = ALT0
    
    gpio[GPFSEL1] = sel;
    
    // Setup pull-up/pull-down
    gpio[GPPUD] = 0;           // Disable pull-up/pull-down
    usleep(150);
    gpio[GPPUDCLK0] = (1 << 14) | (1 << 15);  // Clock GPIO 14, 15
    usleep(150);
    gpio[GPPUDCLK0] = 0;       // Remove clock
}

// Initialize UART0
void setup_uart() {
    // Disable UART
    uart[UART0_CR] = 0;
    
    // Set baud rate to 115200
    // Formula: BAUDDIV = FUARTCLK/(16 * Baud rate)
    // FUARTCLK = 48MHz, Baud = 115200
    // BAUDDIV = 48000000/(16 * 115200) = 26.0416...
    // IBRD = 26, FBRD = 0.0416 * 64 + 0.5 = 3
    uart[UART0_IBRD] = 26;
    uart[UART0_FBRD] = 3;
    
    // 8N1: 8 data bits, no parity, 1 stop bit, enable FIFO
    uart[UART0_LCRH] = (3 << 5) | (1 << 4);  // 8 bits + FIFO enable
    
    // Enable UART, TX, RX
    uart[UART0_CR] = (1 << 0) | (1 << 8) | (1 << 9);  // UARTEN | TXE | RXE
}

// Send single character
void uart_putc(char c) {
    // Wait for TX FIFO to have space
    while (uart[UART0_FR] & UART_FR_TXFF) {
        usleep(1);
    }
    uart[UART0_DR] = c;
}

// Send string
void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

// Wait for transmission complete
void uart_flush() {
    // Wait for TX FIFO empty
    while (!(uart[UART0_FR] & UART_FR_TXFE)) {
        usleep(1);
    }
}

// Clean up resources
void cleanup() {
    if (gpio != MAP_FAILED) {
        munmap((void *)gpio, 4096);
    }
    if (uart != MAP_FAILED) {
        munmap((void *)uart, 4096);
    }
}

int main() {
    printf("BCM2711 UART Test Program\n");
    printf("GPIO14 = TX, GPIO15 = RX\n");
    printf("Baud Rate: 115200, 8N1\n\n");
    
    // Initialize peripherals
    if (init_peripherals() < 0) {
        return 1;
    }
    
    printf("Setting up GPIO...\n");
    setup_gpio_uart();
    
    printf("Setting up UART...\n");
    setup_uart();
    
    printf("Starting data transmission test...\n");
    printf("Check PuTTY output (115200 8N1)\n\n");
    
    // Send test data
    for (int i = 0; i < 10; i++) {
        char buffer[100];
        sprintf(buffer, "Test Message #%d - BCM2711 UART GPIO14/15 Test\r\n", i + 1);
        uart_puts(buffer);
        uart_flush();
        
        printf("Sent: %s", buffer);
        sleep(1);
    }
    
    // ASCII test
    uart_puts("\r\n=== ASCII Test ===\r\n");
    for (char c = '!'; c <= '~'; c++) {
        uart_putc(c);
        if ((c - '!') % 16 == 15) {
            uart_puts("\r\n");
        }
    }
    uart_puts("\r\n");
    
    // Number test
    uart_puts("\r\n=== Number Test ===\r\n");
    for (int i = 0; i < 100; i += 7) {
        char num_buffer[50];
        sprintf(num_buffer, "Number: %d\r\n", i);
        uart_puts(num_buffer);
        usleep(100000);  // 0.1 second delay
    }
    
    uart_puts("\r\n=== Test Complete ===\r\n");
    uart_flush();
    
    printf("\nTest Complete!\n");
    
    cleanup();
    return 0;
}
