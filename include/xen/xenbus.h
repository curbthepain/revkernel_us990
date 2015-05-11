/******************************************************************************
 * xenbus.h
 *
 * Talks to Xen Store to figure out what devices we have.
 *
 * Copyright (C) 2005 Rusty Russell, IBM Corporation
 * Copyright (C) 2005 XenSource Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _XEN_XENBUS_H
#define _XEN_XENBUS_H

#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/export.h>
#include <linux/completion.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <xen/interface/xen.h>
#include <xen/interface/grant_table.h>
#include <xen/interface/io/xenbus.h>
#include <xen/interface/io/xs_wire.h>

/*                                       */
struct xenbus_watch
{
	struct list_head list;

	/*                     */
	const char *node;

	/*                                                              */
	void (*callback)(struct xenbus_watch *,
			 const char **vec, unsigned int len);
};


/*                  */
struct xenbus_device {
	const char *devicetype;
	const char *nodename;
	const char *otherend;
	int otherend_id;
	struct xenbus_watch otherend_watch;
	struct device dev;
	enum xenbus_state state;
	struct completion down;
};

static inline struct xenbus_device *to_xenbus_device(struct device *dev)
{
	return container_of(dev, struct xenbus_device, dev);
}

struct xenbus_device_id
{
	/*                                       */
	char devicetype[32]; 	/*                          */
};

/*                  */
struct xenbus_driver {
	const struct xenbus_device_id *ids;
	int (*probe)(struct xenbus_device *dev,
		     const struct xenbus_device_id *id);
	void (*otherend_changed)(struct xenbus_device *dev,
				 enum xenbus_state backend_state);
	int (*remove)(struct xenbus_device *dev);
	int (*suspend)(struct xenbus_device *dev);
	int (*resume)(struct xenbus_device *dev);
	int (*uevent)(struct xenbus_device *, struct kobj_uevent_env *);
	struct device_driver driver;
	int (*read_otherend_details)(struct xenbus_device *dev);
	int (*is_ready)(struct xenbus_device *dev);
};

#define DEFINE_XENBUS_DRIVER(var, drvname, methods...)		\
struct xenbus_driver var ## _driver = {				\
	.driver.name = drvname + 0 ?: var ## _ids->devicetype,	\
	.driver.owner = THIS_MODULE,				\
	.ids = var ## _ids, ## methods				\
}

static inline struct xenbus_driver *to_xenbus_driver(struct device_driver *drv)
{
	return container_of(drv, struct xenbus_driver, driver);
}

int __must_check xenbus_register_frontend(struct xenbus_driver *);
int __must_check xenbus_register_backend(struct xenbus_driver *);

void xenbus_unregister_driver(struct xenbus_driver *drv);

struct xenbus_transaction
{
	u32 id;
};

/*                     */
#define XBT_NIL ((struct xenbus_transaction) { 0 })

char **xenbus_directory(struct xenbus_transaction t,
			const char *dir, const char *node, unsigned int *num);
void *xenbus_read(struct xenbus_transaction t,
		  const char *dir, const char *node, unsigned int *len);
int xenbus_write(struct xenbus_transaction t,
		 const char *dir, const char *node, const char *string);
int xenbus_mkdir(struct xenbus_transaction t,
		 const char *dir, const char *node);
int xenbus_exists(struct xenbus_transaction t,
		  const char *dir, const char *node);
int xenbus_rm(struct xenbus_transaction t, const char *dir, const char *node);
int xenbus_transaction_start(struct xenbus_transaction *t);
int xenbus_transaction_end(struct xenbus_transaction t, int abort);

/*                                                              */
__scanf(4, 5)
int xenbus_scanf(struct xenbus_transaction t,
		 const char *dir, const char *node, const char *fmt, ...);

/*                                               */
__printf(4, 5)
int xenbus_printf(struct xenbus_transaction t,
		  const char *dir, const char *node, const char *fmt, ...);

/*                                                        
                                                              */
int xenbus_gather(struct xenbus_transaction t, const char *dir, ...);

/*                                                 */
extern int xenstored_ready;
int register_xenstore_notifier(struct notifier_block *nb);
void unregister_xenstore_notifier(struct notifier_block *nb);

int register_xenbus_watch(struct xenbus_watch *watch);
void unregister_xenbus_watch(struct xenbus_watch *watch);
void xs_suspend(void);
void xs_resume(void);
void xs_suspend_cancel(void);

/*                                                         */
void *xenbus_dev_request_and_reply(struct xsd_sockmsg *msg);

struct work_struct;

/*                                                                */
void xenbus_suspend(void);
void xenbus_resume(void);
void xenbus_probe(struct work_struct *);
void xenbus_suspend_cancel(void);

#define XENBUS_IS_ERR_READ(str) ({			\
	if (!IS_ERR(str) && strlen(str) == 0) {		\
		kfree(str);				\
		str = ERR_PTR(-ERANGE);			\
	}						\
	IS_ERR(str);					\
})

#define XENBUS_EXIST_ERR(err) ((err) == -ENOENT || (err) == -ERANGE)

int xenbus_watch_path(struct xenbus_device *dev, const char *path,
		      struct xenbus_watch *watch,
		      void (*callback)(struct xenbus_watch *,
				       const char **, unsigned int));
__printf(4, 5)
int xenbus_watch_pathfmt(struct xenbus_device *dev, struct xenbus_watch *watch,
			 void (*callback)(struct xenbus_watch *,
					  const char **, unsigned int),
			 const char *pathfmt, ...);

int xenbus_switch_state(struct xenbus_device *dev, enum xenbus_state new_state);
int xenbus_grant_ring(struct xenbus_device *dev, unsigned long ring_mfn);
int xenbus_map_ring_valloc(struct xenbus_device *dev,
			   int gnt_ref, void **vaddr);
int xenbus_map_ring(struct xenbus_device *dev, int gnt_ref,
			   grant_handle_t *handle, void *vaddr);

int xenbus_unmap_ring_vfree(struct xenbus_device *dev, void *vaddr);
int xenbus_unmap_ring(struct xenbus_device *dev,
		      grant_handle_t handle, void *vaddr);

int xenbus_alloc_evtchn(struct xenbus_device *dev, int *port);
int xenbus_bind_evtchn(struct xenbus_device *dev, int remote_port, int *port);
int xenbus_free_evtchn(struct xenbus_device *dev, int port);

enum xenbus_state xenbus_read_driver_state(const char *path);

__printf(3, 4)
void xenbus_dev_error(struct xenbus_device *dev, int err, const char *fmt, ...);
__printf(3, 4)
void xenbus_dev_fatal(struct xenbus_device *dev, int err, const char *fmt, ...);

const char *xenbus_strstate(enum xenbus_state state);
int xenbus_dev_is_online(struct xenbus_device *dev);
int xenbus_frontend_closed(struct xenbus_device *dev);

#endif /*               */