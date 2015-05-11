/*
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
                         
  
                                                              
                                                    
  
                                                                  
  
                                                                            
                                    
                                         
                                                              
                                   
  
                                                                         
                                                                             
                                                    
                                
  
                                                                            
                                                                   
                                
  
                    
                                                                         
                                                            
  
 */

/*                                                   */
/*                 */

#define pr_fmt(fmt)	"pft [%s]: " fmt, __func__

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/printk.h>
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/bitops.h>
#include <linux/fdtable.h>
#include <linux/selinux.h>

#include <linux/pft.h>
#include <linux/msm_pft.h>

#include "objsec.h"

/*                                                  */
#define PFT_TAG_MAGIC		((u32)(0xABC00000))

/*                          */
#define PFT_TAG_ENCRYPTED	BIT(16)

#define PFT_TAG_MAGIC_MASK	0xFFF00000
#define PFT_TAG_FLAGS_MASK	0x000F0000
#define PFT_TAG_KEY_MASK	0x0000FFFF

/*                                  */
#define PFT_DEFAULT_KEY_INDEX	1

/*                                               */
#define PFT_NO_KEY		0

/*                             */
#define XATTR_NAME_PFE "security.pfe"

/*                                   */
#define PFT_REQUESTED_MAJOR	213

/*                 */
#define DEVICE_NAME	"pft"

/*                                 */
#define PFT_MAX_APPS	1000

/*                      */
#define PFT_MAX_COMMAND_SIZE (PAGE_SIZE)

/*                    */
#define current_pid() ((u32)(current->pid))

static const char *pft_state_name[PFT_STATE_MAX_INDEX] = {
	"deactivated",
	"deactivating",
	"key_removed",
	"removing_key",
	"key_loaded",
};

/* 
                                             
                                   
                    
                         
  
                                                          
 */
struct pft_file_info {
	struct file *file;
	pid_t pid;
	struct list_head list;
};

/* 
                                              
  
                                  
                               
                                     
                               
                            
                                               
                                                  
                                                 
                                             
                                   
  
                                                               
                                
                                                           
                                                         
 */
struct pft_device {
	struct cdev cdev;
	dev_t device_no;
	struct class *driver_class;
	int open_count;
	int major;
	enum pft_state state;
	struct pft_command_response response;
	u32 pfm_pid;
	struct file *inplace_file;
	u32 *uid_table;
	u32 uid_count;
	struct list_head open_file_list;
	struct mutex lock;
};

/*                     */
static struct pft_device *pft_dev;

static struct inode *pft_bio_get_inode(struct bio *bio);

/* 
                                                    
  
                                       
 */
static bool pft_is_ready(void)
{
	return  (pft_dev != NULL);
}

/* 
                                                           
                      
  
                               
  
                                        
 */
static char *file_to_filename(struct file *filp)
{
	struct dentry *dentry = NULL;
	char *filename = NULL;

	if (!filp || !filp->f_dentry)
		return "unknown";

	dentry = filp->f_dentry;
	filename = dentry->d_iname;

	return filename;
}

/* 
                                                             
                        
  
                               
  
                                        
 */
static char *inode_to_filename(struct inode *inode)
{
	struct dentry *dentry = NULL;
	char *filename = NULL;

	if (list_empty(&inode->i_dentry))
		return "unknown";

	dentry = list_first_entry(&inode->i_dentry, struct dentry, d_alias);

	filename = dentry->d_iname;

	return filename;
}

/* 
                                                
  
                                                     
 */
static inline void pft_set_response(u32 error_code)
{
	pft_dev->response.error_code = error_code;
}

/* 
                                                               
         
                      
  
                                                               
 */
static int pft_add_file(struct file *filp)
{
	struct pft_file_info *node = NULL;

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (!node) {
		pr_err("malloc failure\n");
		return -ENOMEM;
	}

	node->file = filp;
	INIT_LIST_HEAD(&node->list);

	mutex_lock(&pft_dev->lock);
	list_add(&node->list, &pft_dev->open_file_list);
	pr_debug("adding file %s to open list.\n", file_to_filename(filp));
	mutex_unlock(&pft_dev->lock);

	return 0;
}

/* 
                                                            
                        
                         
  
                                                   
 */
static int pft_remove_file(struct file *filp)
{
	int ret = -ENOENT;
	struct pft_file_info *tmp = NULL;
	struct list_head *pos = NULL;
	struct list_head *next = NULL;
	bool found = false;

	mutex_lock(&pft_dev->lock);
	list_for_each_safe(pos, next, &pft_dev->open_file_list) {
		tmp = list_entry(pos, struct pft_file_info, list);
		if (filp == tmp->file) {
			found = true;
			break;
		}
	}

	if (found) {
		pr_debug("remove file %s. from open list.\n ",
			 file_to_filename(filp));
		list_del(&tmp->list);
		kfree(tmp);
		ret = 0;
	}
	mutex_unlock(&pft_dev->lock);

	return ret;
}

/* 
                                                                
                 
  
                                                 
 */
