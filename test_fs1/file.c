#include "aufs.h"

loff_t aufs_file_llseek(struct file *file, loff_t offset, int wherence) {

	return -EISDIR;
}

ssize_t aufs_read_file(struct file *filp, char __user *buf, size_t siz, loff_t *ppos) {
	return -EISDIR;
}

static int aufs_read_dir(struct file *filp, void *dirent, filldir_t filldir) {
	return -EINVAL;
}

static const struct file_operations aufs_file_operations = {
	.llseek = aufs_file_llseek,
	.read   = aufs_read_file,
	.readdir =  aufs_read_dir,
};
