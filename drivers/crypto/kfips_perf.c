/*
 *      Copyright (c) INSIDE Secure Oy 2011-2014.
 *      All Rights Reserved
 *
 *      This software is open source; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This General Public License does NOT permit incorporating this software
 *      into proprietary programs.  If you are unable to comply with the GPL, a
 *      commercial license for this software may be purchased from INSIDE
 *      Secure at
 *      www.insidesecure.com/Produits/Security-Solutions-for-Android/MatrixDAR
 *
 *      This program is distributed in WITHOUT ANY WARRANTY; without even the
 *      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *      See the GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *      http : //www.gnu.org/copyleft/gpl.html
 */
/*                                                                            */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/crypto.h>
#include <linux/vmalloc.h>
#include <linux/percpu.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/version.h>
#include <asm/div64.h>
#include <crypto/scatterwalk.h>
#include "kfips_perf.h"

/*                                                           */
#define KFIPS_CPU_BALANCE

#ifdef KFIPS_PROC_STATUS
#define KFIPS_PROC_STATUS_NAME KFIPS_PROC_NAME "_status"
#endif /*                   */

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3, 9, 0))
#define KFIPS_USE_CREATE_PROC_ENTRY
#endif /*                                            */

/*                                                                  */

/*                                                     */
static int uid = 1000;

/*                                                                      
                            */
#ifndef KFIPS_MAX_WORKERS
#define KFIPS_MAX_WORKERS 1
#endif /*                   */

/*
                                                
                                                                          
                                                                          
                                                                                
                                                                           
                                                                             
                                                                             
                                                                           
                                                                      
                 
 */
#ifdef CONFIG_ARCH_MSM8974_NEXUS5_MATRIXDAR111
#if KFIPS_MAX_WORKERS >= 4
#ifndef KFIPS_BUSYCOUNT_DEFAULT
#define KFIPS_BUSYCOUNT_DEFAULT { 0, 0, 0, 0 }
#endif /*                         */
#ifndef KFIPS_CPU_BALANCE_FLAG_DEFAULT
#define KFIPS_CPU_BALANCE_FLAG_DEFAULT { 1, 1, 1, 1 }
#endif /*                                */
#ifndef KFIPS_CPU_BALANCE_AFFINITY_DEFAULT
#define KFIPS_CPU_BALANCE_AFFINITY_DEFAULT { 2, 4, 8, 1 }
#endif /*                                    */
#ifndef KFIPS_PENDING_MAX_DEFAULT
#define KFIPS_PENDING_MAX_DEFAULT 200000
#endif /*                           */
#ifndef KFIPS_PENDING_HIGH_DEFAULT
#define KFIPS_PENDING_HIGH_DEFAULT 190000
#endif /*                           */
#ifndef KFIPS_PENDING_LOW_DEFAULT
#define KFIPS_PENDING_LOW_DEFAULT 102400
#endif /*                           */
#ifndef KFIPS_CPU_MAX_DEFAULT
#define KFIPS_CPU_MAX_DEFAULT 3
#endif /*                       */
#endif /*                   */
#endif /*                                         */

/*                                                     
                                                                      
                                                                      
              
                                                                    
                                                                    
                                                               */
#ifndef KFIPS_BUSYCOUNT_DEFAULT
#define KFIPS_BUSYCOUNT_DEFAULT { }
#endif /*                         */
#ifndef KFIPS_CPU_BALANCE_FLAG_DEFAULT
#define KFIPS_CPU_BALANCE_FLAG_DEFAULT { }
#endif /*                                */
#ifndef KFIPS_CPU_BALANCE_AFFINITY_DEFAULT
#define KFIPS_CPU_BALANCE_AFFINITY_DEFAULT { }
#endif /*                                    */
#ifndef KFIPS_PENDING_MAX_DEFAULT
#ifdef KFIPS_PENDING_LEN
#define KFIPS_PENDING_MAX_DEFAULT KFIPS_PENDING_LEN
#else /*                   */
#define KFIPS_PENDING_MAX_DEFAULT 128
#endif /*                   */
#endif /*                           */
#ifndef KFIPS_PENDING_HIGH_DEFAULT
#define KFIPS_PENDING_HIGH_DEFAULT (KFIPS_PENDING_MAX_DEFAULT / 2)
#endif /*                           */
#ifndef KFIPS_PENDING_LOW_DEFAULT
#define KFIPS_PENDING_LOW_DEFAULT 0
#endif /*                           */
#ifndef KFIPS_CPU_MAX_DEFAULT
#define KFIPS_CPU_MAX_DEFAULT KFIPS_MAX_WORKERS
#endif /*                       */

/*                 */
struct kfips_request_context *g_inplay[KFIPS_MAX_WORKERS][KFIPS_RING_ENTRIES];

/*                                             */
static unsigned g_busycount[KFIPS_MAX_WORKERS] = KFIPS_BUSYCOUNT_DEFAULT;

/*                                                                       
           */
static unsigned g_cpu_balance_flag[KFIPS_MAX_WORKERS] =
	KFIPS_CPU_BALANCE_FLAG_DEFAULT;

/*                                            */
static unsigned g_cpu_balance_affinity[KFIPS_MAX_WORKERS] =
	KFIPS_CPU_BALANCE_AFFINITY_DEFAULT;

/*                                          */
static const unsigned g_busycount_default[KFIPS_MAX_WORKERS] =
	KFIPS_BUSYCOUNT_DEFAULT;
static const unsigned g_cpu_balance_flag_default[KFIPS_MAX_WORKERS] =
	KFIPS_CPU_BALANCE_FLAG_DEFAULT;
static unsigned g_cpu_balance_affinity_default[KFIPS_MAX_WORKERS] =
	KFIPS_CPU_BALANCE_AFFINITY_DEFAULT;

/*                                                                    */
struct kfips_transform_context {
	int keylen;
	uint32_t key[(AES_MAX_KEY_SIZE * 2) / sizeof(uint32_t)];
};

/*                                                                
                                                                   */
struct kfips_request_context {
	/*                                                        
                          */
	struct list_head list;

	/*                                            */
	uint32_t flags;

	/*                  */
	struct ablkcipher_request *req;
};

#ifdef KFIPS_PROC_STATUS
/*                               */
static struct proc_dir_entry *g_proc_status_entry;
#endif
/*             */
static struct proc_dir_entry *g_proc_entry;

/*                                                                  
                  */
static spinlock_t g_lock;
static struct list_head g_pending;

/*                                         */
static wait_queue_head_t g_file_wq;
static wait_queue_head_t g_cpu_wq;

static uint32_t g_pending_len;

