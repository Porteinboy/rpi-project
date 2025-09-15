// irs90a_hal.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include "register_map.h"
#include "pin_mapping.h"
#include "irs_90_hal.h"

static void __iomem *gpio_base;

static void gpio20_set_input(void){
    unsigned int val = readl(gpio_base + GPIO20_FSEL);
    val &= ~(0x7 << GPIO20_BIT_SHIFT);
    writel(val, gpio_base + GPIO20_FSEL);
}

static int __init irs_90a_init(void){
    gpio_base = ioremap(GPIO_BASE, 0xB0);
    if(!gpio_base) {
        printk(KERN_ERR "IRS90a HAL: ioremap failed\n");
	return -ENOMEM;
    }

    gpio20_set_input();

    printk(KERN_INFO "IRS90a HAL Initialized\n");
    return 0;
}

static void __exit irs_90a_exit(void){
    iounmap(gpio_base);
    printk(KERN_INFO "IRS90a HAL exited\n");
}

module_init(irs_90a_init);
module_exit(irs_90a_exit);

int read_gpio(void){
  
    // 將腳位讀到的值，回傳給driver層   	
    unsigned int val = readl(gpio_base + GPLEV0);
    return (val >> 20 ) & 0x001;
    
}

EXPORT_SYMBOL(read_gpio);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");