static bool pft_is_current_process_registered(void)
{
	int is_registered = false;
	int i;
	u32 uid = current_uid();

	mutex_lock(&pft_dev->lock);
	for (i = 0; i < pft_dev->uid_count; i++) {
		if (pft_dev->uid_table[i] == uid) {
			pr_debug("current UID [%u] is registerd.\n", uid);
			is_registered = true;
			break;
		}
	}
	mutex_unlock(&pft_dev->lock);

	return is_registered;
}

/* 
                                                              
                       
                                    
  
                                           
 */
static bool pft_is_xattr_supported(struct inode *inode)
{
	if (inode == NULL) {
		pr_err("invalid argument inode passed as NULL");
		return false;
	}

	if (inode->i_security == NULL) {
		pr_debug("i_security is NULL, not ready yet\n");
		return false;
	}

	if (inode->i_op == NULL) {
		pr_debug("i_op is NULL\n");
		return false;
	}

	if (inode->i_op->getxattr == NULL) {
		pr_debug_once("getxattr() not supported , filename=%s\n",
			      inode_to_filename(inode));
		return false;
	}

	if (inode->i_op->setxattr == NULL) {
		pr_debug("setxattr() not supported\n");
		return false;
	}

	return true;
}

/* 
                                          
                                    
  
              
 */
static u32 pft_get_inode_tag(struct inode *inode)
{
	struct inode_security_struct *isec = inode->i_security;

	if (isec == NULL)
		return 0;

	return isec->tag;
}

/* 
                                                
                                    
  
                    
 */
static inline u32 pft_get_inode_key_index(struct inode *inode)
{
	return pft_get_inode_tag(inode) & PFT_TAG_KEY_MASK;
}

/* 
                                        
                                    
  
                                                                 
                                                              
                                                            
  
                                    
 */
static inline bool pft_is_tag_valid(struct inode *inode)
{
	struct inode_security_struct *isec = inode->i_security;

	if (isec == NULL)
		return false;

	return ((isec->tag & PFT_TAG_MAGIC_MASK) == PFT_TAG_MAGIC) ?
		true : false;
}

/* 
                                                          
  
                                               
  
                                         
 */
static inline bool pft_is_file_encrypted(u32 tag)
{
	return (tag & PFT_TAG_ENCRYPTED) ? true : false;
}

/* 
                                                   
                 
                                    
  
                                                        
                                
 */
static inline void pft_tag_inode_non_encrypted(struct inode *inode)
{
	struct inode_security_struct *isec = inode->i_security;

	isec->tag = (u32)(PFT_TAG_MAGIC);
}

/* 
                                                          
                                    
  
                                                           
 */
static void pft_tag_inode_encrypted(struct inode *inode, u32 key_index)
{
	struct inode_security_struct *isec = inode->i_security;

	isec->tag = key_index | PFT_TAG_ENCRYPTED | PFT_TAG_MAGIC;
}

/* 
                                        
                                   
                            
  
                                                      
                                              
                                                            
  
                                                    
 */
static int pft_get_file_tag(struct dentry *dentry, u32 *tag_ptr)
{
	ssize_t size = 0;
	struct inode *inode;
	const char *xattr_name = XATTR_NAME_PFE;
	u32 key;

	if (!dentry || !dentry->d_inode || !tag_ptr) {
		pr_err("invalid param");
		return -EINVAL;
	}

	inode = dentry->d_inode;
	if (pft_is_tag_valid(inode)) {
		*tag_ptr = pft_get_inode_tag(inode);
		return 0;
	}

	/*
                                                                   
              
  */
	size = inode->i_op->getxattr(dentry, xattr_name, &key, sizeof(key));

	if (size == -ENODATA || size == -EOPNOTSUPP) {
		pft_tag_inode_non_encrypted(inode);
		*tag_ptr = pft_get_inode_tag(inode);
	} else if (size > 0) {
		pr_debug("First time file %s opened, found xattr = %u.\n",
		       inode_to_filename(inode), key);
		pft_tag_inode_encrypted(inode, key);
		*tag_ptr = pft_get_inode_tag(inode);
	} else {
		pr_err("getxattr() failure, ret=%d.\n", size);
		return -EINVAL;
	}

	return 0;
}

/* 
                                                      
                        
                                    
  
                                                
                                                
  
                                                    
 */
static int pft_tag_file(struct dentry *dentry, u32 key_index)
{
	int size = 0;
	const char *xattr_name = XATTR_NAME_PFE;

	if (!dentry || !dentry->d_inode) {
		pr_err("invalid NULL param");
		return -EINVAL;
	}

	if (!pft_is_xattr_supported(dentry->d_inode)) {
		pr_err("set xattr for file %s is not support.\n",
		       dentry->d_iname);
		return -EINVAL;
	}

	size = dentry->d_inode->i_op->setxattr(dentry, xattr_name, &key_index,
					       sizeof(key_index), 0);
	if (size < 0) {
		pr_err("failed to set xattr for file %s, ret =%d.\n",
		       dentry->d_iname, size);
		return -EFAULT;
	}

	pft_tag_inode_encrypted(dentry->d_inode, key_index);
	pr_debug("file %s tagged encrypted\n", dentry->d_iname);

	return 0;
}

