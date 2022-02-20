# Hello World kernel driver

This is a simple example of a kernel driver.

You can compile using:
```console
make ARCH=arm CROSS_COMPILE=<compiler_path>/arm-linux-gnueabihf- -C <linux_path> M=$PWD modules
```
or using the custom Makefile, you can also compile for your host machine using:
```console
make host
```
Note you could need to install kernel-devel package for your host machine, i.e:
```console
sudo dnf install kernel-devel.x86_64
```

You can see some information of compiled driver using:
```console
modinfo main.ko
```
```console
<compiler_path>/arm-linux-gnueabihf-objdump -h main.ko
```
## Adding in-tree module to kernel menu configuration:
- Create a folder inside linux repository, in drivers/char/<your_name>, for exaple drivers/char/foo.
- Copy [main.c](main.c) inside this folder.
- Copy [Kconfig](Kconfig) inside this folder.
- Create a Makefile with the following line:
  ```make
  obj-$(CONFIG_CUSTOM_HELLOWORLD) += main.o
  ```
- Add the local Kconfig entry to upper level Kconfig:
  ```
  source "drivers/char/foo/Kconfig"
  ```
- Modify the upper Makefile (in drivers/char/) adding:
  ```make
  obj-y +=.my_c_dev/
  ```
Then you can access to this kernel module executing:
```console
make ARCH=arm menuconfig
```
The new module will be in Device Drivers ---> Character devices ---> maherme custom modules.
