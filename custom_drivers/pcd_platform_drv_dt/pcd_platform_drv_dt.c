#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/mutex.h>
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
    struct mutex pcd_lock;
};

/* Driver private data structure */
struct pcdrv_private_data{
    int total_devices;
    dev_t device_num_base;
    struct class* class_pcd;
    struct device* device_pcd;
};

struct pcdrv_private_data pcdrv_data;

struct device_config{
    int config_item1;
    int config_item2;
};

enum pcdev_names{
    PCDEVA1X,
    PCDEVB1X,
    PCDEVC1X,
    PCDEVD1X
};

struct device_config pcdev_config[] = {
    [PCDEVA1X] = {.config_item1 = 60, .config_item2 = 21},
    [PCDEVB1X] = {.config_item1 = 50, .config_item2 = 22},
    [PCDEVC1X] = {.config_item1 = 40, .config_item2 = 23},
    [PCDEVD1X] = {.config_item1 = 30, .config_item2 = 24}
};

int check_permission(int dev_perm, int acc_mode){

    if(dev_perm == RDWR)
        return 0;
    if((dev_perm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
        return 0;
    if((dev_perm == WRONLY) && ((acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ)))
        return 0;

    return -EPERM;
}

loff_t pcd_lseek(struct file* p_file, loff_t offset, int whence){

    struct pcdev_private_data* pcdev_data = (struct pcdev_private_data*)p_file->private_data;
    int max_size = pcdev_data->pdata.size;
    loff_t temp;

    pr_info("lseek requested\n");
    pr_info("Current value of the file position = %lld\n", p_file->f_pos);

    switch(whence){
        case SEEK_SET:
            if((offset > max_size) || (offset < 0)){
                return -EINVAL;
            }
            p_file->f_pos = offset;
            break;
        case SEEK_CUR:
            temp = p_file->f_pos + offset;
            if((temp > max_size) || (temp < 0)){
                return -EINVAL;
            }
            p_file->f_pos = temp;
            break;
        case SEEK_END:
            temp = max_size + offset;
            if((temp > max_size) || (temp < 0)){
                return -EINVAL;
            }
            p_file->f_pos = temp;
            break;
        default:
            return -EINVAL;
    }

    pr_info("New value of the file position = %lld\n", p_file->f_pos);

    return p_file->f_pos;
}

ssize_t pcd_read(struct file* p_file, char __user* buff, size_t count, loff_t* f_pos){

    struct pcdev_private_data* pcdev_data = (struct pcdev_private_data*)p_file->private_data;
    int max_size = pcdev_data->pdata.size;

    mutex_lock(&pcdev_data->pcd_lock);

    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    if((*f_pos + count) > max_size){
        count = max_size - *f_pos;
    }

    if(copy_to_user(buff, pcdev_data->buffer + (*f_pos), count)){
        return -EFAULT;
    }

    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    mutex_unlock(&pcdev_data->pcd_lock);

    return count;
}

ssize_t pcd_write(struct file* p_file, const char __user* buff, size_t count, loff_t* f_pos){

    struct pcdev_private_data* pcdev_data = (struct pcdev_private_data*)p_file->private_data;
    int max_size = pcdev_data->pdata.size;

    mutex_lock(&pcdev_data->pcd_lock);

    pr_info("Write requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    if((*f_pos + count) > max_size){
        count = max_size - *f_pos;
    }

    if(!count){
        pr_err("No space left on the device\n");
        return -ENOMEM;
    }

    if(copy_from_user(pcdev_data->buffer + (*f_pos), buff, count)){
        return -EFAULT;
    }

    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    mutex_unlock(&pcdev_data->pcd_lock);

    return count;
}

int pcd_open(struct inode* inode, struct file* p_file){

    int ret;
    int minor_n;
    struct pcdev_private_data* pcdev_data;

    /* Find out on which device file open was attempted by the user space */
    minor_n = MINOR(inode->i_rdev);
    pr_info("minor access = %d\n", minor_n);

    pcdev_data = container_of(inode->i_cdev, struct pcdev_private_data, cdev);

    /* To supply device private data to other methods of the driver */
    p_file->private_data = pcdev_data;

    ret = check_permission(pcdev_data->pdata.perm, p_file->f_mode);

    (!ret)?pr_info("Open was successful\n"):pr_info("Open was unsuccessful\n");

    return ret;
}

int pcd_release(struct inode* inode, struct file* p_file){

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

    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);

    /* Remove a device that was created with device_create() */
    device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);

    /* Remove a cdev entry from the system */
    cdev_del(&dev_data->cdev);

    pcdrv_data.total_devices--;

    dev_info(&pdev->dev, "Device is removed\n");

    return 0;
}

struct pcdev_platform_data* pcdev_get_platdata_from_dt(struct device* dev){

    struct device_node* dev_node = dev->of_node;
    struct pcdev_platform_data* pdata;

    if(!dev_node){
        /* This probe didn't happen because of device tree node */
        return NULL;
    }

    pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
    if(!pdata){
        dev_info(dev, "Cannot allocate memory\n");
        return ERR_PTR(-ENOMEM);
    }

    if(of_property_read_string(dev_node, "org,device-serial-num", &pdata->serial_number)){
        dev_info(dev, "Missing serial number property\n");
        return ERR_PTR(-EINVAL);
    }

    if(of_property_read_u32(dev_node, "org,size", &pdata->size)){
        dev_info(dev, "Missing size property\n");
        return ERR_PTR(-EINVAL);
    }

    if(of_property_read_u32(dev_node, "org,perm", &pdata->perm)){
        dev_info(dev, "Missing permission property\n");
        return ERR_PTR(-EINVAL);
    }

    return pdata;
}

struct of_device_id org_pcdev_dt_match[];

/* Gets called when matched platform device is found */
int pcd_platform_driver_probe(struct platform_device* pdev){

    int ret;

    struct pcdev_private_data* dev_data;
    struct pcdev_platform_data* pdata;
    struct device* dev = &pdev->dev;
    int driver_data;
    /* Used to store matched entry of "of_device_id" list of this driver */
    const struct of_device_id* match;

    dev_info(dev, "A device is detected\n");

    /* Match will always be NULL if Linux doesn't support device tree i.e CONFIG_OF is off */
    match = of_match_device(of_match_ptr(org_pcdev_dt_match), dev);

    if(match){
        pdata = pcdev_get_platdata_from_dt(dev);
        if(IS_ERR(pdata)){
            return PTR_ERR(pdata);
        }
        driver_data = (int)match->data;
    }
    else{
        /* Get the platform data */
        pdata = (struct pcdev_platform_data*)dev_get_platdata(dev);
        driver_data = pdev->id_entry->driver_data;
    }

    if(!pdata){
        dev_info(dev, "No platform data available\n");
        return -EINVAL;
    }

    /* Dynamically allocate memory for the device private data */
    dev_data = devm_kzalloc(&pdev->dev, sizeof(*dev_data), GFP_KERNEL);
    if(!dev_data){
        dev_info(dev, "Cannot allocate memory\n");
        return -ENOMEM;
    }

    /* Initialize the lock mechanism for mutex */
    mutex_init(&dev_data->pcd_lock);

    /* Save the device private data pointer in the platform device structure */
    dev_set_drvdata(&pdev->dev, dev_data);

    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    dev_info(dev, "Device serial number = %s\n", dev_data->pdata.serial_number);
    dev_info(dev, "Device size = %d\n", dev_data->pdata.size);
    dev_info(dev, "Device permission = %d\n", dev_data->pdata.perm);

    dev_info(dev, "Config item 1 = %d\n", pcdev_config[driver_data].config_item1);
    dev_info(dev, "Config item 2 = %d\n", pcdev_config[driver_data].config_item2);

    /* Dynamically allocate memory for the device buffer using size information from the platform data */
    dev_data->buffer = devm_kzalloc(&pdev->dev, dev_data->pdata.size, GFP_KERNEL);
    if(!dev_data->buffer){
        dev_info(dev, "Cannot allocate memory");
        return -ENOMEM;
    }

    /* Get the device number */
    dev_data->dev_num = pcdrv_data.device_num_base + pcdrv_data.total_devices;

    /* Do cdev init and cdev add */
    cdev_init(&dev_data->cdev, &pcd_fops);
    dev_data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev_data->cdev, dev_data->dev_num, 1);
    if(ret < 0){
        dev_err(dev, "cdev add failed\n");
        return ret;
    }

    /* Create device file for the detected platform device */
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd,
                                          dev,
                                          dev_data->dev_num,
                                          NULL,
                                          "pcdev-%d",
                                          pcdrv_data.total_devices);
    if(IS_ERR(pcdrv_data.device_pcd)){
        dev_err(dev, "Device create failed\n");
        ret = PTR_ERR(pcdrv_data.device_pcd);
        cdev_del(&dev_data->cdev);
        return ret;
    }

    pcdrv_data.total_devices++;

    dev_info(dev, "Probe was successful\n");

    return 0;
}

struct platform_device_id pcdevs_ids[] = {
    {.name = "pcdev-A1x", .driver_data = PCDEVA1X},
    {.name = "pcdev-B1x", .driver_data = PCDEVB1X},
    {.name = "pcdev-C1x", .driver_data = PCDEVC1X},
    {.name = "pcdev-D1x", .driver_data = PCDEVD1X},
    {}
};

struct of_device_id org_pcdev_dt_match[] = {
    {.compatible = "pcdev-A1x", .data = (void*)PCDEVA1X},
    {.compatible = "pcdev-B1x", .data = (void*)PCDEVB1X},
    {.compatible = "pcdev-C1x", .data = (void*)PCDEVC1X},
    {.compatible = "pcdev-D1x", .data = (void*)PCDEVD1X},
    {}
};

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .id_table = pcdevs_ids,
    .driver = {
        .name = "pseudo-char-device",
        .of_match_table = of_match_ptr(org_pcdev_dt_match)
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
