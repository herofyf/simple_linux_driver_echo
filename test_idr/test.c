#include <linux/idr.h>
#include "../common.h"

MODULE_LICENSE("GPL");

struct student {
	int id;
	char * name;
};

int print_idr_item(int id, void *p, void *data) {
	
	struct student *ps = (struct student *)(p);
	if (ps) {
		printk(KERN_INFO "idr id = %d, val = %d\n", id, ps->id);
	}	

	// pls don't return nonzero, it causes break
	return 0;
}


static int __init testidr_init(void) {
	DEFINE_IDR(idp);

	// to define 4 students pointers
	struct student *stu[4];
	
	int id, ret, i;
	
	// init 4 struct student
	for (i = 0; i < 4; i ++) {
		stu[i] = kmalloc(sizeof(struct student), GFP_KERNEL);
		stu[i]->id = i + 2;
		stu[i]->name = "wyb";
	}

	// add 4 students to idr
	print_current_time(0);
	for (i = 0; i < 4; i ++) {
		do {
			if (idr_pre_get(&idp, GFP_KERNEL) == 0)
				return -ENOMEM;
			
			id = idr_alloc(&idp, stu[i], 0, 0, GFP_KERNEL);
			printk(KERN_INFO "Got id = %d for stu %d\n", id, i);		
		} while(id == -EAGAIN);
	}

	idr_for_each(&idp, print_idr_item, NULL);	
	
	idr_destroy(&idp);
	for (i = 0; i < 4; i ++) {
		kfree(stu[i]);	
	}	

	printk(KERN_INFO "test idr init done.\n");
}


static void __exit testidr_exit() {

}


module_init(testidr_init);
module_exit(testidr_exit);
