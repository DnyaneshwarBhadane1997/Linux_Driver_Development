obj-m += tasklet_module.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod tasklet_module.ko
	sudo rmmod  tasklet_module.ko
	dmesg

