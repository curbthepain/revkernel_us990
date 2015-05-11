/* Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/jiffies.h>
#include <linux/debugfs.h>
#include <linux/io.h>
#include <linux/idr.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/completion.h>

#include <mach/msm_ipc_logging.h>

#include "ipc_logging.h"

#define LOG_PAGE_DATA_SIZE	sizeof(((struct ipc_log_page *)0)->data)
#define LOG_PAGE_FLAG (1 << 31)

static LIST_HEAD(ipc_log_context_list);
static DEFINE_RWLOCK(context_list_lock_lha1);
static void *get_deserialization_func(struct ipc_log_context *ilctxt,
				      int type);

static struct ipc_log_page *get_first_page(struct ipc_log_context *ilctxt)
{
	struct ipc_log_page_header *p_pghdr;
	struct ipc_log_page *pg = NULL;

	if (!ilctxt)
		return NULL;
	p_pghdr = list_first_entry(&ilctxt->page_list,
				   struct ipc_log_page_header, list);
	pg = container_of(p_pghdr, struct ipc_log_page, hdr);
	return pg;
}

/* 
                                                                         
  
                           
                                                                    
  
                                                                                
                                                                         
 */
static int is_nd_read_empty(struct ipc_log_context *ilctxt)
{
	if (!ilctxt)
		return -EINVAL;

	return ((ilctxt->nd_read_page == ilctxt->write_page) &&
		(ilctxt->nd_read_page->hdr.nd_read_offset ==
		 ilctxt->write_page->hdr.write_offset));
}

/* 
                                                              
  
                           
                                                                    
  
                                                                   
                                
 */
static int is_read_empty(struct ipc_log_context *ilctxt)
{
	if (!ilctxt)
		return -EINVAL;

	return ((ilctxt->read_page == ilctxt->write_page) &&
		(ilctxt->read_page->hdr.read_offset ==
		 ilctxt->write_page->hdr.write_offset));
}

/* 
                                                                              
                       
  
                           
                                                              
 */
static bool is_nd_read_equal_read(struct ipc_log_context *ilctxt)
{
	uint16_t read_offset;
	uint16_t nd_read_offset;

	if (ilctxt->nd_read_page == ilctxt->read_page) {
		read_offset = ilctxt->read_page->hdr.read_offset;
		nd_read_offset = ilctxt->nd_read_page->hdr.nd_read_offset;

		if (read_offset == nd_read_offset)
			return true;
	}

	return false;
}


static struct ipc_log_page *get_next_page(struct ipc_log_context *ilctxt,
					  struct ipc_log_page *cur_pg)
{
	struct ipc_log_page_header *p_pghdr;
	struct ipc_log_page *pg = NULL;

	if (!ilctxt || !cur_pg)
		return NULL;

	if (ilctxt->last_page == cur_pg)
		return ilctxt->first_page;

	p_pghdr = list_first_entry(&cur_pg->hdr.list,
			struct ipc_log_page_header, list);
	pg = container_of(p_pghdr, struct ipc_log_page, hdr);

	return pg;
}

/* 
                                                    
  
                            
                                           
                                                                           
  
                                                                               
                                                                           
                                                                       
             
 */
static void ipc_log_read(struct ipc_log_context *ilctxt,
			 void *data, int data_size)
{
	int bytes_to_read;

	bytes_to_read = MIN(LOG_PAGE_DATA_SIZE
				- ilctxt->nd_read_page->hdr.nd_read_offset,
			      data_size);

	memcpy(data, (ilctxt->nd_read_page->data +
		ilctxt->nd_read_page->hdr.nd_read_offset), bytes_to_read);

	if (bytes_to_read != data_size) {
		/*                                          */
		ilctxt->nd_read_page->hdr.nd_read_offset = 0;
		ilctxt->nd_read_page = get_next_page(ilctxt,
			ilctxt->nd_read_page);
		BUG_ON(ilctxt->nd_read_page == NULL);

		memcpy((data + bytes_to_read),
			   (ilctxt->nd_read_page->data +
			ilctxt->nd_read_page->hdr.nd_read_offset),
			   (data_size - bytes_to_read));
		bytes_to_read = (data_size - bytes_to_read);
	}
	ilctxt->nd_read_page->hdr.nd_read_offset += bytes_to_read;
}