/*                         */
static uint32_t g_pending_max = KFIPS_PENDING_MAX_DEFAULT;
static uint32_t g_pending_low = KFIPS_PENDING_LOW_DEFAULT;
static uint32_t g_pending_high = KFIPS_PENDING_HIGH_DEFAULT;
static const uint32_t g_pending_max_default = KFIPS_PENDING_MAX_DEFAULT;
static const uint32_t g_pending_low_default = KFIPS_PENDING_LOW_DEFAULT;
static const uint32_t g_pending_high_default = KFIPS_PENDING_HIGH_DEFAULT;

/*                          */
static int g_num_queues;

/*                                                             */
static DEFINE_SPINLOCK(g_kfips_lock);

/*                                                     */
static unsigned long long g_num_queue_busy;

static unsigned long g_cpu_mask;
static atomic_t g_cpu_num = ATOMIC_INIT(0);
static atomic_t g_cpu_num_current = ATOMIC_INIT(0);
static int g_cpu_max = KFIPS_CPU_MAX_DEFAULT;
static const int g_cpu_max_default = KFIPS_CPU_MAX_DEFAULT;

#ifdef KFIPS_PROC_STATUS
static pid_t g_pid_perworker[KFIPS_MAX_WORKERS];
static unsigned long long g_requests_perworker[KFIPS_MAX_WORKERS];
static unsigned long long g_encrypt_bytes_perworker[KFIPS_MAX_WORKERS];
static unsigned long long g_decrypt_bytes_perworker[KFIPS_MAX_WORKERS];
static unsigned long long g_wakeup_perworker[KFIPS_MAX_WORKERS];
#endif /*                   */

/*                                                                
                                                                 
                                                                      
                                   */
static int sg_count(struct scatterlist *sg, size_t nbytes)
{
	int n = 0;

	while (sg && nbytes > 0) {
		n++;
		nbytes -= sg->length;
		sg = scatterwalk_sg_next(sg);
	}
	if (nbytes > 0 && !sg)
		pr_err("too short input to sg_count!");

	return n;
}

/*                                         */
static __must_check int sg_copy_from_user_buffer(
	struct scatterlist *sg,
	unsigned int nents,
	const unsigned char __user *src,
	size_t nbytes)
{
	int ret = 0;
	struct sg_mapping_iter miter;

	sg_miter_start(&miter, sg, nents, SG_MITER_TO_SG);
	while (sg_miter_next(&miter) && nbytes > 0 && miter.addr) {
		size_t len;

		len = min(miter.length, nbytes);
		if (__copy_from_user(miter.addr, src, len)) {
			ret = -EINVAL;
			goto error_sg_copy_from_user_buffer;
		}
		nbytes -= len;
		src += len;
	}
	/*                                                        */
	BUG_ON(nbytes != 0);
error_sg_copy_from_user_buffer:
	sg_miter_stop(&miter);
	return ret;
}

/*                                         */
static __must_check int sg_copy_to_user_buffer(
	struct scatterlist *sg,
	unsigned int nents,
	unsigned char __user *dst,
	size_t nbytes)
{
	int ret = 0;
	struct sg_mapping_iter miter;

	sg_miter_start(&miter, sg, nents, SG_MITER_FROM_SG);
	while (sg_miter_next(&miter) && nbytes > 0 && miter.addr) {
		size_t len;

		len = min(miter.length, nbytes);
		if (__copy_to_user(dst, miter.addr, len)) {
			ret = -EINVAL;
			goto error_sg_copy_to_user_buffer;
		}
		nbytes -= len;
		dst += len;
	}
	/*                                                        */
	BUG_ON(nbytes != 0);
 error_sg_copy_to_user_buffer:
	sg_miter_stop(&miter);
	return ret;
}

#ifdef KFIPS_CPU_BALANCE
/*                                                                   
                                                                     
                            */
static void cpu_balance2(struct task_struct *task, unsigned int worker_id)
{
	const unsigned long cpumasks[2] = { 0x55555555UL, 0xAAAAAAAAUL };
	struct cpumask cpumask = { CPU_BITS_NONE };
	int ret;

	/*                                       */
	cpumask_set_cpu(1 - worker_id, &cpumask);
	if (!cpumask_intersects(&task->cpus_allowed, &cpumask)) {
		/*                                                  */
		return;
	}

	cpumask.bits[0] = cpumasks[worker_id];

	/*                                        */
	ret = set_cpus_allowed_ptr(task, &cpumask);
	/*                                            
                                                                  */
}

/*                                                                   
              */
static void cpu_balance(struct task_struct *task, unsigned int worker_id)
{
	struct cpumask cpumask = { CPU_BITS_NONE };
	int ret;
	int cpus;

	if (g_cpu_balance_affinity[worker_id]) {
		/*                                                */
		memcpy(&cpumask, &g_cpu_balance_affinity[worker_id],
		       sizeof(g_cpu_balance_affinity[worker_id]));

		if (cpumask_equal(&task->cpus_allowed, &cpumask)) {
			/*                                                  */
			return;
		}

		/*                                        */
		ret = set_cpus_allowed_ptr(task, &cpumask);
		/*                                            
                                             
                              */
		return;
	}

	cpus = atomic_read(&g_cpu_num_current);

	if (cpus == 1) {
		return;
	} else if (cpus == 2) {
		/*                            */
		if (worker_id < 2)
			cpu_balance2(task, worker_id);
		return;
	}

	/*                                  */
	cpumask_set_cpu((unsigned int) worker_id, &cpumask);
	if (cpumask_equal(&task->cpus_allowed, &cpumask)) {
		/*                                                  */
		return;
	}

	/*                                        */
	ret = set_cpus_allowed_ptr(task, &cpumask);
	/*                                            
                                                                     */
}
#endif /*                   */

static int file_open(struct inode *inode, struct file *filp);
static int file_release(struct inode *inode, struct file *filp);
static int file_mmap(struct file *, struct vm_area_struct *vma);
static long file_ioctl(struct file *, unsigned int cmd, unsigned long arg);
static ssize_t file_read(struct file *, char *b, size_t c, loff_t *pos);
static ssize_t file_write(struct file *, const char *b, size_t c,
			  loff_t *pos);

static const struct file_operations file_fops = {
	.open = file_open,
	.release = file_release,
	.unlocked_ioctl = file_ioctl,
	.read = file_read,
	.write = file_write,
	.mmap = file_mmap,
	.owner = THIS_MODULE,
};

#ifndef KFIPS_USE_CREATE_PROC_ENTRY
#ifdef KFIPS_PROC_STATUS
/*                          */
static int kfips_proc_status_read(char *buf,
				  char **start,
				  off_t off,
				  int count,
				  int *eof,
				  void *data);

static const struct file_operations status_file_fops = {
	.read = kfips_proc_status_read
};

