#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x14\x00\x00\x00\x53\x3f\x45\x4a"
	"iowrite32\0\0\0"
	"\x10\x00\x00\x00\x09\xcd\x80\xde"
	"ioremap\0"
	"\x14\x00\x00\x00\xc1\x6b\xa8\x06"
	"iowrite16\0\0\0"
	"\x18\x00\x00\x00\x67\x6b\x0e\x25"
	"dma_free_attrs\0\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x1c\x00\x00\x00\xc0\xfb\xc3\x6b"
	"__unregister_chrdev\0"
	"\x14\x00\x00\x00\x3b\x4a\x51\xc1"
	"free_irq\0\0\0\0"
	"\x14\x00\x00\x00\x10\x54\xec\x74"
	"pci_iounmap\0"
	"\x1c\x00\x00\x00\x0e\x69\x2f\xae"
	"pci_release_regions\0"
	"\x1c\x00\x00\x00\xcf\xd8\x21\xe4"
	"pci_disable_device\0\0"
	"\x14\x00\x00\x00\x40\xb5\x52\x85"
	"pci_dev_put\0"
	"\x20\x00\x00\x00\x3a\x69\xa3\x65"
	"pci_read_config_word\0\0\0\0"
	"\x14\x00\x00\x00\xf3\xf5\x8a\xa7"
	"ioread32\0\0\0\0"
	"\x14\x00\x00\x00\x9d\x8e\x2c\x84"
	"ioread16\0\0\0\0"
	"\x18\x00\x00\x00\xf1\xfd\x89\x4f"
	"pci_get_device\0\0"
	"\x20\x00\x00\x00\x4a\x3c\x2a\x44"
	"pci_enable_device_mem\0\0\0"
	"\x18\x00\x00\x00\xd2\x70\x62\x27"
	"pci_set_master\0\0"
	"\x18\x00\x00\x00\xca\x90\x00\x99"
	"dma_set_mask\0\0\0\0"
	"\x20\x00\x00\x00\xb3\xb4\x01\xeb"
	"dma_set_coherent_mask\0\0\0"
	"\x1c\x00\x00\x00\x0a\x24\xe9\xac"
	"pci_request_regions\0"
	"\x14\x00\x00\x00\x19\x1d\x5b\x53"
	"pci_iomap\0\0\0"
	"\x20\x00\x00\x00\x8e\x83\xd5\x92"
	"request_threaded_irq\0\0\0\0"
	"\x18\x00\x00\x00\xbe\xc7\xbf\xd8"
	"dma_alloc_attrs\0"
	"\x10\x00\x00\x00\xc5\x8f\x57\xfb"
	"memset\0\0"
	"\x1c\x00\x00\x00\xd1\x53\xbd\x3f"
	"__register_chrdev\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x7a\xef\x79\x14"
	"dma_mmap_attrs\0\0"
	"\x18\x00\x00\x00\xc2\x9c\xc4\x13"
	"_copy_from_user\0"
	"\x18\x00\x00\x00\xe1\xbe\x10\x6b"
	"_copy_to_user\0\0\0"
	"\x18\x00\x00\x00\x2c\x18\xbc\xf1"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F57C1B40F9854599C005BD5");
