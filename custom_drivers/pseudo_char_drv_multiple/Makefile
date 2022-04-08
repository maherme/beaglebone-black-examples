obj-m := pcd_n.o

ARCH=arm
CROSS_COMPILE=/usr/bin/gcc-linaro-12.0.0-2022.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
KERN_DIR=~/Projects/BeagleBoneBlack/linux/
HOST_KERN_DIR=/lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean

help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help

host:
	make -C $(HOST_KERN_DIR) M=$(PWD) modules

test:
	gcc test_read.c -o test_read
