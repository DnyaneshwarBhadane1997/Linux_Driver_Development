obj-m += container_of_kernel.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod container_of_kernel.ko
	sudo rmmod  container_of_kernel.ko
	dmesg