/* 
                                                
  
                            
                                                     
                                                                           
 */
static void ipc_log_drop(struct ipc_log_context *ilctxt, void *data,
		int data_size)
{
	int bytes_to_read;
	bool push_nd_read;

	bytes_to_read = MIN(LOG_PAGE_DATA_SIZE
				- ilctxt->read_page->hdr.read_offset,
			      data_size);
	if (data)
		memcpy(data, (ilctxt->read_page->data +
			ilctxt->read_page->hdr.read_offset), bytes_to_read);

	if (bytes_to_read != data_size) {
		/*                                          */
		push_nd_read = is_nd_read_equal_read(ilctxt);

		ilctxt->read_page->hdr.read_offset = 0;
		if (push_nd_read) {
			ilctxt->read_page->hdr.nd_read_offset = 0;
			ilctxt->read_page = get_next_page(ilctxt,
				ilctxt->read_page);
			BUG_ON(ilctxt->read_page == NULL);
			ilctxt->nd_read_page = ilctxt->read_page;
		} else {
			ilctxt->read_page = get_next_page(ilctxt,
				ilctxt->read_page);
			BUG_ON(ilctxt->read_page == NULL);
		}

		if (data)
			memcpy((data + bytes_to_read),
				   (ilctxt->read_page->data +
				ilctxt->read_page->hdr.read_offset),
				   (data_size - bytes_to_read));

		bytes_to_read = (data_size - bytes_to_read);
	}

	/*                                                  */
	push_nd_read = is_nd_read_equal_read(ilctxt);
	ilctxt->read_page->hdr.read_offset += bytes_to_read;
	ilctxt->write_avail += data_size;

	if (push_nd_read)
		ilctxt->nd_read_page->hdr.nd_read_offset += bytes_to_read;
}

/* 
                              
  
                                                              
                  
                                                    
                                        
  
                          
                           
  
                                                               
 */
static int msg_read(struct ipc_log_context *ilctxt,
	     struct encode_context *ectxt)
{
	struct tsv_header hdr;

	if (!ectxt)
		return -EINVAL;

	if (is_nd_read_empty(ilctxt))
		return 0;

	ipc_log_read(ilctxt, &hdr, sizeof(hdr));
	ectxt->hdr.type = hdr.type;
	ectxt->hdr.size = hdr.size;
	ectxt->offset = sizeof(hdr);
	ipc_log_read(ilctxt, (ectxt->buff + ectxt->offset),
			 (int)hdr.size);

	return sizeof(hdr) + (int)hdr.size;
}

/* 
                              
  
                          
 */
static void msg_drop(struct ipc_log_context *ilctxt)
{
	struct tsv_header hdr;

	if (!is_read_empty(ilctxt)) {
		ipc_log_drop(ilctxt, &hdr, sizeof(hdr));
		ipc_log_drop(ilctxt, NULL, (int)hdr.size);
	}
}

/*
                                                                  
                                                            
 */
void ipc_log_write(void *ctxt, struct encode_context *ectxt)
{
	struct ipc_log_context *ilctxt = (struct ipc_log_context *)ctxt;
	int bytes_to_write;
	unsigned long flags;

	if (!ilctxt || !ectxt) {
		pr_err("%s: Invalid ipc_log or encode context\n", __func__);
		return;
	}

	read_lock_irqsave(&context_list_lock_lha1, flags);
	spin_lock(&ilctxt->context_lock_lhb1);
	while (ilctxt->write_avail <= ectxt->offset)
		msg_drop(ilctxt);

	bytes_to_write = MIN(LOG_PAGE_DATA_SIZE
				- ilctxt->write_page->hdr.write_offset,
				ectxt->offset);
	memcpy((ilctxt->write_page->data +
		ilctxt->write_page->hdr.write_offset),
		ectxt->buff, bytes_to_write);

	if (bytes_to_write != ectxt->offset) {
		uint64_t t_now = sched_clock();

		ilctxt->write_page->hdr.write_offset += bytes_to_write;
		ilctxt->write_page->hdr.end_time = t_now;

		ilctxt->write_page = get_next_page(ilctxt, ilctxt->write_page);
		BUG_ON(ilctxt->write_page == NULL);
		ilctxt->write_page->hdr.write_offset = 0;
		ilctxt->write_page->hdr.start_time = t_now;
		memcpy((ilctxt->write_page->data +
			ilctxt->write_page->hdr.write_offset),
		       (ectxt->buff + bytes_to_write),
		       (ectxt->offset - bytes_to_write));
		bytes_to_write = (ectxt->offset - bytes_to_write);
	}
	ilctxt->write_page->hdr.write_offset += bytes_to_write;
	ilctxt->write_avail -= ectxt->offset;
	complete(&ilctxt->read_avail);
	spin_unlock(&ilctxt->context_lock_lhb1);
	read_unlock_irqrestore(&context_list_lock_lha1, flags);
}
EXPORT_SYMBOL(ipc_log_write);