/* 
                                                           
                                   
  
                                                               
                                       
  
                                
 */
static inline u32 pft_get_app_key_index(u32 uid)
{
	return PFT_DEFAULT_KEY_INDEX;
}

/* 
                                                   
                         
  
                                                          
 */
static bool pft_is_encrypted_file(struct dentry *dentry)
{
	int rc;
	u32 tag;

	if (!pft_is_ready())
		return false;

	if (!pft_is_xattr_supported(dentry->d_inode))
		return false;

	rc = pft_get_file_tag(dentry, &tag);
	if (rc < 0)
		return false;

	return pft_is_file_encrypted(tag);
}

/* 
                                                         
                       
                                  
  
                                                      
             
 */
static bool pft_is_inplace_inode(struct inode *inode)
{
	if (!pft_dev->inplace_file || !pft_dev->inplace_file->f_path.dentry)
		return false;

	return (pft_dev->inplace_file->f_path.dentry->d_inode == inode);
}

/* 
                                                        
              
                        
  
                                                                
            
  
                                                      
             
 */
static inline bool pft_is_inplace_file(struct file *filp)
{
	if (!filp || !filp->f_path.dentry || !filp->f_path.dentry->d_inode)
		return false;

	return pft_is_inplace_inode(filp->f_path.dentry->d_inode);
}

/* 
                                                                
                                  
                                                       
                                              
                                            
  
                                                               
                                                               
                             
                                                         
                            
                                                               
                                                     
         
  
                                                    
 */
int pft_get_key_index(struct bio *bio, u32 *key_index,
		      bool *is_encrypted, bool *is_inplace)
{
	u32 tag = 0;
	struct inode *inode = NULL;

	if (!pft_is_ready())
		return -ENODEV;

	if (!selinux_is_enabled())
		return -ENODEV;

	if (!bio)
		return -EPERM;

	if (!is_encrypted) {
		pr_err("is_encrypted is NULL\n");
		return -EPERM;
	}
	if (!is_inplace) {
		pr_err("is_inplace is NULL\n");
		return -EPERM;
	}
	if (!key_index) {
		pr_err("key_index is NULL\n");
		return -EPERM;
	}

	inode = pft_bio_get_inode(bio);
	if (!inode)
		return -EINVAL;

	if (!pft_is_tag_valid(inode)) {
		pr_debug("file %s, Tag not valid\n", inode_to_filename(inode));
		return -EINVAL;
	}

	if (!pft_is_xattr_supported(inode)) {
		*is_encrypted = false;
		*is_inplace = false;
		*key_index = 0;
		return 0;
	}

	tag = pft_get_inode_tag(inode);

	*is_encrypted = pft_is_file_encrypted(tag);
	*key_index = pft_get_inode_key_index(inode);
	*is_inplace = pft_is_inplace_inode(inode);

	if (*is_encrypted)
		pr_debug("file %s is encrypted\n", inode_to_filename(inode));

	return 0;
}
EXPORT_SYMBOL(pft_get_key_index);

/* 
                                                  
                                  
  
                                              
  
                                                                        
 */
static struct inode *pft_bio_get_inode(struct bio *bio)
{
	if (!bio)
		return NULL;
	if (!bio->bi_io_vec)
		return NULL;
	if (!bio->bi_io_vec->bv_page)
		return NULL;

	if (PageAnon(bio->bi_io_vec->bv_page)) {
		struct inode *inode;

		/*                                               */
		inode = dio_bio_get_inode(bio);
		pr_debug("inode on direct-io, inode = 0x%x.\n", (int) inode);

		return inode;
	}

	if (!bio->bi_io_vec->bv_page->mapping)
		return NULL;

	if (!bio->bi_io_vec->bv_page->mapping->host)
		return NULL;

	return bio->bi_io_vec->bv_page->mapping->host;
}

/* 
                                                        
                                         
                                          
  
                                                           
                                                
                                                     
  
                                                       
             
 */
bool pft_allow_merge_bio(struct bio *bio1, struct bio *bio2)
{
	u32 key_index1 = 0, key_index2 = 0;
	bool is_encrypted1 = false, is_encrypted2 = false;
	bool allow = false;
	bool is_inplace = false; /*      */
	int ret;

	if (!pft_is_ready())
		return true;

	ret = pft_get_key_index(bio1, &key_index1,
				&is_encrypted1, &is_inplace);
	if (ret)
		is_encrypted1 = false;

	ret = pft_get_key_index(bio2, &key_index2,
				&is_encrypted2, &is_inplace);
	if (ret)
		is_encrypted2 = false;

	allow = ((is_encrypted1 == is_encrypted2) &&
		 (key_index1 == key_index2));

	return allow;
}
EXPORT_SYMBOL(pft_allow_merge_bio);

/* 
                                               
                                
                               
               
  
                                                   
                                               
                                                           
                
             
                                                                
  
                                                    
 */
