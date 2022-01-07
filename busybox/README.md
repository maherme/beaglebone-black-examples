# Busybox Example

In this example the BBB will be booted using an SD card with a BOOT partition (FAT16) with an u-boot.img and an MLO files. The u-boot will load the linux kernel and the device tree using the TFTP protocol and the root file system, generated using busybox, will be mounted using a NFS server.

For this example I will use this versions:
| Software               | Version        |
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
## Preparing SD

You need to create a BOOT partition format as FAT16 and copy there the MLO and the u-boot.img files generated from u-boot compilation process, you need to copy the [uEnv.txt](uEnv.txt) file from this repository also.

## TFTP Server

In Fedora distribution TFTP server is already installed, you will need to copy am335x-boneblack.dtb and uImage in ```/var/lib/tftpboot``` directory (if does not exist you will need to create it). These files are generated as output of the linux compilation process, you can find the uImage in ```arch/arm/boot``` and the am335x-boneblack.dtb in ```arch/arm/boot/dts```

## NFS Server

You will need to install NFS server, in Fedora distribution:
```console
sudo dnf install nfs-utils
```

You also need to create a folder for storing the root file system, for example ```/srv/nfs/bbb``` and give permissons for all users:
```console
sudo chmod -R 777 /srv/nfs/bbb
```

For sharing this folder through the NFS sever you have to modify the ```/etc/exports``` file adding the line: ```/srv/nfs/bbb 192.168.7.2(rw,sync,no_root_squash,no_subtree_check)```

For making this change effective run:
```console
sudo exportfs -arv
```
And restart the NFS server:
```console
sudo systemctl restart nfs-server.service
```

Maybe you will need to add some exceptions to the firewall:
```console
sudo firewall-cmd --permanent --zone public --add-service rpc-bind
sudo firewall-cmd --permanent --zone public --add-service nfs
sudo firewall-cmd --reload
```

After that you need to copy the generated root file system by busybox with installed kernel modules into the ```/srv/nfs/bbb``` folder, and create a ```/dev``` and ```/proc``` directories into the root file system to avoid some errors when the Beaglebone Black mounts the root file system.
