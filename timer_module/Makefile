obj-m += timer_module.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C ${KDIR} M=$(PWD) modules
clean:
	make -C ${KDIR} M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod timer_module.ko
	sudo rmmod  timer_module.ko
	dmesg