int pft_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	if (!dir || !dentry)
		return 0;

	if (!pft_is_ready())
		return 0;

	switch (pft_dev->state) {
	case PFT_STATE_DEACTIVATED:
	case PFT_STATE_KEY_LOADED:
		break;
	case PFT_STATE_KEY_REMOVED:
	case PFT_STATE_DEACTIVATING:
	case PFT_STATE_REMOVING_KEY:
		/*                                                     */
		if (pft_is_current_process_registered()) {
			pr_debug("key removed, registered uid %u is denied from creating new file %s\n",
				current_uid(), dentry->d_iname);
			return -EACCES;
		}
		break;
	default:
		BUG(); /*                                     */
		break;
	}

	return 0;

}
EXPORT_SYMBOL(pft_inode_create);

/* 
                                                    
                                
                               
               
  
                                                   
                                               
                                                         
                           
             
                                                                          
  
                                                    
 */
int pft_inode_post_create(struct inode *dir, struct dentry *dentry,
			  umode_t mode)
{
	int ret;

	if (!dir || !dentry)
		return 0;

	if (!pft_is_ready())
		return 0;

	switch (pft_dev->state) {
	case PFT_STATE_DEACTIVATED:
	case PFT_STATE_KEY_REMOVED:
	case PFT_STATE_DEACTIVATING:
	case PFT_STATE_REMOVING_KEY:
		break;
	case PFT_STATE_KEY_LOADED:
		/*                                                */
		if (pft_is_current_process_registered()) {
			u32 key_index = pft_get_app_key_index(current_uid());
			ret = pft_tag_file(dentry, key_index);
			if (ret == 0)
				pr_debug("key loaded, pid [%u] uid [%d] is creating file %s\n",
					 current_pid(), current_uid(),
					 dentry->d_iname);
			else {
				pr_err("Failed to tag file %s by pid %d\n",
					dentry->d_iname, current_pid());
				return -EFAULT;
			}
		}
		break;
	default:
		BUG(); /*                                     */
		break;
	}

	return 0;
}
EXPORT_SYMBOL(pft_inode_post_create);

/* 
                                                  
                                
                               
               
        
  
                                                       
                          
             
                                                                                
  
                                                               
 */
int pft_inode_mknod(struct inode *dir, struct dentry *dentry,
		    umode_t mode, dev_t dev)
{
	int rc;

	/*                                                */
	rc = pft_inode_create(dir, dentry, mode);

	return rc;
}
EXPORT_SYMBOL(pft_inode_mknod);

/* 
                                         
                          
                       
             
              
  
                                           
  
                                                            
 */
int pft_inode_rename(struct inode *inode, struct dentry *dentry,
		     struct inode *new_inode, struct dentry *new_dentry)
{
	if (!inode || !dentry || !new_inode || !new_dentry || !dentry->d_inode)
		return 0;

	if (!pft_is_ready())
		return 0;

	/*                                    */
	if (!pft_is_encrypted_file(dentry))
		return 0;

	pr_debug("attempt to rename encrypted file [%s]\n", dentry->d_iname);

	if (pft_is_inplace_inode(dentry->d_inode)) {
		pr_err("access in-place-encryption file %s by uid [%d] pid [%d] is blocked.\n",
		       inode_to_filename(inode), current_uid(), current_pid());
		return -EACCES;
	}

	if (!pft_is_current_process_registered()) {
		pr_err("unregistered app (uid %u pid %u) is trying to access encrypted file %s\n",
		       current_uid(), current_pid(), dentry->d_iname);
		return -EACCES;
	} else
		pr_debug("rename file %s\n", dentry->d_iname);

	return 0;
}
EXPORT_SYMBOL(pft_inode_rename);

/* 
                                               
                      
                             
  
                                                  
                                        
                                                     
                                                    
             
                                                                
  
                                                    
 */
int pft_file_open(struct file *filp, const struct cred *cred)
{
	int ret;

	if (!filp || !filp->f_path.dentry)
		return 0;

	if (!pft_is_ready())
		return 0;

	if (filp->f_flags & O_DIRECT)
		pr_debug("file %s using O_DIRECT.\n", file_to_filename(filp));

	/*                                    */
	if (!pft_is_encrypted_file(filp->f_dentry))
		return 0;

	/*
                                                       
                                      
  */
	if (pft_is_inplace_file(filp) && current_pid() != pft_dev->pfm_pid) {
		pr_err("Access in-place-encryption file %s by uid %d pid %d is blocked.\n",
			 file_to_filename(filp), current_uid(), current_pid());
		return -EACCES;
	}

	switch (pft_dev->state) {
	case PFT_STATE_DEACTIVATED:
	case PFT_STATE_KEY_REMOVED:
	case PFT_STATE_DEACTIVATING:
	case PFT_STATE_REMOVING_KEY:
		/*                                                          */
		pr_debug("key not loaded. uid (%u) can not access file %s\n",
			 current_uid(), file_to_filename(filp));
		return -EACCES;
	case PFT_STATE_KEY_LOADED:
		 /*                                                  */
		if (!pft_is_current_process_registered()) {
			pr_err("unregistered app (uid %u pid %u) is trying to access encrypted file %s\n",
			       current_uid(), current_pid(),
			       file_to_filename(filp));
			return -EACCES;
		}

		ret = pft_add_file(filp);
		if (ret) {
			pr_err("failed to add file %s to the list.\n",
			       file_to_filename(filp));
			return -EFAULT;
		}
		break;
	default:
		BUG(); /*                                     */
		break;
	}

	return 0;
}
EXPORT_SYMBOL(pft_file_open);

