# Pseudo Character Device Platform Driver

This is a platform driver example, the driver is divided into two kernel objects.

You can compile this driver using the Makefile:
```console
make host
```
Then you will get two .ko files: pcd_device_setup.ko and pcd_platform_drv.ko.
You can install both kernel objects:
```console
sudo insmod pcd_device_setup.ko
sudo insmod pcd_platform_drv.ko
```
You can also remove the driver using:
```console
sudo rmmod pcd_platform_drv
sudo rmmod pcd_device_setup
```
You can get the print information of these operations doing ```dmesg```, here my log when installing and removing the driver:
```console
[ 9282.628632] pcdev_platform_init : Device setup module loaded
[ 9299.563670] pcd_platform_driver_probe : A device is detected
[ 9299.563676] pcd_platform_driver_probe : Device serial number = PCDEVABC1111
[ 9299.563677] pcd_platform_driver_probe : Device size = 512
[ 9299.563679] pcd_platform_driver_probe : Device permission = 17
[ 9299.563680] pcd_platform_driver_probe : Config item 1 = 60
[ 9299.563681] pcd_platform_driver_probe : Config item 2 = 21
[ 9299.564031] pcd_platform_driver_probe : Probe was successful
[ 9299.564046] pcd_platform_driver_probe : A device is detected
[ 9299.564048] pcd_platform_driver_probe : Device serial number = PCDEVXYZ2222
[ 9299.564049] pcd_platform_driver_probe : Device size = 1024
[ 9299.564050] pcd_platform_driver_probe : Device permission = 17
[ 9299.564051] pcd_platform_driver_probe : Config item 1 = 50
[ 9299.564052] pcd_platform_driver_probe : Config item 2 = 22
[ 9299.564127] pcd_platform_driver_probe : Probe was successful
[ 9299.564144] pcd_platform_driver_probe : A device is detected
[ 9299.564146] pcd_platform_driver_probe : Device serial number = PCDEVXYZ3333
[ 9299.564147] pcd_platform_driver_probe : Device size = 128
[ 9299.564149] pcd_platform_driver_probe : Device permission = 1
[ 9299.564150] pcd_platform_driver_probe : Config item 1 = 40
[ 9299.564151] pcd_platform_driver_probe : Config item 2 = 23
[ 9299.564190] pcd_platform_driver_probe : Probe was successful
[ 9299.564201] pcd_platform_driver_probe : A device is detected
[ 9299.564202] pcd_platform_driver_probe : Device serial number = PCDEVXYZ4444
[ 9299.564203] pcd_platform_driver_probe : Device size = 32
[ 9299.564204] pcd_platform_driver_probe : Device permission = 16
[ 9299.564206] pcd_platform_driver_probe : Config item 1 = 30
[ 9299.564207] pcd_platform_driver_probe : Config item 2 = 24
[ 9299.564241] pcd_platform_driver_probe : Probe was successful
[ 9299.564256] pcd_platform_driver_init : pcd platform driver loaded
[ 9448.738942] pcd_platform_driver_remove : Device is removed
[ 9448.739054] pcd_platform_driver_remove : Device is removed
[ 9448.739125] pcd_platform_driver_remove : Device is removed
[ 9448.739172] pcd_platform_driver_remove : Device is removed
[ 9448.739195] pcd_platform_driver_cleanup : pcd platform driver unloaded
[ 9455.315129] pcdev_release : Device released
[ 9455.315154] pcdev_release : Device released
[ 9455.315176] pcdev_release : Device released
[ 9455.315194] pcdev_release : Device released
[ 9455.315196] pcdev_platform_exit : Device setup module unloaded
```
