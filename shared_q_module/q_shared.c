
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/workqueue.h>

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is shared work q  module");

MODULE_VERSION("0.01");

static int sleep = 0;
struct work_data {
	struct work_struct my_work;
	wait_queue_head_t my_wq;
	int the_data;
};
static void work_handler(struct work_struct *work)
{

	
	struct work_data *my_data = container_of(work, 
	struct work_data, my_work);
	printk("Work queue module handler: %s, data is %d\n", __FUNCTION__,
	my_data->the_data);
	msleep(2000);
	wake_up_interruptible(&my_data->my_wq);
	kfree(my_data);
 }   


static int __init  entryFn(void){
	struct work_data * my_data;
	printk(KERN_INFO "it is inside of the drama ");
	my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data->the_data = 34;


	printk(KERN_INFO "it hasstartt ");
	INIT_WORK(&my_data->my_work, work_handler);
	init_waitqueue_head(&my_data->my_wq);

	schedule_work(&my_data->my_work);
	printk("I'm going to sleep ...\n");
	
	wait_event_interruptible(my_data->my_wq, sleep != 0);
	printk("I am Waked up...\n");
	 printk(KERN_INFO "SIMPLE MODULE");	
	return 0;
}
static void __exit exitFn(void){



	printk("Work queue module exit: %s %d\n", __FUNCTION__, __LINE__);

	
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