/*
                                                                   
                                                         
 */
void msg_encode_start(struct encode_context *ectxt, uint32_t type)
{
	if (!ectxt) {
		pr_err("%s: Invalid encode context\n", __func__);
		return;
	}

	ectxt->hdr.type = type;
	ectxt->hdr.size = 0;
	ectxt->offset = sizeof(ectxt->hdr);
}
EXPORT_SYMBOL(msg_encode_start);

/*
                        
 */
void msg_encode_end(struct encode_context *ectxt)
{
	if (!ectxt) {
		pr_err("%s: Invalid encode context\n", __func__);
		return;
	}

	/*                    */
	ectxt->hdr.size = ectxt->offset - sizeof(ectxt->hdr);
	BUG_ON(ectxt->hdr.size > MAX_MSG_SIZE);
	memcpy(ectxt->buff, &ectxt->hdr, sizeof(ectxt->hdr));
}
EXPORT_SYMBOL(msg_encode_end);

/*
                                                          
  
                                                           
                       
                                          
 */
static inline int tsv_write_data(struct encode_context *ectxt,
				 void *data, uint32_t size)
{
	if (!ectxt) {
		pr_err("%s: Invalid encode context\n", __func__);
		return -EINVAL;
	}
	if ((ectxt->offset + size) > MAX_MSG_SIZE) {
		pr_err("%s: No space to encode further\n", __func__);
		return -EINVAL;
	}

	memcpy((void *)(ectxt->buff + ectxt->offset), data, size);
	ectxt->offset += size;
	return 0;
}

/*
                                                     
  
                                                           
                        
                                    
 */
static inline int tsv_write_header(struct encode_context *ectxt,
				   uint32_t type, uint32_t size)
{
	struct tsv_header hdr;

	hdr.type = (unsigned char)type;
	hdr.size = (unsigned char)size;
	return tsv_write_data(ectxt, &hdr, sizeof(hdr));
}

/*
                                      
  
                                                             
 */
int tsv_timestamp_write(struct encode_context *ectxt)
{
	int ret;
	uint64_t t_now = sched_clock();

	ret = tsv_write_header(ectxt, TSV_TYPE_TIMESTAMP, sizeof(t_now));
	if (ret)
		return ret;
	return tsv_write_data(ectxt, &t_now, sizeof(t_now));
}
EXPORT_SYMBOL(tsv_timestamp_write);

/*
                         
  
                                                             
                                  
 */
int tsv_pointer_write(struct encode_context *ectxt, void *pointer)
{
	int ret;
	ret = tsv_write_header(ectxt, TSV_TYPE_POINTER, sizeof(pointer));
	if (ret)
		return ret;
	return tsv_write_data(ectxt, &pointer, sizeof(pointer));
}
EXPORT_SYMBOL(tsv_pointer_write);

/*
                                 
  
                                                           
                          
 */
int tsv_int32_write(struct encode_context *ectxt, int32_t n)
{
	int ret;
	ret = tsv_write_header(ectxt, TSV_TYPE_INT32, sizeof(n));
	if (ret)
		return ret;
	return tsv_write_data(ectxt, &n, sizeof(n));
}
EXPORT_SYMBOL(tsv_int32_write);

