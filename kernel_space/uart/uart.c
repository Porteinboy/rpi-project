/*
 *  uart driver exercise code copyright is owned by the authors
 *  author: jiunnder2000@yahoo.com.tw
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/errno.h>  /* error codes */
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/poll.h>
#include <linux/math.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
#include <linux/cdev.h>
static struct cdev mycdev;
static dev_t devno=0;
#endif

#include "rpi4_gpio.h"

#define UART2_NAME "uart2"
#define UART3_NAME "uart3"

int uart_major = 0;
int uart_minor = 0;

uint32_t uart_port_base[] = { UART2_BASE, UART3_BASE};
char uart_name[][8] = { UART2_NAME, UART3_NAME };

/* Reference: bcm2711-peripherals.pdf p.68
 * (0b011): GPIO takes alternate function 4
 * a4: GPIO0: TXD2
 * a4: GPIO1: RXD2
 * a4: GPIO4: TXD3
 * a4: GPIO5: TXD3
 */
uint32_t fn = 0x3 | (0x3 <<3) | (0x3 <<12) | (0x3 <<15), orig_fn ;
uint32_t *gpio_base;
uint32_t *gpio_GPFSEL0;
uint32_t *uart_base[UARTS_NUM];
uint32_t *uart_DR[UARTS_NUM];
uint32_t *uart_RSRECR[UARTS_NUM];
uint32_t *uart_FR[UARTS_NUM];
uint32_t *uart_IBRD[UARTS_NUM];
uint32_t *uart_FBRD[UARTS_NUM];
uint32_t *uart_LCRH[UARTS_NUM];
uint32_t *uart_CR[UARTS_NUM];

static int uart_open (struct inode *inode, struct file *filp)
{
    int dev_minor = iminor(inode);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
        try_module_get(THIS_MODULE);
#else
        MOD_INC_USE_COUNT;
#endif
    printk(KERN_ALERT "uart_open():\n");
 
    return 0;
}

static int uart_close(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "uart_close():\n");
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
    module_put(THIS_MODULE);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,4,0)
    MOD_DEC_USE_COUNT;
#endif
    return 0;
}

ssize_t uart_read (struct file * filp, char *buf, size_t count, loff_t *t)
{
    int dev_minor = iminor(filp->f_path.dentry->d_inode);
    int rcount;
    uint32_t status;
    unsigned char kbuf;

    // To do: read data from uart2 and copy to user space
    status = readl(uart_FR[dev_minor]);

    //if ( !(status & UART_FR_RXFE) )
    if ( (status & UART_FR_RXFF) )
    {
	    kbuf= readb(uart_DR[dev_minor]);
	    printk(KERN_ALERT " uart_read[%d](): status:%x, kbuf: %2x\n", dev_minor, status, kbuf);
 	    rcount = copy_to_user(buf, &kbuf, 1);
       
	    return 1;
    }

    return 0;
}

ssize_t uart_write (struct file * filp, const char *buf, size_t count, loff_t * t)
{
    int dev_minor = iminor(filp->f_path.dentry->d_inode);
    int i=0;
    uint32_t status;
    unsigned char kbuf[80];

    // Copy the data from user space
    if( copy_from_user(kbuf, buf, count) != 0 ) {
      printk("uart_write(): count:%d\n", count);
      return -EFAULT;
    }
    
    while( i < count ) {
      status = readl(uart_FR[dev_minor]);
      // To do: Wait for until transmit holding register empty
      if( status & UART_FR_TXFE ) {
        printk(KERN_ALERT "uart_write[%d](): status:%x, kbuf[0]:%x\n", dev_minor, status, kbuf[0]);
	// To do: Write to UART transmit holding register
        writeb(*(kbuf+i), uart_DR[dev_minor]);
	
	while ( readl(uart_FR[dev_minor]) & UART_FR_BUSY);
	//  printk(KERN_ALERT "TX complete\n");
	 
	i++;
      }
    }

    return i;
}


struct file_operations uart_fops = {
    .read=uart_read,
    .write=uart_write,
    .open=uart_open,
    .release = uart_close
};

