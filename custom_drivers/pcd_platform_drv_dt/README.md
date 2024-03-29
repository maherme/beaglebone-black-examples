# Pseudo Char Platform Driver Using Device Tree
This is a similar driver to [pcd_platform_drv](../pcd_platform_drv), but here I am using the device tree for abstracting some features of the driver as the serial number, the memory size and the permissions of each device. You can find the details in the [arm335x-boneblack-pcd.dtsi](arm335x-boneblack-pcd.dtsi) file:
|                    | pcdev-0      | pcdev-1      | pcdev-2      | pcdev-3      |
|:------------------:|:------------:|:------------:|:------------:|:------------:|
| Size (bytes)       | 512          | 1024         | 256          | 2048         |
| Serial Number      | PCDEV1ABC123 | PCDEV2ABC456 | PCDEV3ABC789 | PCDEV4ABC000 |
| Access Permissions | RDWR         | RDWR         | RDWR         | RDWR         |

The binding between the nodes in the dtsi file and the driver is done using a compatibility struct, in the example is this:
```c
struct of_device_id org_pcdev_dt_match[] = {
    {.compatible = "pcdev-A1x", .data = (void*)PCDEVA1X},
    {.compatible = "pcdev-B1x", .data = (void*)PCDEVB1X},
    {.compatible = "pcdev-C1x", .data = (void*)PCDEVC1X},
    {.compatible = "pcdev-D1x", .data = (void*)PCDEVD1X},
    {}
};
```

So if a node in the dtsi has a compatible field which matches with one in the of_device_id struct, the probe function is called.

This struct needs to be included in the platform driver struct in order to be used by the kernel:
```c
struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .id_table = pcdevs_ids,
    .driver = {
        .name = "pseudo-char-device",
        .of_match_table = of_match_ptr(org_pcdev_dt_match)
    }
};
```

For a deep diving in the binding process you can explore the [platform_match](https://elixir.bootlin.com/linux/latest/source/drivers/base/platform.c#L1335) function in the kernel, placed in drivers/base/platform.c file.

---
**NOTE**

For device tree functionality was provided by the kernel, the CONFIG_OF option must be set. You can use menuconfig for setting, but it is set by default.
You can explore this macro in the [of.h](https://elixir.bootlin.com/linux/latest/source/include/linux/of.h#L158) kernel file placed in include/linux/of.h.

---

## Compile

You will need to have access to a linux kernel repository for compiling the device tree:

- Place the [am335x-boneblack-pcd.dtsi](am335x-boneblack-pcd.dtsi) into the linux kernel repository, in /arch/arm/boot/dts folder.
- Include the file in am335x-boneblack.dts adding ```#include "am335x-boneblack-pcd.dtsi"```.
- Compile for generating the Device Tree Blob file (am335x-boneblack.dtb) using (you need to execute this command in the linux kernel root directory):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack.dtb
```

- Compile the platform driver:
```console
make all
```

## Test

For testing I used these versions:

| Software               | Version        |
|:----------------------:|:--------------:|
| u-boot                 | v2019.04       |
| linux                  | 5.4.106+       |
| gcc-linaro-gnueabihf   | 6.4.1-2017.11  |

- Place the am335x-boneblack.dtb file in the BOOT partition of your device (i.e. in the BOOT partiton of the uSD card).
- Place the generated kernel object into the Beaglebone Black, i.e. using scp command:
```console
scp *.ko debian@192.168.7.2:/home/debian/drivers
```
- Connect to the Beaglebone Black opening a terminal (i.e. minicom) and reboot it.
- After the reboot you can watch the devices detected in /sys/devices/platform directory.
- You can load the generated kernel modules using ```ìnsmod```, the probe function will be executed 4 times.

The device tree is deployed in /sys/firmware/devicetree/base directory:
```console
# ls /sys/firmware/devicetree/base/
#address-cells    compatible        model             pcdev-3
#size-cells       cpus              name              pcdev-4
aliases           fixedregulator0   ocp               pmu@4b000000
chosen            interrupt-parent  opp-table         serial-number
clk_mcasp0        leds              pcdev-1           soc
clk_mcasp0_fixed  memory@80000000   pcdev-2           sound
```

And also in /proc/device-tree directory:
```console
# ls /proc/device-tree/
#address-cells    compatible        model             pcdev-3
#size-cells       cpus              name              pcdev-4
aliases           fixedregulator0   ocp               pmu@4b000000
chosen            interrupt-parent  opp-table         serial-number
clk_mcasp0        leds              pcdev-1           soc
clk_mcasp0_fixed  memory@80000000   pcdev-2           sound
```

The /proc/device-tree is a symbolic link to /sys/firmware/devicetree/base so from the user space is better to use the /proc location.  
You can access the device tree node properties through this directory:
```console
# ls /proc/device-tree/pcdev-1/
compatible             org,device-serial-num  org,size
name                   org,perm
# cat /proc/device-tree/pcdev-1/name; echo
pcdev-1
# cat /proc/device-tree/pcdev-1/compatible; echo
pcdev-A1x
# hexdump -C /proc/device-tree/pcdev-1/org,perm
00000000  00 00 00 11                                       |....|
00000004
# hexdump -C /proc/device-tree/pcdev-1/org,size 
00000000  00 00 02 00                                       |....|
00000004
```
