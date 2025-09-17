// uart_driver.h
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <linux/types.h>
#include <linux/fs.h>

int uart_driver_init(void);
void uart_driver_exit(void);

extern struct file_operations uart_fops;

#endif // UART_DRIVER_H