/*
                       
  
                                                          
                                   
                                        
 */
int tsv_byte_array_write(struct encode_context *ectxt,
			 void *data, int data_size)
{
	int ret;
	ret = tsv_write_header(ectxt, TSV_TYPE_BYTE_ARRAY, data_size);
	if (ret)
		return ret;
	return tsv_write_data(ectxt, data, data_size);
}
EXPORT_SYMBOL(tsv_byte_array_write);

/*
                                  
  
                                                                 
                                              
 */
int ipc_log_string(void *ilctxt, const char *fmt, ...)
{
	struct encode_context ectxt;
	int avail_size, data_size, hdr_size = sizeof(struct tsv_header);
	va_list arg_list;

	if (!ilctxt)
		return -EINVAL;

	msg_encode_start(&ectxt, TSV_TYPE_STRING);
	tsv_timestamp_write(&ectxt);
	avail_size = (MAX_MSG_SIZE - (ectxt.offset + hdr_size));
	va_start(arg_list, fmt);
	data_size = vsnprintf((ectxt.buff + ectxt.offset + hdr_size),
			      avail_size, fmt, arg_list);
	va_end(arg_list);
	tsv_write_header(&ectxt, TSV_TYPE_BYTE_ARRAY, data_size);
	ectxt.offset += data_size;
	msg_encode_end(&ectxt);
	ipc_log_write(ilctxt, &ectxt);
	return 0;
}
EXPORT_SYMBOL(ipc_log_string);

/* 
                                               
  
                          
                                       
                               
                                                                  
  
                                                                  
                                                             
                              
 */
int ipc_log_extract(void *ctxt, char *buff, int size)
{
	struct encode_context ectxt;
	struct decode_context dctxt;
	void (*deserialize_func)(struct encode_context *ectxt,
				 struct decode_context *dctxt);
	struct ipc_log_context *ilctxt = (struct ipc_log_context *)ctxt;
	unsigned long flags;

	if (size < MAX_MSG_DECODED_SIZE)
		return -EINVAL;

	dctxt.output_format = OUTPUT_DEBUGFS;
	dctxt.buff = buff;
	dctxt.size = size;
	read_lock_irqsave(&context_list_lock_lha1, flags);
	spin_lock(&ilctxt->context_lock_lhb1);
	while (dctxt.size >= MAX_MSG_DECODED_SIZE &&
	       !is_nd_read_empty(ilctxt)) {
		msg_read(ilctxt, &ectxt);
		deserialize_func = get_deserialization_func(ilctxt,
							ectxt.hdr.type);
		spin_unlock(&ilctxt->context_lock_lhb1);
		read_unlock_irqrestore(&context_list_lock_lha1, flags);
		if (deserialize_func)
			deserialize_func(&ectxt, &dctxt);
		else
			pr_err("%s: unknown message 0x%x\n",
				__func__, ectxt.hdr.type);
		read_lock_irqsave(&context_list_lock_lha1, flags);
		spin_lock(&ilctxt->context_lock_lhb1);
	}
	if ((size - dctxt.size) == 0)
		INIT_COMPLETION(ilctxt->read_avail);
	spin_unlock(&ilctxt->context_lock_lhb1);
	read_unlock_irqrestore(&context_list_lock_lha1, flags);
	return size - dctxt.size;
}
EXPORT_SYMBOL(ipc_log_extract);

/*
                                                           
  
                                                    
                      
                                         
 */
static void tsv_read_data(struct encode_context *ectxt,
			  void *data, uint32_t size)
{
	BUG_ON((ectxt->offset + size) > MAX_MSG_SIZE);
	memcpy(data, (ectxt->buff + ectxt->offset), size);
	ectxt->offset += size;
}

/*
                                                                     
                    
  
                                                    
                     
 */
static void tsv_read_header(struct encode_context *ectxt,
			    struct tsv_header *hdr)
{
	BUG_ON((ectxt->offset + sizeof(*hdr)) > MAX_MSG_SIZE);
	memcpy(hdr, (ectxt->buff + ectxt->offset), sizeof(*hdr));
	ectxt->offset += sizeof(*hdr);
}

