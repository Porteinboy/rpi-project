#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

KSYMTAB_FUNC(uart_hal_init, "", "");
KSYMTAB_FUNC(uart_hal_exit, "", "");
KSYMTAB_FUNC(uart_hal_write_char, "", "");
KSYMTAB_FUNC(uart_hal_read_char, "", "");

SYMBOL_CRC(uart_hal_init, 0xb4ef4356, "");
SYMBOL_CRC(uart_hal_exit, 0x695cb4d7, "");
SYMBOL_CRC(uart_hal_write_char, 0x94588010, "");
SYMBOL_CRC(uart_hal_read_char, 0x886a41d2, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xedc03953, "iounmap" },
	{ 0x122c3a7e, "_printk" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0x40863ba1, "ioremap_prot" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F5EC4AE7849BE287D50F327");