#endif /*                   */
#endif /*                             */

/*                                                                     
           */
static int file_open(struct inode *inode, struct file *filp)
{
	unsigned int current_worker;

	spin_lock_bh(&g_kfips_lock);
	current_worker = g_num_queues;

	if (current_worker >= KFIPS_MAX_WORKERS) {
		spin_unlock_bh(&g_kfips_lock);
		pr_err("Too many workers. (could not attach pid %d).\n",
		       (int) (current->pid));
		return -EPERM;
	}

	g_num_queues = current_worker + 1;
	spin_unlock_bh(&g_kfips_lock);
	filp->private_data = (void *)(unsigned long)current_worker;
#ifdef KFIPS_PROC_STATUS
	g_pid_perworker[current_worker] = current->pid;
#endif
	pr_info("Process %d connected - %d queues available\n",
		current->pid, g_num_queues);
	return 0;
}

/*                             */
static int file_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return -EINVAL;
}

/*                                               */
static uint32_t get_reqs(struct kfips_request_context *reqs[],
			 unsigned int current_worker,
			 uint32_t num_req,
			 size_t   szleft)
{
	uint32_t count = 0;
	spin_lock_bh(&g_lock);
	while (num_req && !list_empty(&g_pending)) {
		struct kfips_request_context *req;
		req = (struct kfips_request_context *)g_pending.next;
		if (!req || !req->req || req->req->nbytes > szleft) {
			pr_debug("Buffer full at %dth data request",
				 (int) count);
			break;
		}
		list_del(&req->list);
		reqs[count] = req;
		count++;
		num_req--;
		g_pending_len--;
		szleft -= req->req->nbytes; /*                   */
	}
	spin_unlock_bh(&g_lock);
	return count;
}

/*                        */
static int transfer_user_reply(uint32_t flags,
			       uint32_t context,
			       struct kfips_request_context *rctx,
			       void __user *data)
{
	/*                                              */
	struct ablkcipher_request *req = rctx->req;
	int bytes = 0;

	if ((flags & KFIPS_FLAGS_SEND) != 0) {
		int ret;

		bytes = req->nbytes;
		ret = sg_copy_from_user_buffer(req->dst,
					       sg_count(req->dst, bytes),
					       data, req->nbytes);

		/*                                                        */
		req->base.complete(&req->base, ret);
	} else {
		/*                                  */
		req->base.complete(&req->base, -EINVAL);
	}
	return bytes;
}

/*                          */
static uint32_t transfer_user_req(uint32_t flags,
				  uint32_t context,
				  struct kfips_request_context *rctx,
				  struct kfips_ctrl __user *ctrl,
				  void __user *data)
{
	/*                                              */

	/*                      */
	long comb;
	struct ablkcipher_request *req = rctx->req;
	struct kfips_transform_context *ctx =
	crypto_ablkcipher_ctx(crypto_ablkcipher_reqtfm(req));

	uint32_t len = req->nbytes;
	uint32_t keylen = ctx->keylen;

	comb = __copy_to_user(&ctrl->key, ctx->key, ctx->keylen);
	comb |= __copy_to_user(&ctrl->iv, req->info, AES_BLOCK_SIZE);
	comb |= __put_user(len, &ctrl->len) == -EFAULT;
	comb |= __put_user(keylen, &ctrl->keylen) == -EFAULT;

	comb |= sg_copy_to_user_buffer(req->src,
				       sg_count(req->src,
						req->nbytes),
				       data, req->nbytes);

	if (!comb)
		return keylen; /*                             */
	else
		return 0; /*                    */
}

/*                                                                       */
static int transfer_user_replies(struct kfips_req __user *reqs,
				 unsigned char __user *data_p,
				 unsigned int current_worker,
				 uint32_t num_req)
{
	uint32_t blank = KFIPS_FLAGS_BLANK;
	int ret = 0;
	uint32_t i;

	for (i = 0; i < num_req; i++) {
		uint32_t flags;
		__get_user(flags, &reqs->flags);
		pr_debug("Check flags (%d): %x\n", i, flags);
		if ((flags & (KFIPS_FLAGS_SEND | KFIPS_FLAGS_ERR)) != 0) {
			uint32_t context;
			struct kfips_request_context *rctx;
			__get_user(context, &reqs->context);

			pr_debug("Processing from user (%d): wrk: %d ctx: %u\n",
				 i, (int)current_worker,
				 (unsigned int) context);

			rctx = g_inplay[current_worker][context &
							KFIPS_RING_INDEX_MASK];
			if (!rctx) {
				pr_debug("unable to get rctx: (%u,%u) (idx=%d)",
					 (unsigned int) current_worker,
					 (unsigned int) context, (int) i);
				return -EFAULT;
			}
			g_inplay[current_worker][context &
						 KFIPS_RING_INDEX_MASK] = NULL;
			pr_debug("rctx: %p req: %p from data = %p\n",
				 rctx, rctx->req, data_p);
			data_p += transfer_user_reply(flags, context, rctx,
						      data_p);
			__put_user(blank, &reqs->flags);
		}
		reqs += 1;
	}
	return ret;
}

/*                      */
static int transfer_reqs(struct kfips_req __user *reqs,
			 unsigned char __user *datap,
			 struct kfips_request_context **rctxs,
			 unsigned int current_worker,
			 uint32_t num_req)
{
	int ret = 0;
	uint32_t i;

#ifdef KFIPS_PROC_STATUS
	/*                                             */
	g_requests_perworker[current_worker] += 1;
#endif /*                   */

	for (i = 0; i < num_req; i++) {
		uint32_t flags;
		__get_user(flags, &reqs->flags);
		pr_debug("Sending req (%d) to userspace\n", i);
		if ((flags & (KFIPS_FLAGS_BLANK)) != 0) {
			uint32_t context;
			uint32_t kl;
			context = i;

			if (g_inplay[current_worker][i])
				return -EFAULT;

			pr_debug("Memorize inplay(%d, %d) = %p\n",
				 current_worker, i, *rctxs);
			g_inplay[current_worker][i] = *rctxs;
			flags = (*rctxs)->flags;

			/*                                                 
                          */
			if ((flags & KFIPS_FLAGS_BUSY) != 0) {
				struct ablkcipher_request *req;

				/*                                */
				req = (*rctxs)->req;
				req->base.complete(&req->base, -EINPROGRESS);
				flags &= ~KFIPS_FLAGS_BUSY;
				(*rctxs)->flags = flags;
				pr_debug("Worker %d %s, signalling -%s",
					 current_worker,
					 "queue problem resolved",
					 "EINPROGRESS");
			}
			kl = transfer_user_req(flags, context, *rctxs,
					       &reqs->ctrl, datap);

			if (kl == 0 ||
			    (__put_user(flags, &reqs->flags) == -EFAULT) ||
			    (__put_user(context, &reqs->context) == -EFAULT)) {
				/*                                            */
				struct ablkcipher_request *req;

				req = (*rctxs)->req;
				req->base.complete(&req->base, -EINVAL);
				/*                                    */
				flags = KFIPS_FLAGS_BLANK;
				(void)__put_user(flags, &reqs->flags);
			} else {
				/*                         */
				datap += (*rctxs)->req->nbytes;
#ifdef KFIPS_PROC_STATUS
				if ((flags & KFIPS_FLAGS_ENCRYPT) != 0)
					g_encrypt_bytes_perworker[
						current_worker] +=
						(*rctxs)->req->nbytes;
				if ((flags & KFIPS_FLAGS_DECRYPT) != 0)
					g_decrypt_bytes_perworker[
						current_worker] +=
						(*rctxs)->req->nbytes;
#endif /*                   */
			}
		}
		reqs += 1;
		rctxs += 1;
	}
	if (num_req == 0)
		pr_debug("Sending no reqs to userspace\n");
	return ret;
}

