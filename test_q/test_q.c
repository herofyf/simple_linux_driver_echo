#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/slab.h>

struct my_data {
	int x;
	int y;
};

void testq() {
	// init queue
	struct kfifo fifo;
	int ret;
	ret = kfifo_alloc(&fifo, 512, GFP_KERNEL);
	if (ret != 0) {
		printk(KERN_ERR "kfifo alloc failed.");
		return;
	}

	// push item in queue
	struct my_data *data;
	int i = 0;
	for (; i < 10; i ++) {
		data = kmalloc(sizeof(*data), GFP_KERNEL);
		data->x = i;
		kfifo_in(&fifo, &data, sizeof(data));
	}

	int size = kfifo_size(&fifo);
	int ava = kfifo_avail(&fifo);
	int len = kfifo_len(&fifo);
	int empty = kfifo_is_empty(&fifo);
	int full = kfifo_is_full(&fifo);
	printk(KERN_INFO "fifo used %d, and avaiable %d, len=%d, empty=%d, full is %d.\n",
			size, ava, len, empty, full);

	i = 0;	
	int offset = 0;
	while (true){
		offset = (i ++) * sizeof(data);
		if (offset >= len) {
			break;
		}

		__kfifo_out_peek_r(&fifo, &data, sizeof(data), offset);
		printk(KERN_INFO "peek item %d=%d\n",i, data->x);
	}
	

	// print all of items in queue
	i = 0;
	while (kfifo_is_empty(&fifo) == false) {
		ret = kfifo_out(&fifo, &data, sizeof(data));
		printk(KERN_INFO "item %d=%d\n",i, data->x);
		i += 1;
		kfree(data);
	}

	// destory queue
	kfifo_free(&fifo);

}

static int __init testq_init(void) {

	testq();

	return 0;
}

static void __exit testq_exit(void) {

	return;
}

module_init(testq_init);
module_exit(testq_exit);