/*
                     
  
                                                     
                                   
                                                                
 */
void tsv_timestamp_read(struct encode_context *ectxt,
			struct decode_context *dctxt, const char *format)
{
	struct tsv_header hdr;
	uint64_t val;
	unsigned long nanosec_rem;

	tsv_read_header(ectxt, &hdr);
	BUG_ON(hdr.type != TSV_TYPE_TIMESTAMP);
	tsv_read_data(ectxt, &val, sizeof(val));
	nanosec_rem = do_div(val, 1000000000U);
	IPC_SPRINTF_DECODE(dctxt, "[%6u.%09lu]%s",
			(unsigned)val, nanosec_rem, format);
}
EXPORT_SYMBOL(tsv_timestamp_read);

/*
                        
  
                                                     
                                   
                        
 */
void tsv_pointer_read(struct encode_context *ectxt,
		      struct decode_context *dctxt, const char *format)
{
	struct tsv_header hdr;
	void *val;

	tsv_read_header(ectxt, &hdr);
	BUG_ON(hdr.type != TSV_TYPE_POINTER);
	tsv_read_data(ectxt, &val, sizeof(val));

	IPC_SPRINTF_DECODE(dctxt, format, val);
}
EXPORT_SYMBOL(tsv_pointer_read);

/*
                                
  
                                                     
                                   
                        
 */
int32_t tsv_int32_read(struct encode_context *ectxt,
		       struct decode_context *dctxt, const char *format)
{
	struct tsv_header hdr;
	int32_t val;

	tsv_read_header(ectxt, &hdr);
	BUG_ON(hdr.type != TSV_TYPE_INT32);
	tsv_read_data(ectxt, &val, sizeof(val));

	IPC_SPRINTF_DECODE(dctxt, format, val);
	return val;
}
EXPORT_SYMBOL(tsv_int32_read);

/*
                             
  
                                                     
                                   
                        
 */
void tsv_byte_array_read(struct encode_context *ectxt,
			 struct decode_context *dctxt, const char *format)
{
	struct tsv_header hdr;

	tsv_read_header(ectxt, &hdr);
	BUG_ON(hdr.type != TSV_TYPE_BYTE_ARRAY);
	tsv_read_data(ectxt, dctxt->buff, hdr.size);
	dctxt->buff += hdr.size;
	dctxt->size -= hdr.size;
}
EXPORT_SYMBOL(tsv_byte_array_read);

int add_deserialization_func(void *ctxt, int type,
			void (*dfunc)(struct encode_context *,
				      struct decode_context *))
{
	struct ipc_log_context *ilctxt = (struct ipc_log_context *)ctxt;
	struct dfunc_info *df_info;
	unsigned long flags;

	if (!ilctxt || !dfunc)
		return -EINVAL;

	df_info = kmalloc(sizeof(struct dfunc_info), GFP_KERNEL);
	if (!df_info)
		return -ENOSPC;

	read_lock_irqsave(&context_list_lock_lha1, flags);
	spin_lock(&ilctxt->context_lock_lhb1);
	df_info->type = type;
	df_info->dfunc = dfunc;
	list_add_tail(&df_info->list, &ilctxt->dfunc_info_list);
	spin_unlock(&ilctxt->context_lock_lhb1);
	read_unlock_irqrestore(&context_list_lock_lha1, flags);
	return 0;
}
EXPORT_SYMBOL(add_deserialization_func);

static void *get_deserialization_func(struct ipc_log_context *ilctxt,
				      int type)
{
	struct dfunc_info *df_info = NULL;

	if (!ilctxt)
		return NULL;

	list_for_each_entry(df_info, &ilctxt->dfunc_info_list, list) {
		if (df_info->type == type)
			return df_info->dfunc;
	}
	return NULL;
}

/* 
                                                     
                                                                   
  
                                                                      
                                                            
                                                                 
  
                                                 
 */
