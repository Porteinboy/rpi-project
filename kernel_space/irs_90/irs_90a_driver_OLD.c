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
    printk(KERN_INFO "irs90a: device opened\n");
    return 0;
}

static int irs90_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "irs90a: device closed\n");
    return 0;
}

static ssize_t irs90_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int data = read_gpio();

    printk(KERN_INFO "irs90a Read: %d\n", data);

    char msg[4];
    int len = snprintf(msg, sizeof(msg), "%d\n", data);

printk("%s[%d]count:%d, len:%d\n", __FUNCTION__, __LINE__,count, len);
    //if (*ppos > 0 || count < len)
    //    return 0;

    printk("%s[%d]\n", __FUNCTION__, __LINE__);
    if (copy_to_user(buf, msg, len))
        return -EFAULT;

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

static int __init irs_90_init(void)
{
    int ret;
    printk(KERN_INFO "IRS_90a: Initializing\n");

    ret = alloc_chrdev_region(&dev, 0, 1, "irs_90a");
    if(ret < 0){
    	printk(KERN_ALERT "IRS_90a: Failed to allocate char device region\n");
	return ret;
    }
    	printk(KERN_INFO "IRS_90a major number allocated= %d\n", MAJOR(dev));
    	printk(KERN_INFO "IRS_90a minor number= %d\n", MINOR(dev));

	cdev_init(&c_dev, &fops);
	c_dev.owner = THIS_MODULE;

	ret = cdev_add(&c_dev, dev, 1);
	if(ret < 0){
	    unregister_chrdev_region(dev, 1);
	    printk(KERN_ALERT "IRS_90a: Failed to add cdev\n");
	    return ret;
	}

	cl = class_create("irs90a_class");
	if(IS_ERR(cl)){
	    cdev_del(&c_dev);
	    unregister_chrdev_region(dev, 1);
	    printk(KERN_ALERT "IRS_90a: Failed to create class\n");
	    return PTR_ERR(cl);
	}

    cl -> devnode = irs_90_devnode;

    device_create(cl, NULL, dev, NULL, "irs_90a");

    printk(KERN_INFO "irs90a: module loaded, major=%d\n", MAJOR(dev));
    return 0;
}


static void __exit irs_90_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO "irs90a: driver unloaded successfully\n");
}

module_init(irs_90_init);
module_exit(irs_90_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sampson");
MODULE_DESCRIPTION("IRS-90a sensor driver using GPIO20");

