# Pseudo Char Platfor Driver Using Device Tree Overlay

This example is based on [pcd_platform_drv_dt](../pcd_platform_drv_dt) example. The same .c files are used for this example.

For compiling the device tree overlay:
```console
dtc -@ -I dts -O dtb -o PCDEV0.dtbo PCDEV0.dts
dtc -@ -I dts -O dtb -o PCDEV1.dtbo PCDEV1.dts
```

For testing:
- You need to replace the [am335x-boneblack-pcd.dtsi](am335x-boneblack-pcd.dtsi) and compile the .dtb file again (look in [pcd_platform_drv_dt](../pcd_platform_drv_dt) example).
- Copy the [uEnv.txt](uEnv.txt) file into the BOOT parition of your device (i.e. in the BOOT partiton of the uSD card).
- Copy the .dtbo files into the /lib/firmware/ folder in the ROOT partition of you device.
- Reboot the Beaglebone Black, you should watch a message in the boot log indicating that an overlay is applied.
- You can check the driver who is started watching the devices is /sys/devices/platform/ (only pcdev-4 should appear here).
