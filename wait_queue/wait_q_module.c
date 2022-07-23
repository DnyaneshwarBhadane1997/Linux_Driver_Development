#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/slab.h>





#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include<linux/workqueue.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is wait q module");


MODULE_VERSION("0.01");



static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int condition = 0;
static struct work_struct wrk;


static void work_handler(struct work_struct *work)
{
	printk("Waitqueue module handler %s\n", __FUNCTION__);
	msleep(5000);
	printk("Wake up the sleeping module\n");
	condition = 1;
	wake_up_interruptible(&my_wq);
}

static int __init  entryFn(void){

	printk("Start of the module123");	
	printk("Wait queue example\n");
	INIT_WORK(&wrk, work_handler);
	schedule_work(&wrk);
	printk("Going to sleep1 %s\n", __FUNCTION__);

	printk("Going to sleep2 %s\n", __FUNCTION__);
	printk("Going to sleep3 %s\n", __FUNCTION__);
	printk("Going to sleep4 %s\n", __FUNCTION__);

	wait_event_interruptible(my_wq, condition != 0);
	pr_info("woken up by the work job\n");
	
	return 0;
}
static void __exit exitFn(void){
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


