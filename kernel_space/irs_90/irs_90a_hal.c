// irs90a_hal.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include "register_map.h"
#include "pin_mapping.h"
#include "irs_90_hal.h"

static void __iomem *gpio_base;

// 感測器 GPIO 配置表
static struct {
    int gpio_pin;
    unsigned int fsel_offset;
    unsigned int bit_shift;
    unsigned int level_reg;
    const char* name;
} sensor_config[IRS_90_MAX] = {
    {IRS_90A, GPFSEL1, GPIO16_BIT_SHIFT, GPLEV0, "IRS_90A"}, // 使用 GPFSEL1 (0x04)
    {IRS_90B, GPFSEL2, GPIO20_BIT_SHIFT, GPLEV0, "IRS_90B"}, // 使用 GPFSEL2 (0x08)
    {IRS_90C, GPFSEL2, GPIO21_BIT_SHIFT, GPLEV0, "IRS_90C"}, // 使用 GPFSEL2 (0x08)
};

// 設定指定感測器的 GPIO 為輸入模式
static void gpio_set_input(irs_90_sensor_t sensor) {
    unsigned int val;

    if (sensor >= IRS_90_MAX) {
    	printk(KERN_ERR "IRS90 HAL: Invalid sensor ID %d\n", sensor);
	return;
    }

    val = readl(gpio_base + sensor_config[sensor].fsel_offset);
    val &= ~(0x7 << sensor_config[sensor].bit_shift); // 清除對應的 3 bits (設為 000 = input)
    writel(val, gpio_base + sensor_config[sensor].fsel_offset);

    printk(KERN_INFO "IRS90 HAL: %s (GPIO%d) set as input\n", 
		    sensor_config[sensor].name, sensor_config[sensor].gpio_pin);
}

// 模組初始化
static int __init irs_90_hal_init(void){
    int i;
    printk(KERN_INFO "IRS90 HAL: Initializing triple sensor HAL\n");
    
    // 映射 GPIO 基址
    gpio_base = ioremap(GPIO_BASE, 0xB0);
    if(!gpio_base) {
        printk(KERN_ERR "IRS90 HAL: ioremap failed\n");
	return -ENOMEM;
    }

    for (i = 0; i < IRS_90_MAX; i++) {
        gpio_set_input(i);
    }

    printk(KERN_INFO "IRS90 HAL: All sensors initialized successfully\n");
    return 0;
}

static void __exit irs_90_hal_exit(void){
    if (gpio_base) {
	iounmap(gpio_base);
    }
    printk(KERN_INFO "IRS90 HAL: Module exited\n");
}

// 初始化指定感測器
int irs_90_init(irs_90_sensor_t sensor) {
    if(sensor >= IRS_90_MAX) {
    	printk(KERN_ERR "IRS90 HAL: Invalid sensor ID %d\n", sensor);
	return -EINVAL;
    }

    if (!gpio_base) {
    	printk(KERN_ERR "IRS90 HAL: GPIO base not mapped\n");
	return -ENODEV;
    }

    gpio_set_input(sensor);
    return 0;
}
EXPORT_SYMBOL(irs_90_init);

// 讀取指定感測器狀態
int irs_90_read(irs_90_sensor_t sensor) {

    unsigned int val;
    int gpio_pin;
    
    if (sensor >= IRS_90_MAX) {
        printk(KERN_ERR "IRS90 HAL: Invalid sensor ID %d\n", sensor);
	return -EINVAL;
    }

    if (!gpio_base) {
    	printk(KERN_ERR "IRS90 HAL: GPIO base not mapped\n");
	return -ENODEV;
    }

    gpio_pin = sensor_config[sensor].gpio_pin;
    val = readl(gpio_base + GPLEV0);

    // 提取對應 GPIO 的位元值
    int gpio_state = (val >> gpio_pin) & 0x1;

    // IRS-90 是低電位觸發，反轉邏輯
    return gpio_state ? IRS_90_NO_OBJECT : IRS_90_OBJECT_DETECT;
}
EXPORT_SYMBOL(irs_90_read);

// 讀取所有感測器狀態
int irs_90_read_all(irs_90_status_t status_array[]) {

    int i;
    
    if (!status_array) {
        printk(KERN_ERR "IRS90 HAL: Null status array\n");
	return -EINVAL;
    }

    if (!gpio_base) {
    	printk(KERN_ERR "IRS90 HAL: GPIO base not mapped\n");
	return -ENODEV;
    }

    for (i = 0; i < IRS_90_MAX; i++){
    	int result = irs_90_read(i);
	if (result < 0) {
            return result;
	}
	status_array[i] = result;
    }

    return 0;
}
EXPORT_SYMBOL(irs_90_read_all);

// 清理指定感測器
int irs_90_cleanup(irs_90_sensor_t sensor) {

    if (sensor >= IRS_90_MAX) {
        printk(KERN_ERR "IRS90 HAL: Invalid sensor ID %d\n", sensor);
	return -EINVAL;
    }

    printk(KERN_INFO "IRS90 HAL: %s cleanup completed\n",
		    sensor_config[sensor].name);
    return 0;
}
EXPORT_SYMBOL(irs_90_cleanup);

// 清理所有感測器
int irs_90_cleanup_all(void) {

    int i;

    for (i = 0; i < IRS_90_MAX; i++){
    	irs_90_cleanup(i);
    }

    return 0;
}
EXPORT_SYMBOL(irs_90_cleanup_all);

// 獲取感測器對應的 GPIO 腳位號
int irs_90_get_gpio_pin(irs_90_sensor_t sensor) {

    if (sensor >= IRS_90_MAX) {
    	printk(KERN_ERR "IRS90 HAL: Invalid sensor ID %d\n", sensor);
        return -EINVAL;
    }

    return sensor_config[sensor].gpio_pin;
}
EXPORT_SYMBOL(irs_90_get_gpio_pin);

// 向後相容函數 (給你原來的 driver 使用)
int read_gpio(void) {

    // 預設讀取 IRS_90B (GPIO20)，保持向後相容
    return irs_90_read(IRS_90B);
}
EXPORT_SYMBOL(read_gpio);

module_init(irs_90_hal_init);
module_exit(irs_90_hal_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");

