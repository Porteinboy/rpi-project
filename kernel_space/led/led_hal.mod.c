#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

KSYMTAB_FUNC(led_hal_init, "", "");
KSYMTAB_FUNC(led_hal_cleanup, "", "");
KSYMTAB_FUNC(led_hal_get_all_leds, "", "");
KSYMTAB_FUNC(led_hal_set_all_leds, "", "");
KSYMTAB_FUNC(led_hal_set_led, "", "");
KSYMTAB_FUNC(led_hal_get_led, "", "");
KSYMTAB_FUNC(led_hal_turn_off_all, "", "");
KSYMTAB_FUNC(led_hal_turn_on_all, "", "");

SYMBOL_CRC(led_hal_init, 0x10ad57bc, "");
SYMBOL_CRC(led_hal_cleanup, 0x1c8e4178, "");
SYMBOL_CRC(led_hal_get_all_leds, 0x58a44fc7, "");
SYMBOL_CRC(led_hal_set_all_leds, 0xaf8dba91, "");
SYMBOL_CRC(led_hal_set_led, 0x41959a61, "");
SYMBOL_CRC(led_hal_get_led, 0xd65a98ce, "");
SYMBOL_CRC(led_hal_turn_off_all, 0x3c9bd0fb, "");
SYMBOL_CRC(led_hal_turn_on_all, 0x14a717f1, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x122c3a7e, "_printk" },
	{ 0x98cf60b3, "strlen" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0x40863ba1, "ioremap_prot" },
	{ 0xedc03953, "iounmap" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "6BE5950B7EB5C001A366554");
