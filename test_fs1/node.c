#include "aufs.h"

static struct dentry *aufs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags) {
	return NULL;
}

static const struct inode_operations aufs_dir_inode_operations = {
	.lookup = aufs_lookup,
};