/* 
                                                        
                      
               
  
                                                      
                                               
                                                       
                          
             
                                                                    
  
                                                    
 */
int pft_file_permission(struct file *filp, int mask)
{
	if (!filp)
		return 0;

	if (!pft_is_ready())
		return 0;

	/*                                    */
	if (!pft_is_encrypted_file(filp->f_dentry))
		return 0;

	/*
                                                       
                                      
  */
	if (pft_is_inplace_file(filp)) {
		if (current_pid() == pft_dev->pfm_pid) {
			/*                               */
			pr_debug("r/w [mask 0x%x] in-place-encryption file %s by PFM (UID %d, PID %d).\n",
				 mask, file_to_filename(filp),
				 current_uid(), current_pid());
			return 0;
		} else {
			pr_err("Access in-place-encryption file %s by App (UID %d, PID %d) is blocked.\n",
			       file_to_filename(filp),
			       current_uid(), current_pid());
			return -EACCES;
		}
	}

	switch (pft_dev->state) {
	case PFT_STATE_DEACTIVATED:
	case PFT_STATE_KEY_REMOVED:
	case PFT_STATE_DEACTIVATING:
	case PFT_STATE_REMOVING_KEY:
		/*                                                          */
		pr_debug("key not loaded. uid (%u) can not access file %s\n",
			 current_uid(), file_to_filename(filp));
		return -EACCES;
	case PFT_STATE_KEY_LOADED:
		 /*                                                  */
		if (!pft_is_current_process_registered()) {
			pr_err("unregistered app (uid %u pid %u) is trying to access encrypted file %s\n",
			       current_uid(), current_pid(),
			       file_to_filename(filp));
			return -EACCES;
		}
		break;
	default:
		BUG(); /*                                     */
		break;
	}

	return 0;
}
EXPORT_SYMBOL(pft_file_permission);

/* 
                                   
                      
  
                                                                
                                                      
                    
 */
static void pft_sync_file(struct file *filp)
{
	int ret;

	ret = vfs_fsync(filp, false);

	if (ret)
		pr_debug("failed to sync file %s, ret = %d.\n",
			 file_to_filename(filp), ret);
	else
		pr_debug("Sync file %s ok.\n",  file_to_filename(filp));

}

/* 
                                            
                      
  
                                                  
                                               
  
                                                               
 */
int pft_file_close(struct file *filp)
{
	if (!filp)
		return 0;

	if (!pft_is_ready())
		return 0;

	/*                                    */
	if (!pft_is_encrypted_file(filp->f_dentry))
		return 0;

	if (pft_is_inplace_file(filp)) {
		pr_debug("pid [%u] uid [%u] is closing in-place-encryption file %s\n",
			 current_pid(), current_uid(), file_to_filename(filp));
		pft_dev->inplace_file = NULL;
	}

	pft_sync_file(filp);
	pft_remove_file(filp);

	return 0;
}
EXPORT_SYMBOL(pft_file_close);

/* 
                                         
                                
                               
  
                                                    
  
                                                               
 */
int pft_inode_unlink(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = NULL;

	if (!dir || !dentry || !dentry->d_inode)
		return 0;

	if (!pft_is_ready())
		return 0;

	inode = dentry->d_inode;

	/*                                    */
	if (!pft_is_encrypted_file(dentry))
		return 0;

	if (pft_is_inplace_inode(inode)) {
		pr_err("block delete in-place-encryption file %s by uid [%d] pid [%d], while encryption in progress.\n",
		       inode_to_filename(inode), current_uid(), current_pid());
		return -EBUSY;
	}

	if (!pft_is_current_process_registered()) {
		pr_err("unregistered app (uid %u pid %u) is trying to access encrypted file %s\n",
		       current_uid(), current_pid(), inode_to_filename(inode));
		return -EACCES;
	} else
		pr_debug("delete file %s\n", inode_to_filename(inode));

	return 0;
}
EXPORT_SYMBOL(pft_inode_unlink);

/* 
                                                     
                               
                     
  
                                                    
                                                                
                                                               
         
  
                                                               
 */
int pft_inode_set_xattr(struct dentry *dentry, const char *name)
{
	struct inode *inode = NULL;

	if (!dentry || !dentry->d_inode)
		return 0;

	inode = dentry->d_inode;

	if (strcmp(name, XATTR_NAME_PFE) != 0) {
		pr_debug("xattr name=%s file %s\n", name,
		       inode_to_filename(inode));
		return 0; /*                           */
	}

	pr_err("Attemp to set/remove PFE xattr for file %s\n",
	       inode_to_filename(inode));

	/*                                                    */
	return -EACCES;
}
EXPORT_SYMBOL(pft_inode_set_xattr);

