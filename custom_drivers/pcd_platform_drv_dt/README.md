# Pseudo Char Platform Driver Using Device Tree

You will need to have access to a linux kernel repository and U-Boot repository for testing this driver. For this example I will use this versions:
| Software               | Version        |
|:----------------------:|:--------------:|
| u-boot                 | v2019.04       |
| linux                  | 5.4.106+       |
| gcc-linaro-gnueabihf   | 6.4.1-2017.11  |

## Testing Process
- Place the [am335x-boneblack-pcd.dtsi](am335x-boneblack-pcd.dtsi) into the linux kernel repository, in /arch/arm/boot/dts folder.
- Include the file in am335x-boneblack.dts adding ```#include "am335x-boneblack-pcd.dtsi"```.
- Compile for generating the Device Tree Blob file (am335x-boneblack.dtb) using (you need to execute this command in the linux kernel root directory):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack.dtb
```
- Place this file in the BOOT partition of your device (i.e. in the BOOT partiton of the uSD card).
- Compile the platform driver:
```console
make all
```
- Place the generated kernel object into the Beaglebone Black, i.e. using scp command:
```console
scp *.ko debian@192.168.7.2:/home/debian/drivers
```
- Connect to the Beaglebone Black opening a terminal (i.e. minicom) and reboot it.
- After the reboot you can watch the devices detected in /sys/devices/platform directory.
- You can load the generated kernel modules using ```ìnsmod```, the probe function will be executed 4 times.
