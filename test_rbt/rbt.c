#include <linux/rbtree.h>
#include <linux/rbtree_augmented.h>
#include <linux/string.h>
#include "common.h"

MODULE_LICENSE("GPL");


struct student {
	int id;

	struct rb_node node;	
};

#define ST_SIZE 10 


static bool insert_stud(struct rb_root *root, struct student *stud) {
	
	struct student *tmp_stud;
	struct rb_node *tmp_node;
	struct rb_node *parent;
	

	if (root->rb_node == NULL) {
		root->rb_node = &(stud->node);
		rb_set_parent_color(&(stud->node), NULL, RB_BLACK);
		return true;
	}

	tmp_node = root->rb_node;	
	while (tmp_node != NULL) {
		parent = tmp_node;

		tmp_stud =  container_of(tmp_node, struct student, node);
		if (tmp_stud->id > stud->id) {
			tmp_node = tmp_node->rb_left;
		} else if (tmp_stud->id < stud->id) {
			tmp_node =  tmp_node->rb_right;
		} else {
			tmp_stud->id = stud->id;
			return false;
		}
	
	}	

	printk(KERN_INFO "tmp = %d, cur=%d.\n",
			tmp_stud->id,
			stud->id);

	if (tmp_stud->id > stud->id) {
		parent->rb_left = &(stud->node);
	} else {	
		parent->rb_right = &(stud->node);
	}	
	
	rb_set_parent_color(&(stud->node), parent, RB_RED);
	rb_insert_color(&(stud->node), root);
//	rb_insert_color(root, root);

	/*
	tmp_node = root->rb_node;
	tmp_stud =  container_of(tmp_node, struct student, node);
	printk(KERN_INFO " root node = %pK, val = %d \n",
		tmp_node, tmp_stud->id);
	*/
	return true;
}

void print_midorder_students(struct rb_node *node) {
	if (node == NULL) {
		return;
	}

	struct student *stud =  container_of(node, struct student, node);
	if (stud == NULL) {
		return;
	}
	
	printk(KERN_INFO "student = %d \n", stud->id);	

	print_midorder_students(stud->node.rb_left);
	print_midorder_students(stud->node.rb_right);
	
	
}

void print_students(struct rb_root *root) {

	if (root == NULL || root->rb_node) {
		return;
	}

	struct rb_node *node = root->rb_node;
	print_midorder_students(node);
}


void remove_all_students(struct rb_root *root) {

	struct rb_node *tmp_node = root->rb_node;

	while (tmp_node != NULL) {
		rb_erase(tmp_node, root);
		struct student *stud =  container_of(tmp_node, struct student, node);
		if (stud) {
			printk(KERN_INFO "remove student = %d \n", stud->id);	
			free(stud);
		}
	
		tmp_node = root->rb_node;
	}	
}

// insert 10 items and print and remove
static int __init test_init(void)
{
	printk(KERN_INFO "rbt init.\n");

	bool ret = false;
	struct rb_root root;
	root.rb_node = NULL;

	struct student *studs[ST_SIZE];
	int i = 0;
	for (i = 0; i < ST_SIZE; i ++) {
		studs[i] = malloc(sizeof(struct student), GFP_KERNEL);
		studs[i]->id = i;
		studs[i]->node.rb_left = NULL;
		studs[i]->node.rb_right = NULL;
	} 

	for (i = 0; i < ST_SIZE; i ++) {
		ret = insert_stud(&root, studs[i]); 
		if (ret == false) {
			free(studs[i]);
		}
	}

	print_students(&root);	

	remove_all_students(&root);
	return 1;
}

static void __exit test_exit()
{
	printk(KERN_INFO "rbt driver exit\n");
} 

module_init(test_init);
module_exit(test_exit);

