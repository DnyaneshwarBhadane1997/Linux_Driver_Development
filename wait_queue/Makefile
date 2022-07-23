obj-m += wait_q_module.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod wait_q_module.ko
	sudo rmmod  wait_q_module.ko
	dmesg

