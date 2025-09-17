// uart_hal.h
#ifndef  UART_HAL_H
#define  UART_HAL_H

#include <linux/types.h>

#define UART0_BASE 0xFE201000
#define UART0_DR   0x00
#define UART0_FR   0x18
#define UART0_CR   0x30

int uart_hal_init(void);
void uart_hal_exit(void);
char uart_hal_read_char(void);
void uart_hal_write_char(char c);

#endif //  UART_HAL_H

