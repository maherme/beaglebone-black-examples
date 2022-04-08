#Pseudo Char Driver

For compiling you can use:
´´´console
make host
´´´

For loading the kernel object:
```console
sudo insmod pcd.ko 
```

You can watch the printed information using (this command shows the last 5 lines of dmesg output):
```console
dmesg | tail -5
```

For testing the write function:
```console
echo "Hello, I am a linux user" > /dev/pcd 
dmesg | tail -8
```

You will get an output like this:
```console
[  474.722033] pcd_driver_init : Device number <major>:<minor> = 234:0
[  474.722377] pcd_driver_init : Module init was successful
[  976.007133] pcd_open : open was successful
[  976.007214] pcd_write : write requested for 25 bytes
[  976.007223] pcd_write : current file position = 0
[  976.007232] pcd_write : number of bytes successfully written = 25
[  976.007239] pcd_write : updated file position = 25
[  976.007293] pcd_release : release was successful
```

You can check the read function:
```console
cat /dev/pcd 
dmesg | tail -10
```

You will get an output like this:
```console
[ 1193.243847] pcd_open : open was successful
[ 1193.243862] pcd_read : read requested for 131072 bytes
[ 1193.243864] pcd_read : current file position = 0
[ 1193.243866] pcd_read : number of bytes successfully read = 512
[ 1193.243867] pcd_read : updated file position = 512
[ 1193.243879] pcd_read : read requested for 131072 bytes
[ 1193.243880] pcd_read : current file position = 512
[ 1193.243881] pcd_read : number of bytes successfully read = 0
[ 1193.243882] pcd_read : updated file position = 512
[ 1193.243890] pcd_release : release was successful
```
