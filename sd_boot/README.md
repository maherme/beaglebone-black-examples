# SD Booting Example

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
## Preparing SD

You need to create a BOOT partition format as FAT16 and a ROOTF partition format as EXT3.

Copy into the BOOT partition the MLO and the u-boot.img files generated from u-boot compilation process, you need to copy the uImage and the am335x-boneblack.dtb also. Finally copy the [extlinux.conf](extlinux/extlinux.conf) file from this repository into a named folder extlinux, in the BOOT partition also (BOOT/extlinux/extlinux.conf).

Copy the root file system you want to use into the ROOTFS partition (for example, you can use root file system generated with busybox).
