#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/slab.h>

#include <linux/timer.h>

#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include<linux/workqueue.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is timer module");


MODULE_VERSION("0.01");






static int __init  entryFn(void){

	printk(KERN_INFO "SIMPLE MODULE");	
	return 0;
}
static void __exit exitFn(void){
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


