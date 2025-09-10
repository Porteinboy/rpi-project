// IRS-90 Device Driver Layer
#include <linux/module.h>    // Kernel module basic functions
#include <linux/fs.h>        // File system related functions
#include <linux/uaccess.h>   // User space and kernel space data exchange
#include "pin_mapping.h"     // GPIO pin mapping definitions
#include "irs90_hal.h"       // IRS-90 Hardware Abstraction Layer interface

static int major;           // Global major number (main device number)

// ---------- File Operation Functions -------------

/**
 * irs90_open - Handle device open operation
 * @inode: inode structure pointer, contains device information
 * @file: file structure pointer, contains information about opened file
 * 
 * Called when user program executes open("/dev/irs90", ...)
 * Return: 0 for success, negative number for error code
 */
static int irs90_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "irs90: device opened\n");  // Log device open event
    return 0;  // Success
}

/**
 * irs90_read - Handle device read operation
 * @file: file structure pointer
 * @buf: user space buffer pointer for receiving data
 * @count: number of bytes user wants to read
 * @ppos: file position pointer
 * 
 * Called when user program executes read() or cat /dev/irs90
 * Return: actual bytes read, 0 for EOF, negative for error
 */
static ssize_t irs90_read(struct file *file, char __user *buf, size_t count, loff_t *ppos ){
    
    // 1. Measure distance using IRS-90 sensor
    // measure_distance() returns distance in cm, or -1 for error
    long distance = measure_distance();
    
    // Log distance measurement result to kernel log
    if(distance >= 0) {
        printk(KERN_INFO "IRS-90 Distance: %ld cm", distance);
    } else {
        printk(KERN_WARNING "IRS-90 Distance measurement failed");
    }
    
    // 2. Format sensor value as string, format: "distance_cm\n"
    // Example: "25\n" means 25cm distance detected
    char msg[20];  // Declare string buffer
    int len;
    if(distance >= 0) {
        len = snprintf(msg, sizeof(msg), "%ld\n", distance);
    } else {
        len = snprintf(msg, sizeof(msg), "ERROR\n");
    }
    
    // 3. Prevent repeated reading mechanism
    // When file position > 0 or user buffer too small, return 0 for EOF
    // This ensures each cat command only reads data once
    if(*ppos > 0 || count < len) return 0;
    
    // 4. Copy data from kernel space to user space
    // copy_to_user() safely copies data from kernel to user program
    if(copy_to_user(buf, msg, len)) {
        return -EFAULT;  // Memory access error
    }
    
    *ppos = len;   // Update file read position
    return len;    // Return actual number of characters read
}

/**
 * irs90_release - Handle device close operation
 * @inode: inode structure pointer
 * @file: file structure pointer
 * 
 * Called when user program executes close()
 * Return: 0 for success
 */
static int irs90_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "irs90: device closed\n");  // Log device close event
    return 0;  // Success
}

/**
 * file_operations structure
 * Defines the file operation function mapping table for this character device
 * When user programs perform various operations on /dev/irs90,
 * kernel calls corresponding handler functions based on this table
 */
static struct file_operations fops = {
    .owner = THIS_MODULE,       // Specify module owner, prevent module unload while in use
    .open = irs90_open,         // Corresponds to open() system call
    .read = irs90_read,         // Corresponds to read() system call
    .release = irs90_release,   // Corresponds to close() system call
};

// ------------ Module Initialization and Cleanup -------------

/**
 * irs90_driver_init - Module initialization function
 * 
 * Automatically called when loading module with insmod
 * Responsible for registering character device and obtaining major device number
 * Return: 0 for success, negative for failure
 */
static int __init irs90_driver_init(void){
    
    // Register character device to kernel
    // Parameters: major(0 means let kernel auto-assign), device name, file operations table
    major = register_chrdev(0, "irs90", &fops);
    
    // Check if registration successful
    if(major < 0){
        printk(KERN_ALERT "Register char device failed\n");  // Registration failed
        return major;  // Return error code
    }
    
    // Registration successful, log obtained major device number
    // User needs to create device node with this number: mknod /dev/irs90 c <major> 0
    printk(KERN_INFO "IRS-90 driver loaded, major number: %d\n", major);
    return 0;  // Initialization successful
}

/**
 * irs90_driver_exit - Module cleanup function
 * 
 * Automatically called when unloading module with rmmod
 * Responsible for unregistering character device and releasing system resources
 */
static void __exit irs90_driver_exit(void){
    
    // Unregister character device
    // Parameters: major device number, device name
    unregister_chrdev(major, "irs90");
    printk(KERN_INFO "IRS-90 driver unloaded\n");  // Log module unload
}

// Macros to set module entry and exit points
module_init(irs90_driver_init);  // Specify initialization function for insmod
module_exit(irs90_driver_exit);  // Specify cleanup function for rmmod

// Module information macros (included in module during compilation)
MODULE_LICENSE("GPL");                      // License: GPL, avoid being considered proprietary
MODULE_AUTHOR("XIE");                       // Author information
MODULE_DESCRIPTION("IRS-90 Device Driver"); // Brief module description

/*
 * Usage Instructions:
 * 
 * 1. Compile module: make
 * 2. Load module: sudo insmod irs90_driver.ko
 * 3. Check major number: dmesg | tail
 * 4. Create device node: sudo mknod /dev/irs90 c <major_number> 0
 * 5. Set permissions: sudo chmod 666 /dev/irs90
 * 6. Read sensor: cat /dev/irs90
 * 7. Unload module: sudo rmmod irs90_driver
 * 
 * Output format: "distance_cm\n"
 * Example: "25" means 25cm distance detected
 * "ERROR" means measurement failed
 */
