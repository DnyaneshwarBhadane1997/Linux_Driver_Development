#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include <linux/interrupt.h>



#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>
 
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is timer module");


MODULE_VERSION("0.01");



void tasklet_fn(unsigned long); 
/* Init the Tasklet by Static Method */
// DECLARE_TASKLET_OLD(tasklet,tasklet_fn);

//char tasklet_data[] = "We use a string; but it could be pointer to a structure";


/* void tasklet_work(unsigned long data){

	printk("%s\n" , (char *)data);
} */ 

DECLARE_TASKLET_OLD(tasklet,tasklet_fn);

void tasklet_fn(unsigned long arg)
{
        printk(KERN_INFO "Executing Tasklet Function : arg = %ld\n", arg);
}
 
 
//Interrupt handler for IRQ 11. 
static irqreturn_t irq_handler(void) {
        printk(KERN_INFO "Shared IRQ: Called for the tasklet");
        /*Scheduling Task to Tasklet*/

        tasklet_schedule(&tasklet); 
        
        return IRQ_HANDLED;
}


//DECLARE_TASKLET(my_tasklet , tasklet_work , (unsigned long ) tasklet_data);
// DECLARE_TASKLET(my_tasklet, tasklet_work, (unsinged long ) tasklet_data);


static int __init  entryFn(void){

	irq_handler();
	printk(KERN_INFO "SIMPLE MODULE");	
	return 0;
}
static void __exit exitFn(void){

	tasklet_kill(&tasklet);
	 
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is timer module");


MODULE_VERSION("0.01");


/*


char tasklet_data[] = "We use a string; but it could be pointer to a structure";


void tasklet_work(unsigned long data){

	printk("%s\n" , (char *)data);
}




//DECLARE_TASKLET(my_tasklet , tasklet_work , (unsigned long ) tasklet_data);
DECLARE_TASKLET(my_tasklet, tasklet_work, (unsigned long)  tasklet_data);
static int __init  entryFn(void){

	tasklet_schedule(&my_tasklet);
	printk(KERN_INFO "SIMPLE MODULE");	
	return 0;
}
static void __exit exitFn(void){

	tasklet_kill(&my_tasklet);

	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);

*/
