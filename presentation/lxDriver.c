#include "all_includes.h"
#include "custom_ioctl_header.h"
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/mutex.h>

#include <asm/hw_irq.h>

#include "linux/sched/signal.h"
#define IRQ_NO 11
#define SIGETX 44

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");
MODULE_DESCRIPTION("This is presentation driver");
MODULE_VERSION("0.01");

struct mutex etx_mutex;
static int __init entryFn(void);
static void __exit exitFn(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static void tasklet_fn(unsigned long);

//:wq
//static bool loggerFlag =  true;
int32_t value = 0;
static struct task_struct *task = NULL;
static int signum = 0;
char tasklet_data[] = "We use a string; but it could be pointer to a structure";

DECLARE_TASKLET(tasklet,tasklet_fn,(long unsigned int)tasklet_data);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
void tasklet_fn(unsigned long);

/* Init the Tasklet by Static Method */
// DECLARE_TASKLET_OLD(tasklet,tasklet_fn);

void tasklet_fn(unsigned long data){

        printk(KERN_INFO "NO DATA %s\n" , (char *)data);
} 

static irqreturn_t irq_handler(int irq,void *dev_id) {
    struct siginfo info;
    printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
    
    //Sending signal to app
    
    memset(&info, 0, sizeof(struct siginfo));
    info.si_signo = SIGETX;
    info.si_code = SI_QUEUE;
    info.si_int = 1;
 
    if (task != NULL) {
        printk(KERN_INFO "Sending signal to app\n");
        if(send_sig_info(SIGETX, (struct kernel_siginfo *)&info, task) < 0)  {
            printk(KERN_INFO "Unable to send signal\n");
        }
    }
 
    return IRQ_HANDLED;
}
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};

static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function\n");
	mutex_lock(&etx_mutex);
	asm("int $0x3B"); 
        mutex_unlock(&etx_mutex);
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        return len;
}
/*
** This function will be called when we write IOCTL on the Device file
*/
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WRITE_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("Value = %d\n", value);
                        break;
                case READ_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
		case REG_CURRENT_TASK:  
			printk(KERN_INFO "REG_CURRENT_TASK\n");
		        task = get_current();
		        signum = SIGETX;
			break;
		case CREATE_TASKLET:
		        tasklet_schedule(&tasklet);
			break;

                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}

static int __init  entryFn(void){


	printk(KERN_INFO "%s : Loading Module..............\n", __func__);

	if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        mutex_init(&etx_mutex);
//        tasklet_schedule(&tasklet); 

        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
	if (request_irq(IRQ_NO, 
	irq_handler, IRQF_SHARED, "etx_device", (void *)(irq_handler))) {
        	printk(KERN_INFO "my_device: cannot register IRQ ");
	        goto irq;
    	} 
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
irq:
    free_irq(IRQ_NO,(void *)(irq_handler));	
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
static void __exit exitFn(void){
        tasklet_kill(&tasklet);

	free_irq(IRQ_NO,(void *)(irq_handler));

	device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}


// Driver method configurations
// Dnyaneshwar Bhadane 
//

module_init(entryFn);
module_exit(exitFn);
// Documentation part

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");
MODULE_DESCRIPTION("This is presentation driver");
MODULE_VERSION("0.01");


