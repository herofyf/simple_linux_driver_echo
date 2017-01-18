#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/mutex.h>

#include "my_data.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Land fang");
MODULE_DESCRIPTION("To test kref function.");
MODULE_VERSION("0.1");

struct my_data _g_data;

// http://lxr.free-electrons.com/source/Documentation/printk-formats.txt?v=3.14


static int __init test_kref_init(void) {

	printk(KERN_INFO "test kref init.\n");

	kref_init(&_g_data._ref_count);
	mutex_init(&_g_data._mutex_sync);
	INIT_LIST_HEAD(&_g_data._list);
	//kref_get(&_g_data._ref_count);

	struct my_items *item = NULL;
	item = kmalloc(sizeof(*item), GFP_KERNEL);
	item->_val = 0;


	int i = 0;
	for (; i < 10; i ++) {
		item = kmalloc(sizeof(*item), GFP_KERNEL);
		item->_val = i;
		list_add(&item->_link, &_g_data._list);
	}

	timing_print(&_g_data);
	
	return 0;
}

struct pad_test {
	char x1[2];
	char x2[8];
	char x3[4];
};

struct pad_test1 {
	int x:2;
	int y: 4;
};

static void test_un_likely() {
	int b = 1;

	if (likely(b)) {
		printk(KERN_INFO "likely(b) done. \n");
	} else {
		printk(KERN_INFO "likely(b) done else \n");
	}

	if (unlikely(b)) {
		printk(KERN_INFO "unlikely(b) done.\n");
	} else {
		printk(KERN_INFO "unlikely(b) done  else \n");
	}
}


static void test_clang_struct() {

	int x = 1;
	if ( *(char *)&x == 1 ) {
		printk(KERN_INFO "Little endian!\n");
	} else {
		printk(KERN_INFO "Bigger endian!\n");
	}

	struct pad_test1 pad_t1;
	printk(KERN_INFO "pad_t1 sizeof = %d(%pK, %pK)\n", sizeof(pad_t1));

	int val1 = 0;
	printk(KERN_INFO "val1 address %pK\n", &val1);

	struct pad_test pad1;
	printk(KERN_INFO "pad1 address %pK(%pk, %pK, %pk)\n", &pad1, &pad1.x1, &pad1.x2, &pad1.x3);

	int val = 0;
	printk(KERN_INFO "val address %pK\n", &val);
}


static void release_my_data(struct kref *ref)
{


	printk(KERN_INFO "the ref to zero \n");
}

static int __exit test_kref_exit(void) {
	
	printk(KERN_INFO "ref count %u", _g_data._ref_count);
	//kref_put(&_g_data._ref_count, release_my_data);
	kref_put(&_g_data._ref_count, release_my_data);
	mutex_destroy(&_g_data._mutex_sync);

	printk(KERN_INFO "test kref exit.\n");
	return;
}


module_init(test_kref_init);
module_exit(test_kref_exit);
