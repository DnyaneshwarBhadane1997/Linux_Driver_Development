#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h> /* for work queue */
#include <linux/slab.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("DNYANESHWAR BHADANE");

MODULE_DESCRIPTION("This is timer module");


MODULE_VERSION("0.01");


struct workqueue_struct *wq;
struct work_data {
	struct work_struct my_work;
	int the_data;
};

static void work_handler(struct work_struct *work){
	struct work_data * my_data = container_of(work,struct work_data, my_work);
	printk("Work queue module handler: %s, data is %d\n",__FUNCTION__, my_data->the_data);
	kfree(my_data);
}

static int __init  entryFn(void){

	struct work_data * my_data;
	struct work_data * my_data1;
	printk("Work queue module init: %s %d\n",__FUNCTION__, __LINE__);
	wq = create_singlethread_workqueue("my_single_thread");
	my_data = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data->the_data = 34;
	
	my_data1 = kmalloc(sizeof(struct work_data), GFP_KERNEL);
	my_data1->the_data = 55;

	
	INIT_WORK(&my_data->my_work, work_handler);
	queue_work(wq, &my_data->my_work);

	INIT_WORK(&my_data1->my_work, work_handler);
	queue_work(wq, &my_data1->my_work);


	return 0;
}
static void __exit exitFn(void){
	flush_workqueue(wq);
	destroy_workqueue(wq);
	printk("Work queue module exit: %s %d\n",__FUNCTION__, __LINE__);
}

module_init(entryFn);
module_exit(exitFn);