/* 
                                                              
                  
  
                                                     
                
 */
static void pft_close_opened_enc_files(void)
{
	struct pft_file_info *tmp = NULL;
	struct list_head *pos = NULL;
	struct list_head *next = NULL;

	list_for_each_safe(pos, next, &pft_dev->open_file_list) {
		struct file *filp;
		tmp = list_entry(pos, struct pft_file_info, list);
		filp = tmp->file;
		pr_debug("closing file %s.\n", file_to_filename(filp));
		/*                                                */
		filp_close(filp, NULL);
	}
}

/* 
                                               
                            
                                 
  
                                                            
  
                                                               
 */
static int pft_set_state(struct pft_command *command, int size)
{
	u32 state = command->set_state.state;
	int expected_size = sizeof(command->opcode) +
		sizeof(command->set_state);

	if (size != expected_size) {
		pr_err("Invalid buffer size\n");
		pft_set_response(PFT_CMD_RESP_INVALID_CMD_PARAMS);
		return -EINVAL;
	}

	if (state >= PFT_STATE_MAX_INDEX) {
		pr_err("Invalid state %d\n", command->set_state.state);
		pft_set_response(PFT_CMD_RESP_INVALID_STATE);
		return 0;
	}

	pr_debug("Set State %d [%s].\n", state, pft_state_name[state]);

	switch (command->set_state.state) {
	case PFT_STATE_DEACTIVATING:
	case PFT_STATE_REMOVING_KEY:
		pft_close_opened_enc_files();
		/*              */
	case PFT_STATE_DEACTIVATED:
	case PFT_STATE_KEY_LOADED:
	case PFT_STATE_KEY_REMOVED:
		pft_dev->state = command->set_state.state;
		pft_set_response(PFT_CMD_RESP_SUCCESS);
		break;
	default:
		pr_err("Invalid state %d\n", command->set_state.state);
		pft_set_response(PFT_CMD_RESP_INVALID_STATE);
		break;
	}

	return 0;
}

/* 
                                                            
                    
                                 
  
                                                    
 */
static struct file *pft_get_process_open_file(int index)
{
	struct fdtable *files_table;

	files_table = files_fdtable(current->files);
	if (files_table == NULL)
		return NULL;

	if (index >= files_table->max_fds)
		return NULL;
	else
		return files_table->fd[index];
}

/* 
                                                             
            
                            
                                 
  
                                                            
  
                                                           
 */
static int pft_set_inplace_file(struct pft_command *command, int size)
{
	int expected_size;
	u32 fd;
	int rc;
	struct file *filp = NULL;
	struct inode *inode = NULL;
	int writecount;

	expected_size = sizeof(command->opcode) +
		sizeof(command->preform_in_place_file_enc.file_descriptor);

	if (size != expected_size) {
		pr_err("invalid command size %d expected %d.\n",
		       size, expected_size);
		pft_set_response(PFT_CMD_RESP_INVALID_CMD_PARAMS);
		return -EINVAL;
	}

	if (pft_dev->state != (u32) PFT_STATE_KEY_LOADED) {
		pr_err("Key not loaded, state [%d], In-place-encryption is not allowed.\n",
		       pft_dev->state);
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
		return 0;
	}

	/*                                                   */
	if (pft_dev->inplace_file != NULL) {
		pr_err("file %s in-place-encryption in progress.\n",
		       file_to_filename(pft_dev->inplace_file));
		/*                            */
		pft_set_response(PFT_CMD_RESP_INPLACE_FILE_IS_OPEN);
		return 0;
	}

	fd = command->preform_in_place_file_enc.file_descriptor;
	filp = pft_get_process_open_file(fd);

	if (filp == NULL) {
		pr_err("failed to find file by fd %d.\n", fd);
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
		return 0;
	}

	/*                                                       */
	if (!filp->f_path.dentry || !filp->f_path.dentry->d_inode) {
		pr_err("failed to get inode of inplace-file.\n");
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
		return 0;
	}

	inode = filp->f_path.dentry->d_inode;
	writecount = atomic_read(&inode->i_writecount);
	if (writecount > 1) {
		pr_err("file %s is opened %d times for write.\n",
		       file_to_filename(filp), writecount);
		pft_set_response(PFT_CMD_RESP_INPLACE_FILE_IS_OPEN);
		return 0;
	}

	/*
                                             
                                          
                                                
                                            
  */
	if (pft_is_encrypted_file(filp->f_dentry)) {
		pr_err("file %s is already encrypted.\n",
		       file_to_filename(filp));
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
		return 0;
	}


	/*                                             */
	pft_dev->inplace_file = filp;

	/*
                                                            
                                                        
                               
  */
	pft_sync_file(filp);

	rc = pft_tag_file(pft_dev->inplace_file->f_dentry,
			  pft_get_app_key_index(current_uid()));

	if (!rc) {
		pr_debug("tagged file %s to be encrypted.\n",
			 file_to_filename(pft_dev->inplace_file));
		pft_set_response(PFT_CMD_RESP_SUCCESS);
	} else {
		pr_err("failed to tag file %s for encryption.\n",
			file_to_filename(pft_dev->inplace_file));
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
	}

	return 0;
}

