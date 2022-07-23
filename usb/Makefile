obj-m += usbDVR.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod usbDVR.ko
	sudo rmmod  usbDVR.ko
	dmesg


