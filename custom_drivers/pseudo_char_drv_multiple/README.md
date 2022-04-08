# Pseudo Char Driver for Multiple Devices

For compiling the driver:
```console
make host
```
For loading the kernel object:
```console
sudo insmod pcd_n.ko
dmesg | tail -5
```

You will get an output like this:
```console
[ 5930.575731] pcd_driver_init : Device number <major>:<minor> = 234:0
[ 5930.576019] pcd_driver_init : Device number <major>:<minor> = 234:1
[ 5930.576069] pcd_driver_init : Device number <major>:<minor> = 234:2
[ 5930.576105] pcd_driver_init : Device number <major>:<minor> = 234:3
[ 5930.576140] pcd_driver_init : Module init was successful
```

You can list the created devices (it should be 4 devices):
```console
ls -l /dev/pcdev-*
```

Trying to write in device 1:
```console
echo hello > /dev/pcdev-1
dmesg | tail -2
```

You will get an error due to this device has only read permission:
```console
[ 5960.275978] pcd_open : minor access = 0
[ 5960.275985] pcd_open : open was unsuccessful
```

Other way to test the write permission in device 1:
```console
strace dd if=pcd_n.c of=/dev/pcdev-1
```

In the output of this command you will find this line:
```console
openat(AT_FDCWD, "/dev/pcdev-1", O_WRONLY|O_CREAT|O_TRUNC, 0666) = -1 EPERM (Operation not permitted)
```

You can try writing in device 2 (I use count=1 for avoiding to request more than once write process and generate an no memory error):
```console
dd if=pcd_n.c of=/dev/pcdev-2 count=1
```

You will get an output like this:
```console
1+0 records in
1+0 records out
512 bytes copied, 0.000130635 s, 3.9 MB/s
```

And using ```dmesg```:
```console
[ 6920.575835] pcd_open : minor access = 1
[ 6920.575842] pcd_open : open was successful
[ 6920.575867] pcd_write : write requested for 512 bytes
[ 6920.575869] pcd_write : current file position = 0
[ 6920.575871] pcd_write : number of bytes successfully written = 512
[ 6920.575873] pcd_write : updated file position = 512
[ 6920.575888] pcd_release : release was successful
```
