#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define DEV_MEM_SIZE 512

/** @brief This modify pr_info for printing the function name */
#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

/** @brief pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/** @brief This holds the device number */
dev_t device_number;

/** @brief cdev variable */
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file* p_file, loff_t offset, int whence){

    pr_info("lseek requested\n");
    return 0;
}

ssize_t pcd_read(struct file* p_file, char __user* buff, size_t count, loff_t* f_pos)
{

    pr_info("read requested for %zu bytes\n", count);
    return 0;
}

ssize_t pcd_write(struct file* p_file, const char __user* buff, size_t count, loff_t* f_pos)
{

    pr_info("write requested for %zu bytes\n", count);
    return 0;
}

int pcd_open(struct inode* inode, struct file* p_file)
{

    pr_info("open was successful\n");
    return 0;
}

int pcd_release(struct inode* inode, struct file* p_file){

    pr_info("close was successful\n");
    return 0;
}

/* File operations of the driver */
struct file_operations pcd_fops = {.open = pcd_open,
                                   .write = pcd_write,
                                   .read = pcd_read,
                                   .llseek = pcd_lseek,
                                   .release = pcd_release,
                                   .owner = THIS_MODULE};

struct class* class_pcd;
struct device* device_pcd;

static int __init pcd_driver_init(void){

    /* Dynamically allocate a device number */
    alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");

    pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(device_number), MINOR(device_number));

    /* Initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);

    /* Register a device (cdev structure) with VFS */
    pcd_cdev.owner = THIS_MODULE;
    cdev_add(&pcd_cdev, device_number, 1);

    /* Create device class under /sys/class/ */
    class_pcd = class_create(THIS_MODULE, "pcd_class");

    /* Populate the sysfs with device information */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");

    pr_info("Module init was successful");

    return 0;
}

static void __exit pcd_driver_cleanup(void){

    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);

    pr_info("module unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("maherme");
MODULE_DESCRIPTION("A pseudo character driver");
