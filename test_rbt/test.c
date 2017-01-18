#include <linux/idr.h>
#include <linux/string.h>
#include <linux/rbtree.h>
#include "../common.h"

MODULE_LICENSE("GPL");

struct student {
	int id;
	struct rb_node node;
};



static int __init test_init(void) {

#define N  10
	// To insert node


	// to print


	// to remove the tree


}


static void __exit test_exit() {

}


module_init(test_init);
module_exit(test_exit);
