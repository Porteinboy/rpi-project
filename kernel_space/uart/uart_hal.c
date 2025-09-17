// uart_hal.c
#include <linux/module.h> // 重要 必須加上
#include <linux/io.h>
#include <linux/kernel.h>
#include "uart_hal.h"

static void __iomem *uart_base;

int uart_hal_init(void) {
    uart_base = ioremap(UART0_BASE, 0x1000);

    if(!uart_base) {
        printk(KERN_ERR "uart_hal: ioremap failed\n");
	return -ENOMEM;
    }

    // 啟動 UART (簡單 demo)
    iowrite32(1, uart_base + UART0_CR);
    printk(KERN_INFO "uart_hal: initialized\n");
    return 0;
}

void uart_hal_exit(void) {
    iounmap(uart_base);
    printk(KERN_INFO "uart_hal: exited\n");
}

char uart_hal_read_char(void) {
    while(ioread32(uart_base + UART0_FR) & (1 << 4))
	cpu_relax();
    return ioread32(uart_base + UART0_DR) & 0xFF;
}

void uart_hal_write_char(char c) {
    while(ioread32(uart_base + UART0_FR) & (1 << 5))
	cpu_relax();
    iowrite32(c, uart_base + UART0_DR);
}

// 匯出給其他 module 使用
EXPORT_SYMBOL(uart_hal_init);
EXPORT_SYMBOL(uart_hal_exit);
EXPORT_SYMBOL(uart_hal_write_char);
EXPORT_SYMBOL(uart_hal_read_char);

// 模組授權
MODULE_LICENSE("GPL");


