# GPIO Sysfs Driver

This is a driver for controlling the GPIOs through the Sysfs interface.
The driver supports the below functionality:
- It creates a class "bone_gpios" under /sys/class. This is done using the kernel function [class_create](https://elixir.bootlin.com/linux/latest/C/ident/class_create).
- For every detected GPIO in the device tree, it creates a device under /sys/class/bone_gpios. This is done using the kernel function [device_create_with_groups](https://elixir.bootlin.com/linux/latest/A/ident/device_create_with_groups). I needed to create an array of attributes group for the ```const struct attribute_group **groups``` parameter:
  ```c
  static const struct attribute_group* gpio_attr_groups[] = {
    &gpio_attr_group,
    NULL
  };
  ```
- It creates three sysfs files (attributes) for every device:
  - direction: used to configure the GPIO direction ("in" or "out") with read/write permissions.
  - value: used to get the state of the GPIO or to write a new value to the GPIO ("0" or "1") with read/write permissions.
  - label: used to get the label of the GPIO with read only permissions.
- It implements a show and store methods for the attributes.

For knowing the number of GPIO in the device tree I use the [for_each_available_child_of_node](https://elixir.bootlin.com/linux/latest/C/ident/of_get_available_child_count) function.  
For getting the GPIO descriptor I use the [devm_fwnode_get_gpiod_from_child](https://elixir.bootlin.com/linux/v6.1.55/C/ident/devm_fwnode_get_gpiod_from_child) function instead of [gpiod_get](https://elixir.bootlin.com/linux/latest/C/ident/gpiod_get).

## Compile

- Be sure to disable the leds block in the am335x-bone-common.dtsi file, placed in <linux root dir>/arch/arm/boot/dts/ :
```
leds {
    pinctrl-names = "default";
    pinctrl-0 = <&user_leds_s0>;

    compatible = "gpio-leds";
/*
    led2 {
        label = "beaglebone:green:usr0";
        gpios = <&gpio1 21 GPIO_ACTIVE_HIGH>;
        linux,default-trigger = "heartbeat";
        default-state = "off";
    };

    led3 {
        label = "beaglebone:green:usr1";
        gpios = <&gpio1 22 GPIO_ACTIVE_HIGH>;
        linux,default-trigger = "mmc0";
        default-state = "off";
    };

    led4 {
        label = "beaglebone:green:usr2";
        gpios = <&gpio1 23 GPIO_ACTIVE_HIGH>;
        linux,default-trigger = "cpu0";
        default-state = "off";
    };
*/
    led5 {
        label = "beaglebone:green:usr3";
        gpios = <&gpio1 24 GPIO_ACTIVE_HIGH>;
        linux,default-trigger = "mmc1";
        default-state = "off";
    };
};
```
- Put the [am335x-boneblack-pcd.dtsi](am335x-boneblack-pcd.dtsi) in the linux folder <linux root dir>/arch/arm/boot/dts/ .
- Compile for generating the Device Tree Blob file (am335x-boneblack.dtb) using (you need to execute this command in the linux kernel root directory):
```console
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack.dtb
```
- Place this file in the BOOT partition of your device (i.e. in the BOOT partiton of the uSD card).
- Compile the platform driver:
```cosole
make all
```

## Test

- Place the generated kernel object into the Beaglebone Black, i.e. using scp command:
```console
scp *.ko debian@192.168.7.2:/home/debian/drivers
```
- Connect to the Beaglebone Black opening a terminal (i.e. minicom) and reboot it.
- After the reboot you can watch the device bone_gpio_devs detected in /sys/devices/platform directory:
```console
debian@beaglebone:~$ ls /sys/devices/platform/
 4b000000.pmu        fixedregulator0   pcdev-2   reg-dummy       ti-cpufreq
 bone_gpio_devs      leds              pcdev-3   serial8250      uevent
 clk_mcasp0          ocp               pcdev-4   snd-soc-dummy
 cpufreq-dt          opp-table         pm33xx    soc
'Fixed MDIO bus.0'   pcdev-1           power     sound
```
- You can load the generated kernel modules using ```ìnsmod```:
```console
root@beaglebone:/home/debian/drivers# insmod gpio_sysfs.ko 
[  981.566772] gpio_sysfs: loading out-of-tree module taints kernel.
[  981.593858] bone-gpio-sysfs bone_gpio_devs: Total devices found = 6
[  981.617496] bone-gpio-sysfs bone_gpio_devs: GPIO label = gpio2.2
[  981.633699] bone-gpio-sysfs bone_gpio_devs: GPIO label = gpio2.3
[  981.649563] bone-gpio-sysfs bone_gpio_devs: GPIO label = gpio2.4
[  981.669618] bone-gpio-sysfs bone_gpio_devs: GPIO label = usrled0:gpio1.21
[  981.685589] bone-gpio-sysfs bone_gpio_devs: GPIO label = usrled1:gpio2.22
[  981.705788] bone-gpio-sysfs bone_gpio_devs: GPIO label = usrled2:gpio1.23
[  981.725941] gpio_sysfs_init : Module load success
```
- Then you can go to /sys/class/bone_gpios/ and see the gpio for controlling:
```console
root@beaglebone:/sys/class/bone_gpios# ls
gpio2.2  gpio2.3  gpio2.4  usrled0:gpio1.21  usrled1:gpio2.22  usrled2:gpio1.23
```
- You can enter inside one of them and watch the attributes, read or write in them:
```console
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# ls
device  direction  label  power  subsystem  uevent  value
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# cat direction 
out
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# cat label 
usrled0:gpio1.21
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# cat value 
0
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# echo 1 > value 
root@beaglebone:/sys/class/bone_gpios/usrled0:gpio1.21# cat value 
1
```
