#include "aufs.h"

static loff_t aufs_file_llseek(struct file *file, loff_t offset, int wherence) {

	printk(KERN_INFO "llseek aufs. \n");
	return -EISDIR;
}

static ssize_t aufs_read_file(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {
	printk(KERN_INFO "read file aufs. \n");

	// how many left
	return 0;
}

static int aufs_read_dir(struct file *filp, void *dirent, filldir_t filldir) {
	printk(KERN_INFO "read dir aufs. \n");

	return 0;
}

static int aufs_file_open(struct inode *node, struct file *file) {
	printk(KERN_INFO "file open aufs. \n");
	return 0;
}

static ssize_t aufs_write_file(struct file *file, const char __user *data, size_t size, loff_t *offset) {
	printk(KERN_INFO "file write aufs. \n");

	// consumed how many bits
	return size;
}


const struct file_operations aufs_file_operations = {
	.open	= aufs_file_open,
//	.llseek = aufs_file_llseek,
	.read   = aufs_read_file,
//	.readdir =  aufs_read_dir,
	.write = aufs_write_file,
	
};

