#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This module to test the container_of");

MODULE_VERSION("0.01");
struct Person {
	int age;
	int salary;
	char *name;

};

static int __init  entryFn(void){
	struct Person somebody ;
	somebody.age =  20;
	void *agePtr =  &somebody.age;


	*agePtr
	*dev_id

	struct Person *pt;
       	pt  =  container_of(agePtr , struct Person , age);
	printk(KERN_INFO "%d", pt->age, pt->salary);
	printk(KERN_INFO "container of Module Entry\n");
	return 0;
}
static void __exit exitFn(void){
	printk(KERN_INFO "(********************************Module Exit\n");
}


module_init(entryFn);
module_exit(exitFn);