#ifdef KFIPS_PROC_STATUS
/*                                              */
static inline int kfips_wakeup_count(unsigned int current_worker)
{
	g_wakeup_perworker[current_worker]++;
	return 1;
}
#endif /*                   */
/*                                        
                                                                 
                            */
static int long_sleep(unsigned int current_worker)
{
	int ret = 0;
	if (current_worker == 0)
		return 0; /*                           */

	/*                                                             
                              
                                                                 
                                         */
	if (current_worker >= atomic_read(&g_cpu_num_current)) {
		ret = wait_event_interruptible(
			g_cpu_wq,
			g_pending_len >= g_pending_high ? 1 :
			(g_pending_len >= g_pending_low ?
			 current_worker <
			 atomic_read(&g_cpu_num_current) : 0));
	}
	return ret;
}

/*                                                   */
struct userspace_req {
	struct kfips_req reqs[KFIPS_RING_ENTRIES_SAFE];
	uint8_t          data[KFIPS_BUFFER_SIZE*KFIPS_RING_ENTRIES_SAFE];
};

/*                                                        */
static long file_ioctl(struct file *filp,
		       unsigned int cmd,
		       unsigned long arg)
{
	uint32_t blank = KFIPS_FLAGS_BLANK;
	unsigned int current_worker = ((unsigned int)(unsigned long)
				       (filp->private_data));
	int ret = 0;
	uint32_t iosize = ((cmd >> _IOC_SIZESHIFT) & _IOC_SIZEMASK);
	uint32_t num_req = KFIPS_RING_ENTRIES_SAFE;
	uint32_t num_reqo;
	struct userspace_req __user *userp = (void __user *)arg;
	unsigned char __user *datap = (void __user *)(&userp->data);
	struct kfips_request_context *reqs[KFIPS_RING_ENTRIES];

	BUG_ON(current_worker == ((unsigned int)-1));

	if (num_req == 0)
		return 0; /*                            */

	pr_debug("IOCTL Req: cmd = %x, expect = %x sz: %zu/%zu <%zu : %zu >\n",
		 cmd,
		 (unsigned int) (KFIPS_QUEUE_IOCTL(struct userspace_req)),
		 sizeof(struct kfips_req), sizeof(struct userspace_req),
		 sizeof(struct userspace_req) / sizeof(struct kfips_req),
		 sizeof(struct userspace_req) % sizeof(struct kfips_req));

	if (cmd == KFIPS_QUEUE_IOCTL(struct userspace_req)) {
		/*                                         */
		if (!access_ok(VERIFY_WRITE, userp, iosize) ||
		    !access_ok(VERIFY_READ, userp, iosize))
			return -EFAULT;

		pr_debug("IOCTL Req: valid\n");

		/*                        */
		ret = transfer_user_replies(userp->reqs, datap,
					    current_worker, num_req);
		if (ret != 0)
			return ret;

		while (num_req > KFIPS_RING_ENTRIES) {
			num_req--;
			__put_user(blank, &userp->reqs[num_req].flags);
		}

		/*                                                       
              */
		if (current_worker != 0 && g_pending_len < g_pending_low) {
			ret = long_sleep(current_worker);
			if (ret != 0)
				return ret;
		}

		num_reqo = get_reqs(reqs, current_worker, num_req,
				    KFIPS_DATA_SIZE);
		if (num_reqo == 0) {
			/*          */
			unsigned busycount = g_busycount[current_worker];
			while (busycount > 0) {
				cpu_relax();
				if (!list_empty(&g_pending)) {
					/*                                
                            */

					num_reqo = get_reqs(
						reqs,
						current_worker,
						num_req,
						KFIPS_DATA_SIZE);
					if (num_reqo != 0)
						break;
				}
				busycount--;
			}
		}

		while (num_reqo == 0) {
			/*                                                   
                                                    */
			ret = long_sleep(current_worker);
			if (ret != 0)
				return ret;

			/*                                  */
			ret = wait_event_interruptible(
				g_file_wq,
#ifdef KFIPS_PROC_STATUS
				kfips_wakeup_count(current_worker) &&
#endif /*                   */
				!list_empty(&g_pending));

			if (ret != 0)
				return ret;

			num_reqo = get_reqs(reqs, current_worker, num_req,
					    KFIPS_DATA_SIZE);
		}
		pr_debug("Woken up with %d reqs to handle.\n", num_reqo);

		/*                                       */
		ret = transfer_reqs(userp->reqs, datap, reqs, current_worker,
				    num_reqo);
		/*                                */
		while (num_req > num_reqo) {
			num_req--;
			(void)__put_user(blank, &userp->reqs[num_req].flags);
		}
	} else {
		/*                                   */
		ret = -EINVAL;
	}

#ifdef KFIPS_CPU_BALANCE
	/*                                                      */
	if (g_cpu_balance_flag[current_worker])
		cpu_balance(get_current(),
			    (unsigned int) (unsigned long) filp->private_data);
#endif /*                   */

	return ret;
}

/*                                         */
static ssize_t file_read(struct file *filp, char *buf, size_t count,
			 loff_t *pos)
{
	return -EINVAL;
}

/*                                       */
static ssize_t file_write(struct file *filp, const char *buf, size_t count,
			  loff_t *pos)
{
	return -EINVAL;
}