int init_module(void)
{
    int i;
    
    struct resource *uart_region[2];

    printk(KERN_ALERT "uart: INIT_MOD\n");
    // To do: request the IO port region
    for (i=0; i<UARTS_NUM; i++) {
	    uart_region[i] = request_mem_region(uart_port_base[i], UART_REGLEN, uart_name[i]);
	    if ( ! uart_region[i] ) {
		    printk("request_mem_region() %x fail\n", uart_port_base[i]);
		    return -1;
	    }
    }

   //  register character driver
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
    if(uart_major) {
        if ( register_chrdev_region(MKDEV(uart_major,0), UARTS_NUM, "uart") < 0 ) {
            printk ("register_chrdev_region() fail\n");
	    goto fail_register_chrdev;
        }
    }
    else {
        if (alloc_chrdev_region(&devno, 0, UARTS_NUM, "uart") < 0) {
            printk ("alloc_chrdev_region() fail\n");
	    goto fail_register_chrdev;
        }
        uart_major=MAJOR(devno);
    }
    cdev_init(&mycdev, &uart_fops);
    mycdev.owner=THIS_MODULE;
    if(cdev_add(&mycdev, MKDEV(uart_major,0), UARTS_NUM)) {
        printk ("Error adding cdev\n");
        goto fail_cdev_add;
    }
#else
    uart_major = register_chrdev(0, "uart", &uart_fops);
    if (uart_major < 0) {
	printk(KERN_ALERT "uart: can't get major number\n");
        goto fail_cdev_add;
    }
#endif

    gpio_base = ioremap(GPIO_BASE, 1 * 256);
    if ( gpio_base == NULL ) {
        printk("ioremap() fail\n");
	goto failed_gpio_ioremap;
    }

    gpio_GPFSEL0 = (gpio_base+GPFSEL0/4);

    orig_fn = readl(gpio_GPFSEL0);
    writel(fn ,gpio_GPFSEL0); 

    for( i=0; i < UARTS_NUM ; i++ ) {
	if ( i == 0 )
            uart_base[i] = ioremap(UART2_BASE, UART_REGLEN);
	else
            uart_base[i] = ioremap(UART3_BASE, UART_REGLEN);
        if ( uart_base[i] == NULL ) {
            printk("ioremap() fail\n");
            goto failed_uart_ioremap;
        }
        uart_DR[i] = (uart_base[i] + DR/4);
        uart_RSRECR[i] = (uart_base[i] + RSRECR/4);
        uart_FR[i] = (uart_base[i] + FR/4);
        uart_IBRD[i] = (uart_base[i] + IBRD/4);
        uart_FBRD[i] = (uart_base[i] + FBRD/4);
        uart_LCRH[i] = (uart_base[i] + LCRH/4);
        uart_CR[i] = (uart_base[i] + CR/4);

    	/* To do: Enable UART, Enable Receive, Enable Transmit */
	writel(CR_UARTEN | CR_RXE | CR_TXE , uart_CR[i]);

        /* Reference: bcm2711-peripherals.pdf p.151.
         * BAUDDIV = ( FUARTCLK / 16 * Baud rate )
         * ( IBRD + FBRD ) = ( FUARTCLK / 16 * Baud rate )
         *                 = ( 48000000 / 16 * 115200 )
         *                 = 26.041666
	 * IBRD = 26
	 * FBRD = 0.041667 * 64 = 2.6
	 * baud: 115200, IBRD=26, FBRD=3
	 * baud: 38400, IBRD=78, FBRD=8
	 * baud: 19200, IBRD=156, FBRD=16
         * where FUARTCLK is the UART reference clock frequency
         * BAUDDIV: comprised of the IBRD (integer) and the FBRD (fraction)
         */
        /* To do: set the baud: 115200 */
        writel(26, uart_IBRD[i]);
        writel(3, uart_FBRD[i]);

	/* To do: Bits 8, FIFO disable, Stop 1, parity check disable */
        writel(LCRH_WLEN_8BITS , uart_LCRH[i]);
    }
    
    return 0;

failed_gpio_ioremap:
    iounmap(gpio_base);
failed_uart_ioremap:
    cdev_del(&mycdev);
fail_cdev_add:
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
    unregister_chrdev_region(MKDEV(uart_major, 0), UARTS_NUM);
#else
    unregister_chrdev(uart_major, "uart");
#endif
fail_register_chrdev:
    for(i=0; i<UARTS_NUM; i++)
        release_mem_region(uart_port_base[i], UART_REGLEN);

    return -EIO;
}

void cleanup_module(void)
{
    int i;

    /* To do: Disable UART, disable Receive, disable Transmit */
    writel(orig_fn ,gpio_GPFSEL0);
    iounmap(gpio_base);
    for(i=0; i<UARTS_NUM; i++)
        iounmap(uart_base[i]);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(uart_major, 0), UARTS_NUM);
#else
    /* Unregister the device */
    unregister_chrdev(uart_major, "uart");
#endif

    // To do: release the IO port region
    for(i=0; i<UARTS_NUM; i++)
        release_mem_region(uart_port_base[i], UART_REGLEN);
}

MODULE_LICENSE("GPL");