/* 
                                                            
        
                            
                                 
  
                                                            
  
                                                               
 */
static int pft_update_reg_apps(struct pft_command *command, int size)
{
	int i;
	int expected_size;
	void *buf;
	int buf_size;
	u32 items_count = command->update_app_list.items_count;

	if (items_count > PFT_MAX_APPS) {
		pr_err("Number of apps [%d] > max apps [%d]\n",
		       items_count , PFT_MAX_APPS);
		pft_set_response(PFT_CMD_RESP_INVALID_CMD_PARAMS);
		return -EINVAL;
	}

	expected_size =
		sizeof(command->opcode) +
		sizeof(command->update_app_list.items_count) +
		(command->update_app_list.items_count * sizeof(u32));

	if (size != expected_size) {
		pr_err("invalid command size %d expected %d.\n",
		       size, expected_size);
		pft_set_response(PFT_CMD_RESP_INVALID_CMD_PARAMS);
		return -EINVAL;
	}

	mutex_lock(&pft_dev->lock);

	/*                */
	kfree(pft_dev->uid_table);
	pft_dev->uid_table = NULL;
	pft_dev->uid_count = 0;

	if (items_count == 0) {
		pr_info("empty app list - clear list.\n");
		mutex_unlock(&pft_dev->lock);
		return 0;
	}

	buf_size = command->update_app_list.items_count * sizeof(u32);
	buf = kzalloc(buf_size, GFP_KERNEL);

	if (!buf) {
		pr_err("malloc failure\n");
		pft_set_response(PFT_CMD_RESP_GENERAL_ERROR);
		mutex_unlock(&pft_dev->lock);
		return 0;
	}

	pft_dev->uid_table = buf;
	pft_dev->uid_count = command->update_app_list.items_count;
	pr_debug("uid_count = %d\n", pft_dev->uid_count);
	for (i = 0; i < pft_dev->uid_count; i++)
		pft_dev->uid_table[i] = command->update_app_list.table[i];
	pft_set_response(PFT_CMD_RESP_SUCCESS);
	mutex_unlock(&pft_dev->lock);

	return 0;
}

/* 
                                                         
                        
                                  
  
                                                               
 */
static int pft_handle_command(void *buf, int buf_size)
{
	int ret = 0;
	struct pft_command *command = NULL;

	/*                                               */
	if (buf_size < sizeof(command->opcode)) {
		pr_err("Invalid argument used buffer size\n");
		return -EINVAL;
	}

	command = (struct pft_command *)buf;

	pft_dev->response.command_id = command->opcode;

	switch (command->opcode) {
	case PFT_CMD_OPCODE_SET_STATE:
		ret = pft_set_state(command, buf_size);
		break;
	case PFT_CMD_OPCODE_UPDATE_REG_APP_UID:
		ret = pft_update_reg_apps(command, buf_size);
		break;
	case PFT_CMD_OPCODE_PERFORM_IN_PLACE_FILE_ENC:
		ret = pft_set_inplace_file(command, buf_size);
		break;
	default:
		pr_err("Invalid command_op_code %u\n", command->opcode);
		pft_set_response(PFT_CMD_RESP_INVALID_COMMAND);
		return 0;
	}

	return ret;
}

static int pft_device_open(struct inode *inode, struct file *file)
{
	int ret;

	mutex_lock(&pft_dev->lock);
	if (pft_dev->open_count > 0) {
		pr_err("PFT device is already opened (%d)\n",
		       pft_dev->open_count);
		ret = -EBUSY;
	} else {
		pft_dev->open_count++;
		pft_dev->pfm_pid = current_pid();
		pr_debug("PFT device opened by %d (%d)\n",
			 pft_dev->pfm_pid, pft_dev->open_count);
		ret = 0;
	}
	mutex_unlock(&pft_dev->lock);

	pr_debug("device opened, count %d\n", pft_dev->open_count);

	return ret;
}

static int pft_device_release(struct inode *inode, struct file *file)
{
	mutex_lock(&pft_dev->lock);
	if (pft_dev->open_count > 0)
		pft_dev->open_count--;
	pft_dev->pfm_pid = UINT_MAX;
	mutex_unlock(&pft_dev->lock);

	pr_debug("device released, count %d\n", pft_dev->open_count);

	return 0;
}

/* 
                                                     
  
                                                         
                                             
                                                               
                        
                                                             
                   
 */
