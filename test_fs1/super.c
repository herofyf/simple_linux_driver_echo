#include "aufs.h"

extern const struct file_operations aufs_file_operations;

static struct inode *aufs_new_node(struct super_block *sb, int mode) {
	struct inode *node = new_inode(sb);
	if (node == NULL) {
		return NULL;
	}	

	node->i_blocks = 0;	
	node->i_mode = mode;
	node->i_uid = node->i_gid = 0;
	node->i_atime = node->i_mtime = node->i_ctime = CURRENT_TIME;
	return node;
}

static void aufs_create_files(struct super_block *sb, struct dentry *root_de) {
	// at here we need to create a file and a sub directory which has a file included
	struct inode *node = NULL;
	struct dentry *dentry = NULL;

	struct qstr d_name; 
	
	d_name.name = "counter";
	d_name.len = 7;
	d_name.hash = full_name_hash(d_name.name, d_name.len);

	// how to add as its sub file	
	dentry = d_alloc(root_de, &d_name);
	if (dentry == NULL) {
		return;
	} 

	node = aufs_new_node(sb, S_IFREG | 0644);
	if (node == NULL) {
		goto error_de_out;
	}

	node->i_fop = &aufs_file_operations;
	
	d_add(dentry, node);

	printk(KERN_INFO "aufs create files done.\n");
	return;

error_de_out:
	if (dentry != NULL) {
		dput(dentry);
	} 
	return;
}

static void aufs_put_super(struct super_block *sb) {
	// to free extra information belong to this file systemo
	// such if you have extra structure in sb->s_fs_info 
	printk(KERN_INFO "aufs put super.\n");
}

static struct super_operations const aufs_super_ops = {
	.put_super = aufs_put_super,
	.statfs = simple_statfs,
	.drop_inode = generic_delete_inode,
};


static int aufs_fill_sb(struct super_block *sb, void *data, int silent) {
	// for super block members
	struct inode *root = NULL;

	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_magic = AUFS_MAGIC;
	sb->s_op = &aufs_super_ops;
	
	root = aufs_new_node(sb, S_IFDIR | 0755);
	if (root == NULL) {
		goto error_inode_out;
	}
	root->i_op = &simple_dir_inode_operations;
	root->i_fop = &simple_dir_operations;	

	struct dentry *root_dentry = NULL;
	root_dentry = d_make_root(root);
	if (root_dentry == NULL) {
		goto error_de_out;
	}
	sb->s_root = root_dentry;

	aufs_create_files(sb, root_dentry);
	return 0;

error_de_out:
	if (root_dentry != NULL) {
		dput(root_dentry);
	}

error_inode_out:
	if (root != NULL) {
		iput(root);
	}

	return -ENOMEM;

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

void kill_aufs_super(struct super_block *sb) {
	printk(KERN_INFO "kill aufs super block.\n");

	// kill all of nodes	
	kill_litter_super(sb);
}

static struct file_system_type aufs_type = {
		.owner = THIS_MODULE,
		.name = "aufs",
		.mount = aufs_mount,
		.kill_sb = kill_aufs_super,
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