void *ipc_log_context_create(int max_num_pages,
			     const char *mod_name, uint16_t user_version)
{
	struct ipc_log_context *ctxt;
	struct ipc_log_page *pg = NULL;
	int page_cnt;
	unsigned long flags;

	ctxt = kzalloc(sizeof(struct ipc_log_context), GFP_KERNEL);
	if (!ctxt) {
		pr_err("%s: cannot create ipc_log_context\n", __func__);
		return 0;
	}

	init_completion(&ctxt->read_avail);
	INIT_LIST_HEAD(&ctxt->page_list);
	INIT_LIST_HEAD(&ctxt->dfunc_info_list);
	spin_lock_init(&ctxt->context_lock_lhb1);
	for (page_cnt = 0; page_cnt < max_num_pages; page_cnt++) {
		pg = kzalloc(sizeof(struct ipc_log_page), GFP_KERNEL);
		if (!pg) {
			pr_err("%s: cannot create ipc_log_page\n", __func__);
			goto release_ipc_log_context;
		}
		pg->hdr.log_id = (uint64_t)(uintptr_t)ctxt;
		pg->hdr.page_num = LOG_PAGE_FLAG | page_cnt;
		pg->hdr.ctx_offset = (int64_t)((uint64_t)(uintptr_t)ctxt -
			(uint64_t)(uintptr_t)&pg->hdr);

		/*                                                     */
		pg->hdr.magic = IPC_LOGGING_MAGIC_NUM;
		pg->hdr.nmagic = ~(IPC_LOGGING_MAGIC_NUM);

		spin_lock_irqsave(&ctxt->context_lock_lhb1, flags);
		list_add_tail(&pg->hdr.list, &ctxt->page_list);
		spin_unlock_irqrestore(&ctxt->context_lock_lhb1, flags);
	}

	ctxt->log_id = (uint64_t)(uintptr_t)ctxt;
	ctxt->version = IPC_LOG_VERSION;
	strlcpy(ctxt->name, mod_name, IPC_LOG_MAX_CONTEXT_NAME_LEN);
	ctxt->user_version = user_version;
	ctxt->first_page = get_first_page(ctxt);
	ctxt->last_page = pg;
	ctxt->write_page = ctxt->first_page;
	ctxt->read_page = ctxt->first_page;
	ctxt->nd_read_page = ctxt->first_page;
	ctxt->write_avail = max_num_pages * LOG_PAGE_DATA_SIZE;
	ctxt->header_size = sizeof(struct ipc_log_page_header);
	create_ctx_debugfs(ctxt, mod_name);

	/*                                                   */
	ctxt->magic = IPC_LOG_CONTEXT_MAGIC_NUM;
	ctxt->nmagic = ~(IPC_LOG_CONTEXT_MAGIC_NUM);

	write_lock_irqsave(&context_list_lock_lha1, flags);
	list_add_tail(&ctxt->list, &ipc_log_context_list);
	write_unlock_irqrestore(&context_list_lock_lha1, flags);
	return (void *)ctxt;

release_ipc_log_context:
	while (page_cnt-- > 0) {
		pg = get_first_page(ctxt);
		list_del(&pg->hdr.list);
		kfree(pg);
	}
	kfree(ctxt);
	return 0;
}
EXPORT_SYMBOL(ipc_log_context_create);

/*
                            
  
                                                                          
 */
int ipc_log_context_destroy(void *ctxt)
{
	struct ipc_log_context *ilctxt = (struct ipc_log_context *)ctxt;
	struct ipc_log_page *pg = NULL;
	unsigned long flags;

	if (!ilctxt)
		return 0;

	while (!list_empty(&ilctxt->page_list)) {
		pg = get_first_page(ctxt);
		list_del(&pg->hdr.list);
		kfree(pg);
	}

	write_lock_irqsave(&context_list_lock_lha1, flags);
	list_del(&ilctxt->list);
	write_unlock_irqrestore(&context_list_lock_lha1, flags);

	kfree(ilctxt);
	return 0;
}
EXPORT_SYMBOL(ipc_log_context_destroy);

static int __init ipc_logging_init(void)
{
	check_and_create_debugfs();
	return 0;
}

module_init(ipc_logging_init);

MODULE_DESCRIPTION("ipc logging");
MODULE_LICENSE("GPL v2");
