#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

void pcdev_release(struct device* dev){
    pr_info("Device released\n");
}

struct pcdev_platform_data pcdev_pdata[2] = {
    [0] = {.size =  512, .perm = RDWR, .serial_number = "PCDEVABC1111"},
    [1] = {.size = 1024, .perm = RDWR, .serial_number = "PCDDEVXYZ2222"}
};

struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata[0],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata[1],
        .release = pcdev_release
    }
};

static int __init pcdev_platform_init(void){

    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);

    pr_info("Device setup module inserted\n");

    return 0;
}

static void __exit pcdev_platform_exit(void){

    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);

    pr_info("Device setup module removed\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers platform devices");
