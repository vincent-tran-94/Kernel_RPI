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
	{ 0x13868293, "platform_driver_unregister" },
	{ 0xd4c8974a, "misc_deregister" },
	{ 0xfe990052, "gpio_free" },
	{ 0xbbcf8b16, "kthread_stop" },
	{ 0xf28855b, "gpiod_direction_input" },
	{ 0xeb349ff6, "wake_up_process" },
	{ 0xf45a89df, "kthread_create_on_node" },
	{ 0xa75efb1c, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xa0e0c9b1, "misc_register" },
	{ 0x81cbe3cb, "__platform_driver_register" },
	{ 0x9af14a1c, "devm_request_threaded_irq" },
	{ 0x163f4a3f, "_dev_err" },
	{ 0x7606362, "platform_get_irq" },
	{ 0x44239691, "_dev_info" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x98cf60b3, "strlen" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x92997ed8, "_printk" },
	{ 0x9ddce221, "gpiod_set_raw_value" },
	{ 0x3a109e75, "gpio_to_desc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ctraining,intbutton");
MODULE_ALIAS("of:N*T*Ctraining,intbuttonC*");

MODULE_INFO(srcversion, "0D6F3A8060D90ED87C835ED");
