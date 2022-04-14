# Custom Linux Device Drivers

This is a list of different device drivers:
- [hello_world](hello_world): a very simple driver for printing a string when it is initialized and removed.
- [pseudo_char_drv](pseudo_char_drv): a pseudo character driver which implements lseek, read, write, open and release functions.
- [pseudo_char_drv_multiple](pseudo_char_drv_multiple): a pseudo character driver for four devices.
- [pcd_platform_drv](pcd_platform_drv): a pseudo character platform driver.
- [pcd_platform_drv_dt](pcd_platform_drv_dt): a pseudo character platform driver which uses the device tree.
- [pcd_platform_drv_dt_ov](pcd_platform_drv_dt_ov): a pseudo character platform driver which uses the device tree overlay.
- [pcd_sysfs](pcd_sysfs): a pseudo character platform driver which uses the sys filesystem.
- [gpio_sysfs](gpio_sysfs): a gpio driver.
