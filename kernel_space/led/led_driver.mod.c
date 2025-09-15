#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x122c3a7e, "_printk" },
	{ 0x10ad57bc, "led_hal_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x5d9d9fd4, "cdev_init" },
	{ 0xcc335c1c, "cdev_add" },
	{ 0xf311fc60, "class_create" },
	{ 0x93ab9e33, "device_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1c8e4178, "led_hal_cleanup" },
	{ 0x607587f4, "cdev_del" },
	{ 0x4a41ecb3, "class_destroy" },
	{ 0xe095e43a, "device_destroy" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x58a44fc7, "led_hal_get_all_leds" },
	{ 0x98cf60b3, "strlen" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x52c5c991, "__kmalloc_noprof" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x9166fada, "strncpy" },
	{ 0xaf8dba91, "led_hal_set_all_leds" },
	{ 0x37a0cba, "kfree" },
	{ 0xdcb764ad, "memset" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "led_hal");


MODULE_INFO(srcversion, "18F59B4E23928455607B38F");
