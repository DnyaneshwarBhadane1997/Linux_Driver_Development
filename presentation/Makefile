driverName=  lxDriver
testAppName= uTest
obj-m=${driverName}.o

KDIR = /lib/modules/${shell uname -r}/build
all:
	make -C ${KDIR}  M=${shell pwd} modules
clean:
	make -C ${KDIR}  M=${shell pwd} clean
install:
	sudo insmod ${driverName}.ko
logs:
	sudo dmesg -wH
utest:
	gcc ${testAppName}.c
	sudo ./a.out
remove:
	sudo rmmod ${driverName}


	
