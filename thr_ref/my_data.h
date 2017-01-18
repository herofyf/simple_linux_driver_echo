#ifndef MY_DATA_INCLUDE
#define MY_DATA_INCLUDE

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/slab.h>

struct my_items {
	// for list
	struct list_head _link;
	int _val;
};

struct my_data {
	// 
	struct kref  _ref_count;

	//struct my_items *_items;
	struct list_head _list;

	struct mutex _mutex_sync;
};

extern struct my_data _g_data;

void timing_print(void *data);
#endif
