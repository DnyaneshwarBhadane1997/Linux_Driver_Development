#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/list.h>
#include<linux/slab.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is list module");

MODULE_VERSION("0.01");


struct car {
	int doors;
	char *color;
	char *name;
	struct list_head list;

};


//struct list_head carlist;
//
struct list_head myCarList;
LIST_HEAD(myCarList);
 
static void setData(struct car *self , int door , char *color , char * name ){
	self->doors = door;
	self->color =  color;
	self->name = name; 

}
static void traverseListAndPrintData(void){
	struct car *acar;
	list_for_each_entry(acar , &myCarList , list){
		printk(KERN_INFO "CAR NAME123 : %s \n CAR COLOR %s \n and CAR DOORS :  %d \n", acar-> name ,  acar ->color ,  acar->doors);
	
	}
}
static int __init  entryFn(void){
        struct car *blackcar = kzalloc(sizeof(struct car), GFP_KERNEL);
        struct car *bluecar = kzalloc(sizeof(struct car), GFP_KERNEL);

	
	setData(blackcar,  4,"black", "hundai" );
	setData(bluecar ,  4, "blue" ,"BMW"); 
	INIT_LIST_HEAD(&blackcar->list);
	INIT_LIST_HEAD(&bluecar->list);
	list_add_tail(&blackcar->list, &myCarList);
	list_add_tail(&bluecar->list, &myCarList);
	list_add_tail(&blackcar->list, &myCarList);
	list_add_tail(&bluecar->list, &myCarList);

	list_add_tail(&blackcar->list, &myCarList);
	list_add_tail(&bluecar->list, &myCarList);

	list_add_tail(&blackcar->list, &myCarList);
	list_add_tail(&bluecar->list, &myCarList);

	traverseListAndPrintData();
	printk(KERN_INFO "this car name is :  %s" ,  blackcar->name);
	return 0;
}
static void __exit exitFn(void){
	printk(KERN_INFO "(********************************Module Exit\n");
}




module_init(entryFn);
module_exit(exitFn);


