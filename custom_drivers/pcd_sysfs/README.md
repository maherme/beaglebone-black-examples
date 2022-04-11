# Pseudo Char Driver Sysfs

In this example two sysfs attributes are created:
- max_size (world-read/owner-write): use this attribute for reading or changing the value of size information.
- serial_num (world-read): use this attribute for reading the serial number of the device.

## For testing:
- Load the pcd_device_setup kernel object from [pcd_platform_drv](../pcd_platform_drv).
```console
sudo insmod pcd_device_setup.ko 
```
- Compile and load the pcd_sysfs kernel object:
```console
make host
sudo insmod pcd_sysfs.ko
```
- Using ```dmesg``` you can see the probe function is called:
```console
[  882.674813] pseudo-char-device pcdev-A1x.0: A device is detected
[  882.674819] pseudo-char-device pcdev-A1x.0: Device serial number = PCDEVABC1111
[  882.674821] pseudo-char-device pcdev-A1x.0: Device size = 512
[  882.674823] pseudo-char-device pcdev-A1x.0: Device permission = 17
[  882.674825] pseudo-char-device pcdev-A1x.0: Config item 1 = 60
[  882.674826] pseudo-char-device pcdev-A1x.0: Config item 2 = 21
[  882.675163] pseudo-char-device pcdev-A1x.0: Probe was successful
[  882.675179] pseudo-char-device pcdev-B1x.1: A device is detected
[  882.675181] pseudo-char-device pcdev-B1x.1: Device serial number = PCDEVXYZ2222
[  882.675183] pseudo-char-device pcdev-B1x.1: Device size = 1024
[  882.675184] pseudo-char-device pcdev-B1x.1: Device permission = 17
[  882.675185] pseudo-char-device pcdev-B1x.1: Config item 1 = 50
[  882.675187] pseudo-char-device pcdev-B1x.1: Config item 2 = 22
[  882.675235] pseudo-char-device pcdev-B1x.1: Probe was successful
[  882.675250] pseudo-char-device pcdev-C1x.2: A device is detected
[  882.675252] pseudo-char-device pcdev-C1x.2: Device serial number = PCDEVXYZ3333
[  882.675253] pseudo-char-device pcdev-C1x.2: Device size = 128
[  882.675255] pseudo-char-device pcdev-C1x.2: Device permission = 1
[  882.675257] pseudo-char-device pcdev-C1x.2: Config item 1 = 40
[  882.675258] pseudo-char-device pcdev-C1x.2: Config item 2 = 23
[  882.675294] pseudo-char-device pcdev-C1x.2: Probe was successful
[  882.675305] pseudo-char-device pcdev-D1x.3: A device is detected
[  882.675306] pseudo-char-device pcdev-D1x.3: Device serial number = PCDEVXYZ4444
[  882.675307] pseudo-char-device pcdev-D1x.3: Device size = 32
[  882.675309] pseudo-char-device pcdev-D1x.3: Device permission = 16
[  882.675310] pseudo-char-device pcdev-D1x.3: Config item 1 = 30
[  882.675312] pseudo-char-device pcdev-D1x.3: Config item 2 = 24
[  882.675343] pseudo-char-device pcdev-D1x.3: Probe was successful
[  882.675359] pcd_platform_driver_init : pcd platform driver loaded
```
- You can watch the created attributes in /sys/class/pcd_class :
```console
[maherme@localhost pcdev-0]$ pwd
/sys/class/pcd_class/pcdev-0
[maherme@localhost pcdev-0]$ ls
dev  device  max_size  power  serial_num  subsystem  uevent
```
- You can use ```cat``` for consulting attribute information:
```console
[maherme@localhost pcdev-0]$ cat serial_num 
PCDEVABC1111
[maherme@localhost pcdev-0]$ cat max_size 
512
```
- You can use ```echo``` for setting a new attribute value:
```console
[root@localhost pcdev-0]# ls
dev  device  max_size  power  serial_num  subsystem  uevent
[root@localhost pcdev-0]# echo 5 > max_size 
[root@localhost pcdev-0]# cat max_size 
5
[root@localhost pcdev-0]# echo "hello driver" > /dev/pcdev-0
bash: echo: write error: Cannot allocate memory
```
