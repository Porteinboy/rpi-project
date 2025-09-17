// test_uart.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// BCM2711 physical addresses
#define BCM2711_PERI_BASE   0xFE000000
#define GPIO_BASE           (BCM2711_PERI_BASE + 0x200000)
#define UART1_BASE          (BCM2711_PERI_BASE + 0x215000)

// GPIO register offsets
#define GPFSEL0             0
#define GPFSEL1             1
#define GPPUD               37
#define GPPUDCLK0           38

// UART1 register offsets (Mini UART)
#define AUX_ENABLES         1
#define AUX_MU_IO_REG       16
#define AUX_MU_IER_REG      17
#define AUX_MU_IIR_REG      18
#define AUX_MU_LCR_REG      19
#define AUX_MU_MCR_REG      20
#define AUX_MU_LSR_REG      21
#define AUX_MU_MSR_REG      22
#define AUX_MU_SCRATCH      23
#define AUX_MU_CNTL_REG     24
#define AUX_MU_STAT_REG     25
#define AUX_MU_BAUD_REG     26

// UART status flags
#define AUX_MU_LSR_TX_EMPTY (1 << 5)  // TX empty
#define AUX_MU_LSR_TX_IDLE  (1 << 6)  // TX idle

// Global variables
volatile unsigned int *gpio;
volatile unsigned int *aux;

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
    
    // Map AUX (includes UART1/Mini UART)
    aux = (volatile unsigned int *)mmap(
        NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, UART1_BASE);
    
    if (aux == MAP_FAILED) {
        printf("Error: AUX mmap failed\n");
        munmap((void *)gpio, 4096);
        close(mem_fd);
        return -1;
    }
    
    close(mem_fd);
    return 0;
}

// Setup GPIO 8, 1 for UART mode
void setup_gpio_uart() {
    // Set GPIO 8, 1 to ALT3 (UART1 TXD, RXD)
    // GPFSEL0 controls GPIO 0-9
    unsigned int sel = gpio[GPFSEL0];
    
    // Clear GPIO 1, 8 settings (3 bits per GPIO)
    sel &= ~(7 << 3);   // GPIO 1 (bits 5-3)
    sel &= ~(7 << 24);  // GPIO 8 (bits 26-24)
    
    // Set to ALT3 (111)
    sel |= (7 << 3);    // GPIO 1 = ALT3 (RX)
    sel |= (7 << 24);   // GPIO 8 = ALT3 (TX)
    
    gpio[GPFSEL0] = sel;
    
    // Setup pull-up/pull-down
    gpio[GPPUD] = 0;           // Disable pull-up/pull-down
    usleep(150);
    gpio[GPPUDCLK0] = (1 << 1) | (1 << 8);  // Clock GPIO 1, 8
    usleep(150);
    gpio[GPPUDCLK0] = 0;       // Remove clock
}

// Initialize UART1 (Mini UART)
void setup_uart() {
    // Enable Mini UART (bit 0 of AUX_ENABLES)
    aux[AUX_ENABLES] |= 1;
    
    // Disable TX, RX
    aux[AUX_MU_CNTL_REG] = 0;
    
    // Disable interrupts
    aux[AUX_MU_IER_REG] = 0;
    
    // Clear FIFO
    aux[AUX_MU_IIR_REG] = 0xC6;
    
    // Set 8-bit mode
    aux[AUX_MU_LCR_REG] = 3;
    
    // Set RTS high
    aux[AUX_MU_MCR_REG] = 0;
    
    // Set baud rate to 115200
    // Mini UART baud rate = system_clock_freq / (8 * (baudrate_reg + 1))
    // System clock = 250MHz (default), so:
    // 115200 = 250000000 / (8 * (baudrate_reg + 1))
    // baudrate_reg = (250000000 / (8 * 115200)) - 1 = 270.267 - 1 = 269
    aux[AUX_MU_BAUD_REG] = 270;
    
    // Enable TX, RX
    aux[AUX_MU_CNTL_REG] = 3;
}

// Send single character
void uart_putc(char c) {
    // Wait for TX to be ready
    while (!(aux[AUX_MU_LSR_REG] & AUX_MU_LSR_TX_EMPTY)) {
        usleep(1);
    }
    aux[AUX_MU_IO_REG] = c;
}

// Send string
void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

// Wait for transmission complete
void uart_flush() {
    // Wait for TX idle
    while (!(aux[AUX_MU_LSR_REG] & AUX_MU_LSR_TX_IDLE)) {
        usleep(1);
    }
}

// Clean up resources
void cleanup() {
    if (gpio != MAP_FAILED) {
        munmap((void *)gpio, 4096);
    }
    if (aux != MAP_FAILED) {
        munmap((void *)aux, 4096);
    }
}

int main() {
    printf("BCM2711 UART1 Test Program\n");
    printf("GPIO8 = TX, GPIO1 = RX\n");
    printf("Baud Rate: 115200, 8N1\n\n");
    
    // Initialize peripherals
    if (init_peripherals() < 0) {
        return 1;
    }
    
    printf("Setting up GPIO...\n");
    setup_gpio_uart();
    
    printf("Setting up UART1...\n");
    setup_uart();
    
    printf("Starting data transmission test...\n");
    printf("Check PuTTY output (115200 8N1)\n\n");
    
    // Send test data
    for (int i = 0; i < 10; i++) {
        char buffer[100];
        sprintf(buffer, "Test Message #%d - BCM2711 UART1 GPIO8/1 Test\r\n", i + 1);
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
