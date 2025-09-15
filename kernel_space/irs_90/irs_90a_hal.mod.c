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

KSYMTAB_FUNC(irs_90_init, "", "");
KSYMTAB_FUNC(irs_90_read, "", "");
KSYMTAB_FUNC(irs_90_read_all, "", "");
KSYMTAB_FUNC(irs_90_cleanup, "", "");
KSYMTAB_FUNC(irs_90_cleanup_all, "", "");
KSYMTAB_FUNC(irs_90_get_gpio_pin, "", "");
KSYMTAB_FUNC(read_gpio, "", "");

SYMBOL_CRC(irs_90_init, 0x406bf080, "");
SYMBOL_CRC(irs_90_read, 0x1e821c9f, "");
SYMBOL_CRC(irs_90_read_all, 0x6293f8dd, "");
SYMBOL_CRC(irs_90_cleanup, 0xfe23962e, "");
SYMBOL_CRC(irs_90_cleanup_all, 0x6687a919, "");
SYMBOL_CRC(irs_90_get_gpio_pin, 0xd2d3fd20, "");
SYMBOL_CRC(read_gpio, 0x30aff77f, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x122c3a7e, "_printk" },
	{ 0xedc03953, "iounmap" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0x40863ba1, "ioremap_prot" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "097E45CC74F3EDAAEA566D1");