/*                    */
static int file_release(struct inode *inode, struct file *filp)
{
	int i;
	unsigned int current_worker = (unsigned int)(unsigned long)
				      (filp->private_data);
	BUG_ON(current_worker >= KFIPS_MAX_WORKERS);
	pr_info("Process %d disconnected\n", current->pid);
#ifdef KFIPS_PROC_STATUS
	g_pid_perworker[current_worker] = 0;
#endif

	/*                                            */
	for (i = 0; i < KFIPS_RING_INDEX_MASK; i++) {
		struct kfips_request_context *rctx =
			g_inplay[current_worker][i];
		if (rctx)
			transfer_user_reply(KFIPS_FLAGS_ERR, i, rctx, NULL);
		g_inplay[current_worker][i] = 0;
	}

	/*                                              */
	spin_lock_bh(&g_kfips_lock);
	g_num_queues--;
	spin_unlock_bh(&g_kfips_lock);
	return 0;
}

/*                                             
                                                                         */
static int kfips_aes_qcrypt(struct ablkcipher_request *req, uint32_t flags)
{
	struct kfips_request_context *rctx = ablkcipher_request_ctx(req);
	int rc;
	int rflags = req->base.flags;
	uint32_t pending_len;

	/*                                                        */
	if (req->nbytes < AES_BLOCK_SIZE) {
		pr_err("request size %d less than AES block size\n",
		       (int)req->nbytes);
		return -EINVAL;
	}
	if (req->nbytes > KFIPS_DATA_SIZE) {
		pr_err("request size %d greater than maximum supported\n",
		       (int)req->nbytes);
		return -ENOMEM;
	}

	/*                                                          
                                                                      
 */
	if (req->nbytes % AES_BLOCK_SIZE) {
		pr_err("request size is not multiple of AES block size\n");
		return -EINVAL;
	}
	if (!(rflags & CRYPTO_TFM_REQ_MAY_SLEEP)) {
		pr_err("non-sleeping request for kfips");
		return -EINVAL;
	}

	if (!req->base.complete) {
		pr_err("no completion callback?!?");
		return -EINVAL;
	}

	/*                                                                */
	rctx->flags = flags;
	rctx->req = req;

	/*                 */
	spin_lock_bh(&g_lock);

	list_add_tail(&rctx->list, &g_pending);
	pending_len = (++g_pending_len);

	/*                                      */
	if (pending_len > g_pending_max) {
		/*                */
		rc = -EBUSY;
		rctx->flags |= KFIPS_FLAGS_BUSY;
		pr_debug("Pending queue is too long, signalling -EBUSY");

		g_num_queue_busy++;
	} else {
		/*                                         */
		rc = -EINPROGRESS;
	}

	spin_unlock_bh(&g_lock);

	if (pending_len > g_pending_high) {
		/*                                 */
		wake_up_interruptible(&g_cpu_wq);
	}

	/*                                                    */
	wake_up_interruptible(&g_file_wq);

	return rc;
}

/*                                                         */
static int kfips_aes_setkey(struct crypto_ablkcipher *tfm, const u8 *key,
			    unsigned int keylen)
{
	struct ablkcipher_alg *alg = crypto_ablkcipher_alg(tfm);
	struct kfips_transform_context *ctx = crypto_ablkcipher_ctx(tfm);

	if (alg->max_keysize == AES_MAX_KEY_SIZE) {
		if (keylen != KFIPS_KEY_ID_SIZE &&
		    keylen != AES_KEYSIZE_128 &&
		    keylen != AES_KEYSIZE_192 &&
		    keylen != AES_KEYSIZE_256)
			return -EINVAL;
	} else if (alg->max_keysize == AES_MAX_KEY_SIZE * 2) {
		if (keylen != KFIPS_KEY_ID_SIZE &&
		    keylen != AES_KEYSIZE_128 * 2 &&
		    keylen != AES_KEYSIZE_256 * 2)
			return -EINVAL;
	} else {
		return -EINVAL;
	}
	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;
	return 0;
}

/*                                                    */
static int kfips_aes_ecb_encrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req,
				KFIPS_FLAGS_ENCRYPT | KFIPS_FLAGS_ECB);
}

/*                                                    */
static int kfips_aes_ecb_decrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req, KFIPS_FLAGS_DECRYPT | KFIPS_FLAGS_ECB);
}

/*                                                    */
static int kfips_aes_cbc_encrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req,
				KFIPS_FLAGS_ENCRYPT | KFIPS_FLAGS_CBC);
}

/*                                                    */
static int kfips_aes_cbc_decrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req, KFIPS_FLAGS_DECRYPT | KFIPS_FLAGS_CBC);
}

/*                                                    */
static int kfips_aes_xts_encrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req,
				KFIPS_FLAGS_ENCRYPT | KFIPS_FLAGS_XTS);
}

/*                                                    */
static int kfips_aes_xts_decrypt(struct ablkcipher_request *req)
{
	return kfips_aes_qcrypt(req, KFIPS_FLAGS_DECRYPT | KFIPS_FLAGS_XTS);
}

/*                           */
static int kfips_aes_cra_init(struct crypto_tfm *tfm)
{
	tfm->crt_ablkcipher.reqsize = sizeof(struct kfips_request_context);
	return 0;
}

/*                           */
static void kfips_aes_cra_exit(struct crypto_tfm *tfm)
{
}

static struct crypto_alg algs[] = {
	{
		.cra_name = "ecb(fipsaes)",
		.cra_driver_name = "ecb-fipsaes",
		.cra_priority = 500,
		.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize = AES_BLOCK_SIZE,
		.cra_ctxsize = sizeof(struct kfips_transform_context),
		.cra_alignmask = 0,
		.cra_type = &crypto_ablkcipher_type,
		.cra_module = THIS_MODULE,
		.cra_init = kfips_aes_cra_init,
		.cra_exit = kfips_aes_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = 0,
			.setkey = kfips_aes_setkey,
			.encrypt = kfips_aes_ecb_encrypt,
			.decrypt = kfips_aes_ecb_decrypt,
		}
	}, {
		.cra_name = "cbc(fipsaes)",
		.cra_driver_name = "cbc-fipsaes",
		.cra_priority = 500,
		.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize = AES_BLOCK_SIZE,
		.cra_ctxsize = sizeof(struct kfips_transform_context),
		.cra_alignmask = 0,
		.cra_type = &crypto_ablkcipher_type,
		.cra_module = THIS_MODULE,
		.cra_init = kfips_aes_cra_init,
		.cra_exit = kfips_aes_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize = AES_MIN_KEY_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE,
			.ivsize = AES_BLOCK_SIZE,
			.setkey = kfips_aes_setkey,
			.encrypt = kfips_aes_cbc_encrypt,
			.decrypt = kfips_aes_cbc_decrypt,
		}
	}, {
		.cra_name = "xts(fipsaes)",
		.cra_driver_name = "xts-fipsaes",
		.cra_priority = 500,
		.cra_flags = CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize = AES_BLOCK_SIZE,
		.cra_ctxsize = sizeof(struct kfips_transform_context),
		.cra_alignmask = 0,
		.cra_type = &crypto_ablkcipher_type,
		.cra_module = THIS_MODULE,
		.cra_init = kfips_aes_cra_init,
		.cra_exit = kfips_aes_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize = KFIPS_KEY_ID_SIZE,
			.max_keysize = AES_MAX_KEY_SIZE * 2,
			.ivsize = AES_BLOCK_SIZE,
			.setkey = kfips_aes_setkey,
			.encrypt = kfips_aes_xts_encrypt,
			.decrypt = kfips_aes_xts_decrypt,
		}
	}
};

