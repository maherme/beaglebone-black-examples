#include "pcd_platform_drv_dt_sysfs.h"

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
