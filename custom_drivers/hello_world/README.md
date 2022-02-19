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