enum kfips_load_state {
	KFIPS_STATE_NOTLOADED,
	KFIPS_STATE_CRYPTO_REGISTERED,
	KFIPS_STATE_LOADED
};

/*         */
static void kfips_aes_mod_unload(enum kfips_load_state mstate)
{
	int i;

	pr_debug("Unloading kfips from state %d\n", mstate);

	switch (mstate) {
	case KFIPS_STATE_LOADED:
		remove_proc_entry(KFIPS_PROC_NAME, NULL);
	case KFIPS_STATE_CRYPTO_REGISTERED:
		for (i = 0; i < sizeof(algs) / sizeof(algs[0]); i++)
			crypto_unregister_alg(&algs[i]);
	case KFIPS_STATE_NOTLOADED:
		break;
	default:
		pr_err("Invalid module state!\n");
		break;
	}

#ifdef KFIPS_PROC_STATUS
	remove_proc_entry(KFIPS_PROC_STATUS_NAME, NULL);
#endif /*                   */
}

#ifdef KFIPS_PROC_STATUS
/*                          */
static int kfips_proc_status_read(char *buf,
				  char **start,
				  off_t off,
				  int count,
				  int *eof,
				  void *data)
{
	int len = 0;
	unsigned l_count;
	unsigned int current_worker;

	if (off > 0) {
		*eof = 1;
		return 0;
	}

	/*                     */
	spin_lock_bh(&g_kfips_lock);
	len += snprintf(buf + len, PAGE_SIZE - len,
			"Num queues: %d/%d\n", g_num_queues,
			KFIPS_MAX_WORKERS);
	len += snprintf(buf + len, PAGE_SIZE - len,
			"Queue busy: %llu\n", g_num_queue_busy);
	spin_unlock_bh(&g_kfips_lock);
	len += snprintf(buf + len, PAGE_SIZE - len,
			"CPU online mask: 0x%lx\n", g_cpu_mask);
	len += snprintf(buf + len, PAGE_SIZE - len,
			"CPU online num: %u\n",
			(unsigned) atomic_read(&g_cpu_num));

	/*                             */

	len += snprintf(buf + len, PAGE_SIZE - len, "Pid : ");
	for (current_worker = 0; current_worker < KFIPS_MAX_WORKERS;
	     current_worker++)
		len += snprintf(buf + len, PAGE_SIZE - len, " %u",
				(unsigned int) g_pid_perworker[
					current_worker]);
	len += snprintf(buf + len, PAGE_SIZE - len, "\n");

	len += snprintf(buf + len, PAGE_SIZE - len, "Inplay : ");
	for (current_worker = 0; current_worker < KFIPS_MAX_WORKERS;
	     current_worker++) {
		int c;
		l_count = 0;
		for (c = 0; c < KFIPS_RING_ENTRIES; c++)
			if (g_inplay[current_worker][c])
				l_count++;
		len += snprintf(buf + len, PAGE_SIZE - len, " %u", l_count);
	}
	len += snprintf(buf + len, PAGE_SIZE - len, "\n");

	len += snprintf(buf + len, PAGE_SIZE - len, "Pending len : ");
	len += snprintf(buf + len, PAGE_SIZE - len, " %u/%d",
			(unsigned int)g_pending_len, g_pending_max);
	len += snprintf(buf + len, PAGE_SIZE - len, "\n");

	for (current_worker = 0; current_worker < KFIPS_MAX_WORKERS;
	     current_worker++) {
		spin_lock_bh(&g_lock);
		len += snprintf(buf + len, PAGE_SIZE - len,
				"Worker #%d: Wakeups: %llu\n",
				(int) current_worker,
				g_wakeup_perworker[current_worker]);
		len += snprintf(buf + len, PAGE_SIZE - len,
				"Worker #%d: Total system calls: %llu\n",
				(int) current_worker,
				g_requests_perworker[current_worker]);
		len += snprintf(buf + len, PAGE_SIZE - len,
				"Worker #%d: Total encrypted bytes: %llu\n",
				(int) current_worker,
				g_encrypt_bytes_perworker[current_worker]);
		len += snprintf(buf + len, PAGE_SIZE - len,
				"Worker #%d: Total decrypted bytes: %llu\n",
				(int) current_worker,
				g_decrypt_bytes_perworker[current_worker]);
		spin_unlock_bh(&g_lock);
	}

	buf[len] = 0;
	return len;
}
#endif

/*                                     
                                                */
static void kfips_cpu(int state, unsigned int cpu)
{
	if (state == 0) {
		if (test_and_clear_bit(cpu, &g_cpu_mask)) {
			/*              */
			atomic_dec(&g_cpu_num);
			atomic_set(&g_cpu_num_current,
				   min(atomic_read(&g_cpu_num), g_cpu_max));
		}
	} else {
		if (!test_and_set_bit(cpu, &g_cpu_mask)) {
			/*             */
			atomic_inc(&g_cpu_num);
			atomic_set(&g_cpu_num_current,
				   min(atomic_read(&g_cpu_num), g_cpu_max));
			/*                             */
			wake_up_interruptible(&g_cpu_wq);
		}
	}
}

