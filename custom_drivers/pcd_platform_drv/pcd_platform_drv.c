#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define MAX_DEVICES 10

/* Device private data structure */
struct pcdev_private_data{
    struct pcdev_platform_data pdata;
    char* buffer;
    dev_t dev_num;
    struct cdev cdev;
};

/* Driver private data structure */
struct pcdrv_private_data{
    int total_devices;
    dev_t device_num_base;
    struct class* class_pcd;
    struct device* device_pcd;
};

struct pcdrv_private_data pcdrv_data;

int check_permission(int dev_perm, int acc_mode){

    if(dev_perm == RDWR)
        return 0;
    if((dev_perm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
        return 0;
    if((dev_perm == WRONLY) && ((acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ)))
        return 0;

    return -EPERM;
}

loff_t pcd_lseek(struct file* filp, loff_t offset, int whence){

    return 0;
}

ssize_t pcd_read(struct file* filp, char __user* buff, size_t count, loff_t* f_pos){

    return 0;
}

ssize_t pcd_write(struct file* filp, const char __user* buff, size_t count, loff_t* f_pos){

    return -ENOMEM;
}

int pcd_open(struct inode* inode, struct file* filp){

    return 0;
}

int pcd_release(struct inode* inode, struct file* filp){

    pr_info("Release was successful\n");
    return 0;
}

/* File operations of the driver */
struct file_operations pcd_fops = {
    .open = pcd_open,
    .release = pcd_release,
    .read = pcd_read,
    .write = pcd_write,
    .llseek = pcd_lseek,
    .owner = THIS_MODULE
};

/* Gets called when the device is removed from the system */
int pcd_platform_driver_remove(struct platform_device* pdev){

    pr_info("Device is removed\n");
    return 0;
}

/* Gets called when matched platform device is found */
int pcd_platform_driver_probe(struct platform_device* pdev){

    int ret;
    struct pcdev_private_data* dev_data;
    struct pcdev_platform_data* pdata;

    /* Get the platform data */
    pdata = (struct pcdev_platform_data*)dev_get_platdata(&pdev->dev);
    if(!pdata){
        pr_info("No platform data available\n");
        ret = -EINVAL;
        goto out;
    }

    /* Dynamically allocate memory for the device private data */
    dev_data = kzalloc(sizeof(*dev_data), GFP_KERNEL);
    if(!dev_data){
        pr_info("Cannot allocate memory\n");
        ret = -ENOMEM;
        goto out;
    }

    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    pr_info("Device serial number = %s\n", dev_data->pdata.serial_number);
    pr_info("Device size = %d\n", dev_data->pdata.size);
    pr_info("Device permission = %d\n", dev_data->pdata.perm);

    /* Dynamically allocate memory for the device buffer using size information from the platform data */

    pr_info("A device is detected\n");
    return 0;

out:
    pr_info("Device probe failed\n");
    return ret;
}

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name="pseudo-char-device"
    }
};

static int __init pcd_platform_driver_init(void){

    int ret;

    /* Dynamically allocate a device number for MAX_DEVICES */
    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, MAX_DEVICES, "pcdevs");
    if(ret < 0){
        pr_err("Alloc chrdev failed\n");
        goto out;
    }

    /* Create device class under /sys/class */
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if(IS_ERR(pcdrv_data.class_pcd)){
        pr_err("Class creation failed\n");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        goto unreg_chrdev;
    }

    /* Register a platform driver */
    ret = platform_driver_register(&pcd_platform_driver);
    if(ret < 0){
        pr_err("Driver registration failed\n");
        goto class_del;
    }

    pr_info("pcd platform driver loaded\n");

    return 0;

class_del:
    class_destroy(pcdrv_data.class_pcd);

unreg_chrdev:
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);

out:
    pr_info("Platform driver init failed\n");
    return ret;
}

static void __exit pcd_platform_driver_cleanup(void){

    /* Unregister the platform driver */
    platform_driver_unregister(&pcd_platform_driver);

    /* Class destroy */
    class_destroy(pcdrv_data.class_pcd);

    /* Unregister device number for MAX_DEVICES */
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);

    pr_info("pcd platform driver unloaded\n");
}

module_init(pcd_platform_driver_init);
module_exit(pcd_platform_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("maherme");
MODULE_DESCRIPTION("A pseudo character platform driver which handles n platform pcdevs");
