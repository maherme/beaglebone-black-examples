# SD Booting Example

In this example the BBB will be booted using an SD card with a BOOT partition (FAT16) with an u-boot.img, MLO, linux kernel image (uImage) and device tree files. The u-boot will load the linux kernel and the device stored in the BOOT partition and will mount the root file system stored in the ROOTFS partition, also created in the SD card.

For this example I will use this versions:
| Software               | Version        |
|:----------------------:|:--------------:|
| u-boot                 | v2020.10       |
| linux                  | 5.10.87-ti-r33 |
| gcc-linaro-gnueabihf   | 12.0.0-2022.01 |

## Compiling u-boot

For removing previously compiled/generated object files:
```console
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
```
For applying the default configuration for beaglebone black:
```console
make CROSS_COMPILE=arm-linux-gnueabihf- am335x_evm_defconfig
```
If you want to configure any u-boot option:
```console
make CROSS_COMPILE=arm-linux-gnueabihf-  menuconfig
```
For compiling (using 4 threads, for other number of threads change -j4 option):
```console
make CROSS_COMPILE=arm-linux-gnueabihf- -j4
```

## Compiling linux

For removing previously compiled/generated object files:
```console
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
```
For applying the default configuration:
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- omap2plus_defconfig
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
## Preparing SD

You need to create a BOOT partition format as FAT16 and a ROOTF partition format as EXT3.

Copy into the BOOT partition the MLO and the u-boot.img files generated from u-boot compilation process, you need to copy the uImage and the am335x-boneblack.dtb also. Finally copy the [extlinux.conf](extlinux/extlinux.conf) file from this repository into a named folder extlinux, in the BOOT partition also (BOOT/extlinux/extlinux.conf).

Copy the root file system you want to use into the ROOTFS partition (for example, you can use root file system generated with [busybox](https://github.com/mirror/busybox) or using [buildroot](https://github.com/buildroot/buildroot)).

## Compiling Busybox

Refer to this [link](https://github.com/maherme/beaglebone-black-examples/tree/master/busybox#compiling-busybox)

## Compiling Buildroot

For getting the project:
```console
git clone git://git.buildroot.net/buildroot
```

Configuring for Beaglebone Black:
```console
make beaglebone_defconfig
```

Configuring extra settings:
```cosole
make menuconfig
```
Here you can disable the linux kernel and bootloader compilations. For taget options you can select NEON as floating point strategy and Thumb2 as ARM instruction set. And you need to select the filesystem images, ext2/3/4 variant (ext4), and select the format you use in the ROOTFS in your SD card.

For compiling:
```console
make
```

For copying the output root file system into the SD card (you can use ```lsblk``` command for knowing the mmc device in your PC):
```console
sudo dd if=buildroot/output/images/rootfs.ext4 of=/dev/mmcblk0p2
```