/*                                                  */
static int kfips_cpu_callback(struct notifier_block *nfb,
			      unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;

	switch (action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		if (cpu < 32 && cpu <= g_num_queues)
			kfips_cpu(1, cpu);
		break;
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		if (cpu < 32 && cpu <= g_num_queues)
			kfips_cpu(0, cpu);
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block kfips_cpu_notifier = {
	.notifier_call = kfips_cpu_callback,
};

static ssize_t sys_g_cpu_max_show(struct kobject *obj,
				  struct kobj_attribute *attr,
				  char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_cpu_max);
}

static ssize_t sys_g_cpu_max_store(struct kobject *obj,
				   struct kobj_attribute *attr,
				   const char *buf,
				   size_t count)
{
	int tmp_cpu_max = KFIPS_MAX_WORKERS;
	if (count == 0 || buf[0] == '\n') {
		g_cpu_max = g_cpu_max_default;
		return count;
	}
	sscanf(buf, "%du", &tmp_cpu_max);
	tmp_cpu_max = max(1, tmp_cpu_max);
	tmp_cpu_max = min(KFIPS_MAX_WORKERS, tmp_cpu_max);
	g_cpu_max = tmp_cpu_max;

	atomic_set(&g_cpu_num_current,
		   min(atomic_read(&g_cpu_num), g_cpu_max));
	/*                             */
	wake_up_interruptible(&g_cpu_wq);
	return count;
}

static struct kobj_attribute thread_normal_active_attribute =
	__ATTR(thread_normal_active, 0644,
	       sys_g_cpu_max_show, sys_g_cpu_max_store);

static ssize_t sys_g_pending_max_show(struct kobject *obj,
				      struct kobj_attribute *attr,
				      char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_pending_max);
}

static ssize_t sys_g_pending_max_store(struct kobject *obj,
				       struct kobj_attribute *attr,
				       const char *buf,
				       size_t count)
{
	if (count == 0 || buf[0] == '\n') {
		g_pending_max = g_pending_max_default;
		return count;
	}
	sscanf(buf, "%uu", &g_pending_max);
	return count;
}

static struct kobj_attribute pending_max_attribute =
	__ATTR(pending_max, 0644,
	       sys_g_pending_max_show, sys_g_pending_max_store);

static ssize_t sys_g_pending_low_show(struct kobject *obj,
				      struct kobj_attribute *attr,
				      char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_pending_low);
}

static ssize_t sys_g_pending_low_store(struct kobject *obj,
				       struct kobj_attribute *attr,
				       const char *buf,
				       size_t count)
{
	if (count == 0 || buf[0] == '\n') {
		g_pending_low = g_pending_low_default;
		return count;
	}
	sscanf(buf, "%uu", &g_pending_low);
	return count;
}

static struct kobj_attribute pending_low_attribute =
	__ATTR(pending_low, 0644,
	       sys_g_pending_low_show, sys_g_pending_low_store);

static ssize_t sys_g_pending_high_show(struct kobject *obj,
				       struct kobj_attribute *attr,
				       char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_pending_high);
}

static ssize_t sys_g_pending_high_store(struct kobject *obj,
					struct kobj_attribute *attr,
					const char *buf,
					size_t count)
{
	if (count == 0 || buf[0] == '\n') {
		g_pending_high = g_pending_high_default;
		return count;
	}
	sscanf(buf, "%uu", &g_pending_high);
	return count;
}

static struct kobj_attribute pending_high_attribute =
	__ATTR(pending_high, 0644,
	       sys_g_pending_high_show, sys_g_pending_high_store);

static int sys_g_get_n(struct kobj_attribute *attr)
{
	int n = 0;
	/*                                                               */
	const char *str = attr->attr.name;
	while (isalpha(*str))
		str++;
	sscanf(str, "%d", &n);
	if (n > KFIPS_MAX_WORKERS)
		n = KFIPS_MAX_WORKERS - 1;
	if (n < 0)
		n = 0;
	return n;
}

static ssize_t sys_g_busycount_n_show(struct kobject *obj,
				      struct kobj_attribute *attr,
				      char *buf)
{
	int n = sys_g_get_n(attr);
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_busycount[n]);
}

static ssize_t sys_g_busycount_n_store(struct kobject *obj,
				       struct kobj_attribute *attr,
				       const char *buf,
				       size_t count)
{
	int n = sys_g_get_n(attr);
	if (count == 0 || buf[0] == 'n') {
		g_busycount[n] = g_busycount_default[n];
		return count;
	}
	sscanf(buf, "%uu", &g_busycount[n]);
	return count;
}

static ssize_t sys_g_cpu_balance_affinity_n_show(struct kobject *obj,
						 struct kobj_attribute *attr,
						 char *buf)
{
	int n = sys_g_get_n(attr);
	return scnprintf(buf, PAGE_SIZE, "%u\n", g_cpu_balance_affinity[n]);
}

static ssize_t sys_g_cpu_balance_affinity_n_store(struct kobject *obj,
						  struct kobj_attribute *attr,
						  const char *buf,
						  size_t count)
{
	int n = sys_g_get_n(attr);
	if (count == 0 || buf[0] == 'n') {
		g_cpu_balance_affinity[n] =
			g_cpu_balance_affinity_default[n];
		return count;
	}
	sscanf(buf, "%uu", &g_cpu_balance_affinity[n]);
	return count;
}

static ssize_t sys_g_cpu_balance_flag_n_show(struct kobject *obj,
					     struct kobj_attribute *attr,
					     char *buf)
{
	static const char * const noyes[] = { "no", "yes" };
	int n = sys_g_get_n(attr);
	return scnprintf(buf, PAGE_SIZE, "%s\n",
			 noyes[g_cpu_balance_flag[n]]);
}

static ssize_t sys_g_cpu_balance_flag_n_store(struct kobject *obj,
					      struct kobj_attribute *attr,
					      const char *buf,
					      size_t count)
{
	int n = sys_g_get_n(attr);
	if (count == 0 || buf[0] == 'n') {
		g_cpu_balance_flag[n] = g_cpu_balance_flag_default[n];
		return count;
	}
	g_cpu_balance_flag[n] = count > 0 && buf[0] != 'n' && buf[0] > '0';
	return count;
}

static ssize_t num_queues_active_show(struct kobject *obj,
				      struct kobj_attribute *attr,
				      char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_num_queues);
}

static struct kobj_attribute num_queues_active_attribute =
	__ATTR_RO(num_queues_active);

static struct module_version_attribute ___modver_attr;
static ssize_t version_show(struct kobject *obj,
				    struct kobj_attribute *attr,
				    char *buf)
{
	/*                                                    */
	return scnprintf(buf, PAGE_SIZE, "%s\n", ___modver_attr.version);
}

static struct kobj_attribute version_show_attribute =
	__ATTR_RO(version);

static ssize_t thread_high_active_show(struct kobject *obj,
				      struct kobj_attribute *attr,
				      char *buf)
{
	/*                                         */
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_num_queues);
}

static struct kobj_attribute thread_high_active_attribute =
	__ATTR_RO(thread_high_active);

static ssize_t pending_len_show(struct kobject *obj,
				struct kobj_attribute *attr,
				char *buf)
{
	/*                                         */
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_pending_len);
}

static struct kobj_attribute pending_len_attribute =
	__ATTR_RO(pending_len);

