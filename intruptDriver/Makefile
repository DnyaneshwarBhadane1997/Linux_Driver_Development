obj-m += itr.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod itr.ko
	sudo cat /dev/etx_device
	sudo rmmod  itr.ko
