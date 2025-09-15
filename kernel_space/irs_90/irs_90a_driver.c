// irs-90a_driver.c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include "pin_mapping.h"
#include "irs_90_hal.h"

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int irs90_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "IRS90: device opened\n");
    return 0;
}

static int irs90_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "IRS90: device closed\n");
    return 0;
}

static ssize_t irs90_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    irs_90_status_t sensor_status[IRS_90_MAX];
    char msg[8];
    int len;
    int ret;

    // 如果已經讀過了，返回 0 表示 EOF
    if (*ppos > 0) return 0;

    // 讀取所有三個感測器的狀態
    ret = irs_90_read_all(sensor_status);
    if (ret < 0) {
    	printk(KERN_ERR "IRS90: Failed to read sensor, error: %d\n", ret);
	return ret;
    }
    
    // 格式化輸出為 "000" 或 "111" 等格式
    len = snprintf(msg, sizeof(msg), "%d%d%d\n", 
		    sensor_status[IRS_90_A],
		    sensor_status[IRS_90_B],
		    sensor_status[IRS_90_C]);
    
    printk(KERN_INFO "IRS90: Sensor data: A=%d, B=%d, C=%d\n",
		    sensor_status[IRS_90_A], 
		    sensor_status[IRS_90_B], 
		    sensor_status[IRS_90_C] 
		    );
    printk(KERN_DEBUG "IRS90: %s[%d] count: %zu, len:%d\n", 
		    __FUNCTION__, __LINE__, count, len);

    // 檢查緩衝區大小
    if (count < len) {
    	return -EINVAL;
    }

    // 複製資歷到user space
    if (copy_to_user(buf, msg, len)) return -EFAULT;

    *ppos = len;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = irs90_open,
    .release = irs90_release,
    .read = irs90_read,
};

static char *irs_90_devnode(const struct device *dev, umode_t *mode){
	if (mode) *mode = 0666;
	return NULL;
}

static int __init irs_90_driver_init(void)
{
    int ret;
    int i;

    printk(KERN_INFO "IRS_90: Initializing triple sensor driver\n");

    // 初始化所有感測器 HAL
    for (i = 0; i < IRS_90_MAX; i++) {
        ret = irs_90_init(i);
	if (ret < 0) {
	    printk(KERN_ERR "IRS90: Failed to initialize sensor %d, error: %d\n", 
			    i, ret);
	    // 清理已初始化的感測器
	    for (i = i - 1; i >= 0; i--) {
	    	irs_90_cleanup(i);
	    }
	    return ret;
	}
    }

    // 分配字符設備號
    ret = alloc_chrdev_region(&dev, 0, 1, "irs90_all");
    if (ret < 0) {
        printk(KERN_ALERT "IRS90: Failed to allocate char device region\n");
	irs_90_cleanup_all();
	return ret;
    }

    printk(KERN_INFO "IRS90: Major number allocated = %d\n", MAJOR(dev));
    printk(KERN_INFO "IRS90: Minor number = %d\n", MINOR(dev));
    
    // 初始化並添加字符設備
    cdev_init(&c_dev, &fops);
    c_dev.owner = THIS_MODULE;
    ret = cdev_add(&c_dev, dev, 1);
    if (ret < 0) {
    	unregister_chrdev_region(dev, 1);
	irs_90_cleanup_all();
	printk(KERN_ALERT "IRS90: Failed to add cdev\n");
	return ret;
    }
    
    // 創建設備類別
    cl = class_create("irs90_class");
    if (IS_ERR(cl)) {
        cdev_del(&c_dev);
	unregister_chrdev_region(dev , 1);
	irs_90_cleanup_all();
	printk(KERN_ALERT "IRS90: Failed to create class\n");
	return PTR_ERR(cl);
    }

    cl -> devnode = irs_90_devnode;

    // 創建設備節點 /dev/irs90_all
    if (IS_ERR(device_create(cl, NULL, dev, NULL, "irs90_all"))) {
        class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, 1);
	irs_90_cleanup_all();
	printk(KERN_ALERT "IRS90: Failed to create device\n");
	return PTR_ERR(cl);
    }

    printk(KERN_INFO "IRS90: Driver loaded successfully, device: /dev/irs90_all\n");
    printk(KERN_INFO "IRS90: Monitoring sensors - A:GPIO%d, B:GPIO%d, C:GPIO%d\n", 
		    irs_90_get_gpio_pin(IRS_90_A),
		    irs_90_get_gpio_pin(IRS_90_B),
		    irs_90_get_gpio_pin(IRS_90_C));

    return 0;
}


static void __exit irs_90_driver_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, 1);
    irs_90_cleanup_all();

    printk(KERN_INFO "irs90: driver unloaded successfully\n");
}

module_init(irs_90_driver_init);
module_exit(irs_90_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");
MODULE_DESCRIPTION("IRS-90 Triple sensor driver using HAL Layer");

