# Busybox Example

In this example the BBB will be booted using an SD card with a BOOT partition (FAT16) with an u-boot.img and an MLO files. The u-boot will load the linux kernel and the device tree using the TFTP protocol and the root file system, generated using busybox, will be mounted using a NFS server.

For this example I will use this versions:
|                        | Version        |
|:----------------------:|:--------------:|
| u-boot                 | v2017.05-rc2   |
| linux                  | 4.4.68-ti-r110 |
| busybox                | 1.35.0         |
| gcc-linaro-gnueabihf   | 5.4.1-2017.01  |

## Compiling u-boot

For removing previously compiled/generated object files:
```console
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
```
For applying the default configuration for beaglebone black (for newer versions of u-boot use am335x_evm_defconfig):
```console
make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
```
If you want to configure any u-boot option:
```console
make CROSS_COMPILE=arm-linux-gnueabihf-  menuconfig
```
For compiling (using 4 threads, for other number of threads change -j4 option):
```console
make CROSS_COMPILE=arm-linux-gnueabihf- -j4
```

## Compiling busybox

For applying the default configuration:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- defconfig
```
If you want to configure any busybox option (for example compiling as static library):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```
For generating the binary and the file system:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- CONFIG_PREFIX=<install_path> install
```

## Compiling linux

For removing previously compiled/generated object files:
```console
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
```
For applying the default configuration (for newer version use use omap2plus_defconfig):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bb.org_defconfig
```
If you want to configure any kernel option:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```
For compiling:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000 -j4
```
For generating the kernel modules:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 modules
```
For installing the kernel modules in the root file system (in this case the busybox compilation output):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=<path of the RFS> modules_install
```
