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
MODULE_DESCRIPTION("This is demo module");
MODULE_VERSION("0.01");


//static struct delayed_work list_files;
//
static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static struct work_struct wrk;
static int c =0;
static void list_items(struct work_struct *work){

	char *cmd = "ls";
	char *argv[]= {cmd , "-r"};
	char *envp[] = {"HOME=/","PATH=/sbin:/bin:/usr/sbin:/usr/bin",NULL,};

	call_usermodehelper(cmd, argv, envp, 0);
	c =1;
	wake_up_interruptible(&my_wq);
}


static int __init  entryFn(void){

	printk(KERN_INFO "SIMPLE MODULE\n");	

	INIT_WORK(&wrk, list_items);
	schedule_work(&wrk);

	wait_event_interruptible(my_wq ,  c != 0);
	printk(KERN_INFO  "ININT COMPLITED");
	return 0;
}
static void __exit exitFn(void){
	
	printk(KERN_INFO "(********************************Module Exit\n");
}

module_init(entryFn);
module_exit(exitFn);


