// IRS-90 Infrared Distance Sensor Hardware Abstraction Layer (HAL)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>        // insmod, readl, writel
#include <linux/delay.h>     // udelay, mdelay
#include "pin_mapping.h"     // GPIO register offset definitions (datasheet)
#include "register_map.h"    // GPIO register project definitions

// Kernel virtual address
static void __iomem *gpio_base;

// Set GPIO20 as input pin
static void gpio20_set_input(void){
    unsigned int val = readl(gpio_base + GPIO20_FSEL);    // Read GPFSEL2
    val &= ~(0x7 << GPIO20_BIT_SHIFT);                    // Clear corresponding 3 bits
    writel(val, gpio_base + GPIO20_FSEL);                 // Write back => set as input
}

// Module initialization
static int __init irs90_init(void){
    // 1. ioremap GPIO BASE to get virtual address
    gpio_base = ioremap(GPIO_BASE, 0xB0);
    if(!gpio_base) {
        printk(KERN_ERR "IRS-90 HAL: ioremap failed\n");
        return -ENOMEM;
    }
    
    // 2. Configure sensor GPIO pins
    gpio20_set_input();      // Set 20 pin as input
    
    // 3. Initialization complete
    printk(KERN_INFO "IRS-90 HAL initialized\n");
    return 0;
}

// Module unload
static void __exit irs90_exit(void){
    // Release resources
    iounmap(gpio_base);
    // Unload successful
    printk(KERN_INFO "IRS-90 HAL exited\n");
}

// Kernel Module entry/exit points
module_init(irs90_init);        // Called when insmod irs90_hal.ko is executed
module_exit(irs90_exit);        // Called when rmmod irs90_hal is executed

// Function for Device Driver to read GPIO pin values
int read_gpio(int gpio){
    unsigned int val, bit;
    // GPIO 0~31 read GPLEV0 register input values, GPIO 32~53 read GPLEV1 register values
    val = readl(gpio_base + GPLEV0);
    bit = (val >> 20) & 0x1;        // Get GPIO20 bit
    return bit;
    }
}

// Export for use by other kernel modules
EXPORT_SYMBOL(read_gpio);
EXPORT_SYMBOL(write_gpio);
EXPORT_SYMBOL(measure_distance);

// License and configuration
MODULE_LICENSE("GPL");                     // GPL license to avoid being considered proprietary
MODULE_AUTHOR("Sampson");                      // Author
MODULE_DESCRIPTION("IRS-90 HAL Layer");    // Brief description