static char thread_n_busycount_name[KFIPS_MAX_WORKERS][32];
static struct kobj_attribute thread_n_busycount_attributes[
	KFIPS_MAX_WORKERS] = {
	[0 ... KFIPS_MAX_WORKERS-1] = {
		.attr = { .mode = 0644 },
		.show = sys_g_busycount_n_show,
		.store = sys_g_busycount_n_store
	}
};

static char thread_n_cpu_affinity_name[KFIPS_MAX_WORKERS][32];
static struct kobj_attribute thread_n_cpu_affinity_attributes[
	KFIPS_MAX_WORKERS] = {
	[0 ... KFIPS_MAX_WORKERS-1] = {
		.attr = { .mode = 0644 },
		.show = sys_g_cpu_balance_affinity_n_show,
		.store = sys_g_cpu_balance_affinity_n_store
	}
};

static char thread_n_set_affinity_name[KFIPS_MAX_WORKERS][32];
static struct kobj_attribute thread_n_set_affinity_attributes[
	KFIPS_MAX_WORKERS] = {
	[0 ... KFIPS_MAX_WORKERS-1] = {
		.attr = { .mode = 0644 },
		.show = sys_g_cpu_balance_flag_n_show,
		.store = sys_g_cpu_balance_flag_n_store
	}
};

static struct attribute *attrs[8 + KFIPS_MAX_WORKERS * 3 + 1] = {
	&num_queues_active_attribute.attr,
	&thread_normal_active_attribute.attr,
	&thread_high_active_attribute.attr,
	&pending_len_attribute.attr,
	&pending_max_attribute.attr,
	&pending_low_attribute.attr,
	&pending_high_attribute.attr,
	&version_show_attribute.attr,
	[8 ... 8 + KFIPS_MAX_WORKERS * 3] NULL
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *kfips_kobj;

/*                        */
static int __init kfips_aes_mod_init(void)
{
	int rc;
	int i;
	enum kfips_load_state mstate = KFIPS_STATE_NOTLOADED;
	unsigned int cpu_id;

	for (i = 0; i < KFIPS_MAX_WORKERS; i++) {
		snprintf(thread_n_busycount_name[i],
			 sizeof(thread_n_busycount_name[i]),
			 "thread%d_busycount", i);
		thread_n_busycount_attributes[i].attr.name =
			thread_n_busycount_name[i];
		attrs[8 + i * 3] = &thread_n_busycount_attributes[i].attr;

		snprintf(thread_n_cpu_affinity_name[i],
			 sizeof(thread_n_cpu_affinity_name[i]),
			 "thread%d_cpu_affinity", i);
		thread_n_cpu_affinity_attributes[i].attr.name =
			thread_n_cpu_affinity_name[i];
		attrs[9 + i * 3] = &thread_n_cpu_affinity_attributes[i].attr;

		snprintf(thread_n_set_affinity_name[i],
			 sizeof(thread_n_set_affinity_name[i]),
			 "thread%d_set_affinity", i);
		thread_n_set_affinity_attributes[i].attr.name =
			thread_n_set_affinity_name[i];
		attrs[10 + i * 3] = &thread_n_set_affinity_attributes[i].attr;
	}
	kfips_kobj = kobject_create_and_add("kfips", kernel_kobj);
	if (!kfips_kobj)
		return -ENOMEM;

	rc = sysfs_create_group(kfips_kobj, &attr_group);
	if (rc) {
		kobject_put(kfips_kobj);
		return rc;
	}

	init_waitqueue_head(&g_cpu_wq);

	register_hotcpu_notifier(&kfips_cpu_notifier);

	get_online_cpus();
	for_each_online_cpu(cpu_id)
	{
		kfips_cpu(1, cpu_id);
	}
	put_online_cpus();

	spin_lock_init(&g_lock);
	INIT_LIST_HEAD(&g_pending);
	init_waitqueue_head(&g_file_wq);

	memset(&g_inplay, 0, sizeof(g_inplay));

	pr_debug("Loading kfips\n");

	for (i = 0; i < sizeof(algs) / sizeof(algs[0]); i++) {
		INIT_LIST_HEAD(&algs[i].cra_list);
		rc = crypto_register_alg(&algs[i]);
		if (rc != 0) {
			pr_err("Error registering %s\n", algs[i].cra_name);
			while (i > 0)
				crypto_unregister_alg(&algs[--i]);
			return rc;
		}
	}

	mstate = KFIPS_STATE_CRYPTO_REGISTERED;

#ifdef KFIPS_PROC_STATUS
#ifdef KFIPS_USE_CREATE_PROC_ENTRY
	g_proc_status_entry = create_proc_entry(KFIPS_PROC_STATUS_NAME,
						0444, NULL);
#else
	g_proc_status_entry = proc_create(KFIPS_PROC_STATUS_NAME, 0444,
					  NULL, &status_file_fops);
#endif /*                             */

	if (!g_proc_status_entry) {
		pr_err("Unable to register proc entry %s\n",
		       KFIPS_PROC_STATUS_NAME);
		kfips_aes_mod_unload(mstate);
		return -EINVAL;
	}

#ifdef KFIPS_USE_CREATE_PROC_ENTRY
	g_proc_status_entry->read_proc = kfips_proc_status_read;
#endif /*                             */
#endif

#ifdef KFIPS_USE_CREATE_PROC_ENTRY
	g_proc_entry = create_proc_entry(KFIPS_PROC_NAME,
					 S_IWUSR | S_IRUSR,
					 NULL);
#else
	g_proc_entry = proc_create(KFIPS_PROC_NAME, S_IWUSR | S_IRUSR, NULL,
				   &file_fops);
#endif /*                             */

	if (!g_proc_entry) {
		pr_err("Unable to register proc entry %s\n", KFIPS_PROC_NAME);
		kfips_aes_mod_unload(mstate);
		return -EINVAL;
	}
	if (uid >= 0) {
#ifdef KFIPS_USE_CREATE_PROC_ENTRY
		g_proc_entry->uid = uid;
#else
		proc_set_user(g_proc_entry, uid, 0);
#endif /*                             */
	}
#ifdef KFIPS_USE_CREATE_PROC_ENTRY
	g_proc_entry->proc_fops = &file_fops;
#endif /*                             */
	return 0;
}

/*                                   */
static void __exit kfips_aes_mod_exit(void)
{
	kfips_aes_mod_unload(KFIPS_STATE_LOADED);
	kobject_put(kfips_kobj);
}

module_init(kfips_aes_mod_init);
module_exit(kfips_aes_mod_exit);

MODULE_DESCRIPTION("INSIDE Secure FIPS AES-XTS/AES-CBC Driver.");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.2");
MODULE_AUTHOR("INSIDE Secure Oy");

module_param(uid, int, 0);
MODULE_PARM_DESC(
	uid,
	"User id for the userland device access (default 1000 == system)");
