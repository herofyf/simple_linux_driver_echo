#include "my_data.h"


void timing_print(void *data)
{
	// 
	struct my_data *items = (struct my_data *)(data);
	if (items == NULL)
		return;
	
	struct list_head *p, *pnxt;
	struct my_items *item = NULL;
	list_for_each(p, &items->_list) {
		item = list_entry(p, struct my_items, _link);
		printk(KERN_INFO "The value is %d\n", item->_val);
	}


	// remove all of items
	struct list_head *head = &items->_list;
	while (list_empty(head) == false) {
		p = head->next;
		item = container_of(p, struct my_items, _link);
		printk(KERN_INFO "TO DEL ITEM %d\n", item->_val);
		list_del(p);
		kfree(item);
	}

}
