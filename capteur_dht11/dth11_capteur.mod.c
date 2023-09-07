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
	{ 0x292268, "noop_llseek" },
	{ 0x13868293, "platform_driver_unregister" },
	{ 0xd4c8974a, "misc_deregister" },
	{ 0xa0e0c9b1, "misc_register" },
	{ 0x81cbe3cb, "__platform_driver_register" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x6f414e08, "gpiod_get_raw_value" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xf28855b, "gpiod_direction_input" },
	{ 0x9ddce221, "gpiod_set_raw_value" },
	{ 0xf9a482f9, "msleep" },
	{ 0xeb349ff6, "wake_up_process" },
	{ 0xf45a89df, "kthread_create_on_node" },
	{ 0xa75efb1c, "gpiod_direction_output_raw" },
	{ 0x3a109e75, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x5e23a5ba, "of_property_read_variable_u32_array" },
	{ 0x44239691, "_dev_info" },
	{ 0x92997ed8, "_printk" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x56470118, "__warn_printk" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xfe990052, "gpio_free" },
	{ 0xbbcf8b16, "kthread_stop" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ctraining,dht11platform");
MODULE_ALIAS("of:N*T*Ctraining,dht11platformC*");

MODULE_INFO(srcversion, "BD3FEDC48EE0541A357CEF7");
