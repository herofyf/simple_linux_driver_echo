#include "aufs.h"

static void aufs_put_super(struct super_block *sb) {
	// to free extra information belong to this file systemo
	// such if you have extra structure in sb->s_fs_info 
	printk(KERN_INFO "aufs put super.\n");
}

static struct super_operations const aufs_super_ops = {
	.put_super = aufs_put_super,
};


int aufs_fill_sb(struct super_block *sb, void *data, int silent) {
	// for super block members
	struct inode *root = NULL;

	sb->s_magic = AUFS_MAGIC;
	sb->s_op = &aufs_super_ops;
	
	root = new_inode(sb);
	root->i_sb = sb;
	root->i_mtime = root->i_atime = root->i_ctime = CURRENT_TIME;

	// because it is file property	
	root->i_mode = S_IFDIR;
	root->i_op = &aufs_dir_inode_operations;
	root->i_fop = &aufs_file_operations;	

	sb->s_root = d_make_root(root);
	return 0;
	
}

struct dentry *aufs_mount(struct file_system_type *type, int flags,
	const char *dev, void *data) {
	struct dentry *const entry = mount_bdev(type, flags, dev, data,	aufs_fill_sb);
	if (IS_ERR(entry)) {
		printk(KERN_ERR "mount aufs bdev failed.\n");
	} else {
		printk(KERN_ERR "mount aufs bdev %s ok. \n", dev);
	}

	return entry; 
}

void kill_block_super(struct super_block *sb) {
	printk(KERN_INFO "kill aufs super block.\n");
}

static struct file_system_type aufs_type = {
		.owner = THIS_MODULE,
		.name = "aufs",
		.mount = aufs_mount,
		.kill_sb = kill_block_super,
		.fs_flags = FS_REQUIRES_DEV,
};

static int __init aufs_init(void) {
	int res = register_filesystem(&aufs_type);
	return res;
}

static void __exit aufs_fini(void) {
	int res = unregister_filesystem(&aufs_type);
	
}

module_init(aufs_init);
module_exit(aufs_fini);


MODULE_LICENSE("GPL");
