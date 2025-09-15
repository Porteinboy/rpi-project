// led_hal.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/string.h>
#include "led_hal.h"
#include "pin_mapping.h"
#include "register_map.h"

 
#define GPIO_BASE_SIZE         0xB4

#define GPIO_OUTPUT            0x001


static void __iomem *gpio_base = NULL;
static int led_states[LED_COUNT] = {0}; // °O¿ýLEDª¬ºA


static const struct {
    int gpio_num;
    int bit_shift;
} led_config[LED_COUNT] = {
    {LEDA, GPIO13_BIT_SHIFT}, // LED A
    {LEDB, GPIO19_BIT_SHIFT}, // LED B
    {LEDC, GPIO26_BIT_SHIFT}  // LED C
};


int led_hal_init(void)
{
    uint32_t reg_val;
    int i;


    gpio_base = ioremap(GPIO_BASE, GPIO_BASE_SIZE);
    if (!gpio_base) {
        printk(KERN_ERR "LED HAL: Failed to map GPIO registers\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "LED HAL: GPIO registers mapped at %p\n", gpio_base);


    for (i = 0; i < LED_COUNT; i++) {
        switch (led_config[i].gpio_num) {
            case LEDA: // GPIO 13
                reg_val = ioread32(gpio_base + GPFSEL1);
                reg_val &= ~(0x7 << GPIO13_BIT_SHIFT); 
                reg_val |= (GPIO_OUTPUT << GPIO13_BIT_SHIFT);
                iowrite32(reg_val, gpio_base + GPFSEL1);
                printk(KERN_INFO "LED HAL: LED A (GPIO13) configured as output\n");
                break;

            case LEDB: // GPIO 19
                reg_val = ioread32(gpio_base + GPFSEL1);
                reg_val &= ~(0x7 << GPIO19_BIT_SHIFT);
                reg_val |= (GPIO_OUTPUT << GPIO19_BIT_SHIFT); 
                iowrite32(reg_val, gpio_base + GPFSEL1);
                printk(KERN_INFO "LED HAL: LED B (GPIO19) configured as output\n");
                break;

            case LEDC: // GPIO 26
                reg_val = ioread32(gpio_base + GPFSEL2);
                reg_val &= ~(0x7 << GPIO26_BIT_SHIFT); 
                reg_val |= (GPIO_OUTPUT << GPIO26_BIT_SHIFT); 
                iowrite32(reg_val, gpio_base + GPFSEL2);
                printk(KERN_INFO "LED HAL: LED C (GPIO26) configured as output\n");
                break;
        }

        
        led_states[i] = LED_OFF;
    }

    
    led_hal_turn_off_all();

    printk(KERN_INFO "LED HAL: All LEDs initialized and turned off\n");
    return 0;
}


void led_hal_cleanup(void)
{
    if (gpio_base) {
        
        led_hal_turn_off_all();

       
        iounmap(gpio_base);
        gpio_base = NULL;

        printk(KERN_INFO "LED HAL: Cleanup completed\n");
    }
}

int led_hal_set_led(int led_num, int state)
{
    uint32_t gpio_bit;

    if (led_num < 0 || led_num >= LED_COUNT) {
        printk(KERN_ERR "LED HAL: Invalid LED number %d\n", led_num);
        return -EINVAL;
    }

    if (state != LED_OFF && state != LED_ON) {
        printk(KERN_ERR "LED HAL: Invalid LED state %d\n", state);
        return -EINVAL;
    }

    if (!gpio_base) {
        printk(KERN_ERR "LED HAL: GPIO not initialized\n");
        return -ENODEV;
    }

    gpio_bit = 1 << led_config[led_num].gpio_num;

    if (state == LED_ON) {
        
        iowrite32(gpio_bit, gpio_base + GPSET0);
        led_states[led_num] = LED_ON;
        printk(KERN_DEBUG "LED HAL: LED %d turned ON\n", led_num);
    } else {
       
        iowrite32(gpio_bit, gpio_base + GPCLR0);
        led_states[led_num] = LED_OFF;
        printk(KERN_DEBUG "LED HAL: LED %d turned OFF\n", led_num);
    }

    return 0;
}

int led_hal_get_led(int led_num)
{
    if (led_num < 0 || led_num >= LED_COUNT) {
        printk(KERN_ERR "LED HAL: Invalid LED number %d\n", led_num);
        return -EINVAL;
    }

    return led_states[led_num];
}


int led_hal_set_all_leds(const char *states)
{
    int i, ret;

    if (!states || strlen(states) < LED_COUNT) {
        printk(KERN_ERR "LED HAL: Invalid states string\n");
        return -EINVAL;
    }

    for (i = 0; i < LED_COUNT; i++) {
        if (states[i] != '0' && states[i] != '1') {
            printk(KERN_ERR "LED HAL: Invalid character '%c' at position %d\n", states[i], i);
            return -EINVAL;
        }

        ret = led_hal_set_led(i, states[i] - '0');
        if (ret < 0) {
            return ret;
        }
    }

    printk(KERN_DEBUG "LED HAL: All LEDs set to %s\n", states);
    return 0;
}


int led_hal_get_all_leds(char *buffer)
{
    int i;

    if (!buffer) {
        printk(KERN_ERR "LED HAL: Null buffer\n");
        return -EINVAL;
    }

    for (i = 0; i < LED_COUNT; i++) {
        buffer[i] = led_states[i] + '0';
    }
    buffer[LED_COUNT] = '\0';

    return 0;
}


void led_hal_turn_off_all(void)
{
    int i;

    for (i = 0; i < LED_COUNT; i++) {
        led_hal_set_led(i, LED_OFF);
    }

    printk(KERN_INFO "LED HAL: All LEDs turned off\n");
}


void led_hal_turn_on_all(void)
{
    int i;

    for (i = 0; i < LED_COUNT; i++) {
        led_hal_set_led(i, LED_ON);
    }

    printk(KERN_INFO "LED HAL: All LEDs turned on\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");
MODULE_DESCRIPTION("LED HAL for Raspberry Pi");

// 導出符號供其他模組使用
EXPORT_SYMBOL(led_hal_init);
EXPORT_SYMBOL(led_hal_cleanup);
EXPORT_SYMBOL(led_hal_get_all_leds);
EXPORT_SYMBOL(led_hal_set_all_leds);
EXPORT_SYMBOL(led_hal_set_led);
EXPORT_SYMBOL(led_hal_get_led);
EXPORT_SYMBOL(led_hal_turn_off_all);
EXPORT_SYMBOL(led_hal_turn_on_all);
