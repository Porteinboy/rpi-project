// led_driver.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include "led_hal.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Triple LED Driver for Raspberry Pi");
MODULE_VERSION("1.0");

#define DEVICE_NAME     "led_all"
#define CLASS_NAME      "led_driver"
#define DEVICE_COUNT    1

static int major_number;
static struct class *led_class = NULL;
static struct device *led_device = NULL;
static struct cdev led_cdev;
static dev_t dev_number;
static DEFINE_MUTEX(led_mutex);

static int led_open(struct inode *inode, struct file *file);
static int led_release(struct inode *inode, struct file *file);
static ssize_t led_read(struct file *file, char __user *buffer, size_t length, loff_t *offset);
static ssize_t led_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset);

static struct file_operations led_fops = {
    .open = led_open,
    .release = led_release,
    .read = led_read,
    .write = led_write,
    .owner = THIS_MODULE,
};

static int led_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED Driver: Device opened\n");
    return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED Driver: Device closed\n");
    return 0;
}

static ssize_t led_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    char led_status[8];
    int len;
    int ret;
    
    if (*offset > 0) {
        return 0;
    }
    
    mutex_lock(&led_mutex);
    
    
    ret = led_hal_get_all_leds(led_status);
    if (ret < 0) {
        mutex_unlock(&led_mutex);
        printk(KERN_ERR "LED Driver: Failed to get LED status\n");
        return ret;
    }
    
   
    strcat(led_status, "\n");
    len = strlen(led_status);
    
    
    if (length < len) {
        mutex_unlock(&led_mutex);
        printk(KERN_WARNING "LED Driver: User buffer too small\n");
        return -EINVAL;
    }
    
    
    if (copy_to_user(buffer, led_status, len)) {
        mutex_unlock(&led_mutex);
        printk(KERN_ERR "LED Driver: Failed to copy data to user\n");
        return -EFAULT;
    }
    
    *offset += len;
    mutex_unlock(&led_mutex);
    
    printk(KERN_DEBUG "LED Driver: Read LED status: %s", led_status);
    return len;
}

static ssize_t led_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    char *kernel_buffer;
    char led_command[8];
    int i, ret;
    size_t actual_length;
    
   
    if (length > 16) {
        printk(KERN_WARNING "LED Driver: Write length too large\n");
        return -EINVAL;
    }
    
  
    kernel_buffer = kzalloc(length + 1, GFP_KERNEL);
    if (!kernel_buffer) {
        printk(KERN_ERR "LED Driver: Failed to allocate memory\n");
        return -ENOMEM;
    }
    
    
    if (copy_from_user(kernel_buffer, buffer, length)) {
        kfree(kernel_buffer);
        printk(KERN_ERR "LED Driver: Failed to copy data from user\n");
        return -EFAULT;
    }
    
    mutex_lock(&led_mutex);
    
  
    actual_length = length;
    while (actual_length > 0 && 
           (kernel_buffer[actual_length - 1] == '\n' || 
            kernel_buffer[actual_length - 1] == '\r' || 
            kernel_buffer[actual_length - 1] == ' ')) {
        kernel_buffer[actual_length - 1] = '\0';
        actual_length--;
    }
    
   
    if (actual_length < LED_COUNT) {
        mutex_unlock(&led_mutex);
        kfree(kernel_buffer);
        printk(KERN_ERR "LED Driver: Invalid command length %zu, expected at least %d\n", 
               actual_length, LED_COUNT);
        return -EINVAL;
    }
    
  
    strncpy(led_command, kernel_buffer, LED_COUNT);
    led_command[LED_COUNT] = '\0';
    
   
    for (i = 0; i < LED_COUNT; i++) {
        if (led_command[i] != '0' && led_command[i] != '1') {
            mutex_unlock(&led_mutex);
            kfree(kernel_buffer);
            printk(KERN_ERR "LED Driver: Invalid character '%c' at position %d\n", 
                   led_command[i], i);
            return -EINVAL;
        }
    }
    
  
    ret = led_hal_set_all_leds(led_command);
    if (ret < 0) {
        mutex_unlock(&led_mutex);
        kfree(kernel_buffer);
        printk(KERN_ERR "LED Driver: Failed to set LED status\n");
        return ret;
    }
    
    mutex_unlock(&led_mutex);
    kfree(kernel_buffer);
    
    printk(KERN_INFO "LED Driver: LED status set to: %s\n", led_command);
    return length;
}


static int __init led_driver_init(void)
{
    int ret;
    
    printk(KERN_INFO "LED Driver: Initializing Triple LED Driver\n");
    
    
    ret = led_hal_init();
    if (ret < 0) {
        printk(KERN_ERR "LED Driver: Failed to initialize LED HAL\n");
        return ret;
    }
    
    
    ret = alloc_chrdev_region(&dev_number, 0, DEVICE_COUNT, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "LED Driver: Failed to allocate device number\n");
        led_hal_cleanup();
        return ret;
    }
    major_number = MAJOR(dev_number);
    
    printk(KERN_INFO "LED Driver: Allocated major number %d\n", major_number);
    
   
    cdev_init(&led_cdev, &led_fops);
    led_cdev.owner = THIS_MODULE;
    
   
    ret = cdev_add(&led_cdev, dev_number, DEVICE_COUNT);
    if (ret < 0) {
        printk(KERN_ERR "LED Driver: Failed to add character device\n");
        unregister_chrdev_region(dev_number, DEVICE_COUNT);
        led_hal_cleanup();
        return ret;
    }
    
   
    led_class = class_create(CLASS_NAME);
    if (IS_ERR(led_class)) {
        printk(KERN_ERR "LED Driver: Failed to create device class\n");
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_number, DEVICE_COUNT);
        led_hal_cleanup();
        return PTR_ERR(led_class);
    }
    
   
    led_device = device_create(led_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(led_device)) {
        printk(KERN_ERR "LED Driver: Failed to create device\n");
        class_destroy(led_class);
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_number, DEVICE_COUNT);
        led_hal_cleanup();
        return PTR_ERR(led_device);
    }
    
    printk(KERN_INFO "LED Driver: Device /dev/%s created successfully\n", DEVICE_NAME);
    printk(KERN_INFO "LED Driver: Triple LED Driver initialized successfully\n");
    printk(KERN_INFO "LED Driver: Usage - echo '111' > /dev/%s to turn all LEDs on\n", DEVICE_NAME);
    printk(KERN_INFO "LED Driver: Usage - echo '000' > /dev/%s to turn all LEDs off\n", DEVICE_NAME);
    
    return 0;
}

static void __exit led_driver_exit(void)
{
    printk(KERN_INFO "LED Driver: Cleaning up Triple LED Driver\n");
    
    
    if (led_device) {
        device_destroy(led_class, dev_number);
    }
    
   
    if (led_class) {
        class_destroy(led_class);
    }
    
    
    cdev_del(&led_cdev);
    
    
    unregister_chrdev_region(dev_number, DEVICE_COUNT);
    
    
    led_hal_cleanup();
    
    printk(KERN_INFO "LED Driver: Triple LED Driver cleanup completed\n");
}


module_init(led_driver_init);
module_exit(led_driver_exit);
