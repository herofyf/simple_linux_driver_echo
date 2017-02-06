#ifndef __AUFS_H__
#define __AUFS_H__

#include <linux/fs.h>
#include "common.h"

static const unsigned long AUFS_MAGIC = 0x13131313;

static const struct file_operations aufs_file_operations;
static const struct inode_operations aufs_dir_inode_operations;


#endif