static ssize_t pft_device_write(struct file *filp, const char __user *user_buff,
				size_t size, loff_t *f_pos)
{
	int ret;
	char *cmd_buf;

	if (size > PFT_MAX_COMMAND_SIZE || !user_buff || !f_pos) {
		pr_err("inavlid parameters.\n");
		return -EINVAL;
	}

	cmd_buf = kzalloc(size, GFP_KERNEL);
	if (cmd_buf == NULL) {
		pr_err("malloc failure for command buffer\n");
		return -ENOMEM;
	}

	ret = copy_from_user(cmd_buf, user_buff, size);
	if (ret) {
		pr_err("Unable to copy from user (err %d)\n", ret);
		kfree(cmd_buf);
		return -EFAULT;
	}

	ret = pft_handle_command(cmd_buf, size);
	if (ret) {
		kfree(cmd_buf);
		return -EFAULT;
	}

	kfree(cmd_buf);

	return size;
}

/* 
                                                       
  
                                                                
           
 */
static ssize_t pft_device_read(struct file *filp, char __user *buffer,
			       size_t length, loff_t *f_pos)
{
	int ret = 0;

	if (!buffer || !f_pos || length < sizeof(pft_dev->response)) {
		pr_err("inavlid parameters.\n");
		return -EFAULT;
	}

	ret = copy_to_user(buffer, &(pft_dev->response),
			   sizeof(pft_dev->response));
	if (ret) {
		pr_err("Unable to copy to user, err = %d.\n", ret);
		return -EINVAL;
	}

	return sizeof(pft_dev->response);
}


static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = pft_device_read,
	.write = pft_device_write,
	.open = pft_device_open,
	.release = pft_device_release,
};

static int __init pft_register_chardev(void)
{
	int rc;
	unsigned baseminor = 0;
	unsigned count = 1;
	struct device *class_dev;

	rc = alloc_chrdev_region(&pft_dev->device_no, baseminor, count,
				 DEVICE_NAME);
	if (rc < 0) {
		pr_err("alloc_chrdev_region failed %d\n", rc);
		return rc;
	}

	pft_dev->driver_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(pft_dev->driver_class)) {
		rc = -ENOMEM;
		pr_err("class_create failed %d\n", rc);
		goto exit_unreg_chrdev_region;
	}

	class_dev = device_create(pft_dev->driver_class, NULL,
				  pft_dev->device_no, NULL,
				  DEVICE_NAME);
	if (!class_dev) {
		pr_err("class_device_create failed %d\n", rc);
		rc = -ENOMEM;
		goto exit_destroy_class;
	}

	cdev_init(&pft_dev->cdev, &fops);
	pft_dev->cdev.owner = THIS_MODULE;

	rc = cdev_add(&pft_dev->cdev, MKDEV(MAJOR(pft_dev->device_no), 0), 1);
	if (rc < 0) {
		pr_err("cdev_add failed %d\n", rc);
		goto exit_destroy_device;
	}

	return 0;

exit_destroy_device:
	device_destroy(pft_dev->driver_class, pft_dev->device_no);
exit_destroy_class:
	class_destroy(pft_dev->driver_class);
exit_unreg_chrdev_region:
	unregister_chrdev_region(pft_dev->device_no, 1);
	return rc;
}

static void __exit pft_unregister_chrdev(void)
{
	cdev_del(&pft_dev->cdev);
	device_destroy(pft_dev->driver_class, pft_dev->device_no);
	class_destroy(pft_dev->driver_class);
	unregister_chrdev_region(pft_dev->device_no, 1);

}

static void  __exit pft_free_open_files_list(void)
{
	struct pft_file_info *tmp = NULL;
	struct list_head *pos = NULL;
	struct list_head *next = NULL;

	mutex_lock(&pft_dev->lock);
	list_for_each_safe(pos, next, &pft_dev->open_file_list) {
		tmp = list_entry(pos, struct pft_file_info, list);
		list_del(&tmp->list);
		kfree(tmp);
	}
	mutex_unlock(&pft_dev->lock);
}

static void __exit pft_exit(void)
{
	if (pft_dev == NULL)
		return;

	pft_unregister_chrdev();
	pft_free_open_files_list();

	kfree(pft_dev->uid_table);
	kfree(pft_dev);
	pft_dev = NULL;
}

static int __init pft_init(void)
{
	int ret;
	struct pft_device *dev = NULL;

	dev = kzalloc(sizeof(struct pft_device), GFP_KERNEL);
	if (dev == NULL) {
		pr_err("No memory for device structr\n");
		return -ENOMEM;
	}
	pft_dev = dev;

	dev->state = PFT_STATE_DEACTIVATED;
	dev->pfm_pid = UINT_MAX;

	INIT_LIST_HEAD(&dev->open_file_list);
	mutex_init(&dev->lock);

	ret = pft_register_chardev();
	if (ret) {
		pr_err("create character device failed.\n");
		goto fail;
	}

	pr_info("Drivr initialized successfully %s %s.n", __DATE__, __TIME__);

	return 0;

fail:
	pr_err("Failed to init driver.\n");
	kfree(dev);
	pft_dev = NULL;

	return -ENODEV;
}

module_init(pft_init);
module_exit(pft_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Per-File-Tagger driver");
