#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/err.h>
#include <linux/mutex.h>

#define DEV_MEM_SIZE 512

/** @brief This modify pr_info for printing the function name */
#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

static DEFINE_MUTEX(pcd_lock);

/** @brief pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/** @brief This holds the device number */
dev_t device_number;

/** @brief cdev variable */
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file* p_file, loff_t offset, int whence){

    loff_t temp;

    pr_info("lseek requested\n");
    pr_info("current value of the file position = %lld\n", p_file->f_pos);

    switch(whence){
        case SEEK_SET:
            if((offset > DEV_MEM_SIZE) || (offset < 0))
                return -EINVAL;
            p_file->f_pos = offset;
            break;
        case SEEK_CUR:
            temp = p_file->f_pos + offset;
            if((temp > DEV_MEM_SIZE) || (temp < 0))
                return -EINVAL;
            p_file->f_pos = temp;
            break;
        case SEEK_END:
            temp = DEV_MEM_SIZE + offset;
            if((temp > DEV_MEM_SIZE) || (temp < 0))
                return -EINVAL;
            p_file->f_pos = temp;
            break;
        default:
            return -EINVAL;
    }

    pr_info("new value of the file position = %lld\n", p_file->f_pos);

    return p_file->f_pos;
}

ssize_t pcd_read(struct file* p_file, char __user* buff, size_t count, loff_t* f_pos)
{

    if(mutex_lock_interruptible(&pcd_lock)){
        return -EINTR;
    }

    pr_info("read requested for %zu bytes\n", count);
    pr_info("current file position = %lld\n", *f_pos);

    /* Adjust the count */
    if((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    /* Copy to user */
    if(copy_to_user(buff, &device_buffer[*f_pos], count))
        return -EFAULT;

    /* Update the current file position */
    *f_pos += count;

    pr_info("number of bytes successfully read = %zu\n", count);
    pr_info("updated file position = %lld\n", *f_pos);

    mutex_unlock(&pcd_lock);

    /* Return number of bytes which have been successfully read */
    return count;
}

ssize_t pcd_write(struct file* p_file, const char __user* buff, size_t count, loff_t* f_pos)
{

    if(mutex_lock_interruptible(&pcd_lock)){
        return -EINTR;
    }

    pr_info("write requested for %zu bytes\n", count);
    pr_info("current file position = %lld\n", *f_pos);

    /* Adjust the count */
    if((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    if(!count){
        pr_err("No space left on the device\n");
        return -ENOMEM;
    }

    /* Copy from user */
    if(copy_from_user(&device_buffer[*f_pos], buff, count))
        return -EFAULT;

    /* Update the current file position */
    *f_pos += count;

    pr_info("number of bytes successfully written = %zu\n", count);
    pr_info("updated file position = %lld\n", *f_pos);

    mutex_unlock(&pcd_lock);

    /* Return number of bytes which have been successfully written */
    return count;
}

int pcd_open(struct inode* inode, struct file* p_file)
{

    pr_info("open was successful\n");
    return 0;
}

int pcd_release(struct inode* inode, struct file* p_file){

    pr_info("release was successful\n");
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

    int ret;

    /* Dynamically allocate a device number */
    ret = alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");
    if(ret < 0){
        pr_err("Alloc chrdev failed\n");
        goto out;
    }

    pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(device_number), MINOR(device_number));

    /* Initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);

    /* Register a device (cdev structure) with VFS */
    pcd_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pcd_cdev, device_number, 1);
    if(ret < 0){
        pr_err("cdev add failed\n");
        goto unreg_chrdev;
    }

    /* Create device class under /sys/class/ */
    class_pcd = class_create(THIS_MODULE, "pcd_class");
    /* Check if the pointer is erroneus */
    if(IS_ERR(class_pcd)){
        pr_err("Class creation failed\n");
        /* Convert pointer to error code */
        ret = PTR_ERR(class_pcd);
        goto cdev_del;
    }

    /* Populate the sysfs with device information */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    if(IS_ERR(device_pcd)){
        pr_err("Device create failed\n");
        ret = PTR_ERR(device_pcd);
        goto class_del;
    }

    pr_info("Module init was successful");

    return 0;

class_del:
    class_destroy(class_pcd);

cdev_del:
    cdev_del(&pcd_cdev);

unreg_chrdev:
    unregister_chrdev_region(device_number, 1);

out:
    pr_info("Module insertion failed\n");
    return ret;
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
