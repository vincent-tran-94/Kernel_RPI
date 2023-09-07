#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd4b13e21, "module_layout" },
	{ 0x98b8a7e2, "i2c_del_driver" },
	{ 0x28de854e, "i2c_register_driver" },
	{ 0xa0e0c9b1, "misc_register" },
	{ 0xdcccca3e, "of_property_read_string" },
	{ 0xca4cf59e, "devm_kmalloc" },
	{ 0x56470118, "__warn_printk" },
	{ 0x163f4a3f, "_dev_err" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xf9a482f9, "msleep" },
	{ 0x3c01dda, "_dev_warn" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xf4d6426a, "i2c_transfer_buffer_flags" },
	{ 0xd4c8974a, "misc_deregister" },
	{ 0x44239691, "_dev_info" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:lcd2x16");
MODULE_ALIAS("of:N*T*Ctraining,lcd2x16");
MODULE_ALIAS("of:N*T*Ctraining,lcd2x16C*");

MODULE_INFO(srcversion, "9B152A2E44225903D0136C2");
