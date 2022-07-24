#include "all_includes.h"
#include "custom_ioctl_header.h"
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/mutex.h>
#include <linux/usb.h>
#include <asm/hw_irq.h>
#include <linux/timer.h>
#include "linux/sched/signal.h"
#define IRQ_NO 11
#define SIGETX 44

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");
MODULE_DESCRIPTION("This is presentation driver");
MODULE_VERSION("0.01");
static struct usb_device *device;
 
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
    printk(KERN_INFO  "THis is my driver print this data on probe");
    
    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "Pen i/f %d now probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);
 
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;
 
        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
    }
 
    device = interface_to_usbdev(interface);
    return 0;
}
 
static void pen_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen i/f %d now disconnected\n",
            interface->cur_altsetting->desc.bInterfaceNumber);
}
 
 
static struct usb_device_id pen_table[] =
{
     { USB_DEVICE(0x0781, 0x556b)},
     {USB_DEVICE(0x2e82 ,0x22b8)},
    { USB_DEVICE(0x22b8, 0x2e82)},
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);
 
static struct usb_driver pen_driver =
{
    .name = "pen_driver",
    .probe = pen_probe,
    .disconnect = pen_disconnect,
    .id_table = pen_table,
};

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

DECLARE_TASKLET_OLD(tasklet,tasklet_fn);

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
void tasklet_fn(unsigned long);

/* Init the Tasklet by Static Method */
// DECLARE_TASKLET_OLD(tasklet,tasklet_fn);

void tasklet_fn(unsigned long data){

        printk(KERN_INFO "NO DATA %s\n" , tasklet_data);
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

static struct timer_list my_timer;

void my_timer_callback(struct timer_list *timer){

	printk("%s called (%ld).\n", __FUNCTION__, jiffies);
}
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
                case CREATE_TIMER:
                        mod_timer( &my_timer, jiffies + msecs_to_jiffies(5000));
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
        
        usb_register(&pen_driver);
        
        timer_setup(&my_timer,  my_timer_callback , 0);
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
        usb_deregister(&pen_driver);
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


