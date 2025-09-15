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
	{ 0x406bf080, "irs_90_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x5d9d9fd4, "cdev_init" },
	{ 0xcc335c1c, "cdev_add" },
	{ 0xf311fc60, "class_create" },
	{ 0x93ab9e33, "device_create" },
	{ 0xd2d3fd20, "irs_90_get_gpio_pin" },
	{ 0x6687a919, "irs_90_cleanup_all" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xfe23962e, "irs_90_cleanup" },
	{ 0x607587f4, "cdev_del" },
	{ 0x4a41ecb3, "class_destroy" },
	{ 0xe095e43a, "device_destroy" },
	{ 0x6293f8dd, "irs_90_read_all" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "irs_90a_hal");


MODULE_INFO(srcversion, "72EE5A6D8E3BE5DAA49BF26");
