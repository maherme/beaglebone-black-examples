# Hello World kernel driver

This is a simple example of a kernel driver.

You can compile using:
```console
make ARCH=arm CROSS_COMPILE=<compiler_path>/arm-linux-gnueabihf- -C <linux_path> M=$PWD modules
```

You can see some information of compiled driver using:
```console
modinfo main.ko
```
```console
<compiler_path>/arm-linux-gnueabihf-objdump -h main.ko
```
