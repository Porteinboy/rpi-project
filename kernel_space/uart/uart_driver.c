// uart_driver.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "uart_driver.h"
#include "uart_hal.h"

static int major;

static int uart_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "uart_driver: device opened\n");
    return 0;
}

static ssize_t uart_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char c = uart_hal_read_char();
    if (copy_to_user(buf, &c, 1))
	return -EFAULT;
    return 1;
}

static ssize_t uart_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char c;
    if (copy_from_user(&c, buf, 1))
	return -EFAULT;
    uart_hal_write_char(c);
    return 1;
}

struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .open = uart_open,
    .read = uart_read,
    .write = uart_write,
};

int uart_driver_init(void) {
    int ret = uart_hal_init();
    if (ret)
        return ret;

    major = register_chrdev(0, "my_uart", &uart_fops);
    if (major < 0) {
        uart_hal_exit();
	return major;
    }

    printk(KERN_INFO "uart_driver: loaded, major=%d\n", major);
    return 0;
}

void uart_driver_exit(void) {
    unregister_chrdev(major, "my_uart");
    uart_hal_exit();
    printk(KERN_INFO "uart_driver: unloaded");
}

module_init(uart_driver_init)
module_exit(uart_driver_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");
MODULE_DESCRIPTION("BCM2711 UART driver");
