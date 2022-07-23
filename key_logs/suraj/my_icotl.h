#ifndef MY_IOCTL
#define MY_IOCTL


#define MYDEV_IOC_MAGIC 'C'

#define SAY_HELLO   _IO(MYDEV_IOC_MAGIC, 0)
#define START_COUNTDWN  _IOW(MYDEV_IOC_MAGIC, 1, unsigned long *)

#define READ_LAST_1000_WORDS _IOR(MYDEV_IOC_MAGIC, 2, unsigned long *)
#endif

