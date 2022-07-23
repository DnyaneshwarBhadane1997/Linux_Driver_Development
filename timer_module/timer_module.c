#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include <linux/timer.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");
MODULE_DESCRIPTION("This is timer module");
MODULE_VERSION("0.01");

static struct timer_list my_timer;

void my_timer_callback(struct timer_list *timer){

	printk("%s called (%ld).\n", __FUNCTION__, jiffies);
}

static int __init  entryFn(void){

	int retval;
	printk(KERN_INFO "MODULE STARTED .............................\n");
	printk(KERN_INFO "Timer module loaded\n"); 
	timer_setup(&my_timer,  my_timer_callback , 0);


	printk("Setup timer to fire in 300ms (%ld)\n", jiffies);
	retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(5000) );

	if (retval)
		printk("Timer firing failed\n");
	return 0;
}
static void __exit exitFn(void){


	int retval;
	retval = del_timer(&my_timer);
	/* Is timer still active (1) or no (0) */
	if (retval)
		printk("The timer is still in use...\n");
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


