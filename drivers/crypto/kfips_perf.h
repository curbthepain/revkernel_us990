/*
 *      Copyright (c) INSIDE Secure Oy 2011, 2012, 2013, 2014
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
 *      http://www.gnu.org/copyleft/gpl.html
 */
/*                                                                            */

#ifndef _CRYPTO_KFIPS_PERF_H
#define _CRYPTO_KFIPS_PERF_H

#ifdef __KERNEL__
#include "linux/threads.h"
#if defined NR_CPUS
#ifndef KFIPS_MAX_WORKERS
#define KFIPS_MAX_WORKERS NR_CPUS /*                   */
#endif /*                   */
#else
#ifndef KFIPS_MAX_WORKERS
#define KFIPS_MAX_WORKERS 1 /*              */
#endif /*                   */
#endif /*                                 */
#endif /*                    */

#define KFIPS_PROC_NAME "driver/kfips"
#define KFIPS_PROC_PATH "/proc/" KFIPS_PROC_NAME

#ifndef KFIPS_BUFFER_SIZE
#define KFIPS_BUFFER_SIZE 512 /*                                         */
#endif /*                   */

#define KFIPS_AES_MAX_KEY_SIZE 32
#define KFIPS_AES_BLOCK_SIZE 16

#define KFIPS_PENDING_LEN 128

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif /*            */

#ifdef __ANDROID__
/*                                                
                                              */
#define KFIPS_QUEUE_IOCTL(szt) _IOWR_BAD('q', 0x1, szt)
#else /*             */
#define KFIPS_QUEUE_IOCTL(szt) _IOWR('q', 0x1, szt)
#endif /*             */

#ifdef __KERNEL__
#include <crypto/aes.h>
#include <linux/device-mapper.h>
#if KFIPS_AES_MAX_KEY_SIZE != AES_MAX_KEY_SIZE
#error bad KFIPS_AES_MAX_KEY_SIZE
#endif
#if KFIPS_AES_BLOCK_SIZE != AES_BLOCK_SIZE
#error bad KFIPS_AES_BLOCK_SIZE
#endif
#endif

/*                            */
#define REQUEST_VALID 1
#define RESPONSE_VALID 2

/*                                               */
struct kfips_ctrl {
	/*                                       */
	uint8_t key[2 * KFIPS_AES_MAX_KEY_SIZE];
	/*                       */
	uint8_t iv[KFIPS_AES_BLOCK_SIZE];
	/*                */
	uint32_t len;
	/*                     */
	uint32_t keylen;
};

/*                                                    */
struct kfips_req {
	/*                          */
	uint32_t flags;         /*                */
	uint32_t context;       /*                  
                                 */
	struct kfips_ctrl ctrl; /*             */
};

#define KFIPS_FLAGS_BLANK       0x80000000U /*                      */
#define KFIPS_FLAGS_SEND        0x40000000U /*                    */
#define KFIPS_FLAGS_ERR         0x20000000U /*                           */
#define KFIPS_FLAGS_BUSY        0x10000000U /*             */
#define KFIPS_FLAGS_QUEUE_MASK  0xF0000000U /*                     
                       */

/*                                                               */
struct kfips_msg_estimate {
	/*                                        */
	uint32_t combined_valid;
	/*                                       */
	uint8_t key[2 * KFIPS_AES_MAX_KEY_SIZE];
	/*                       */
	uint8_t iv[KFIPS_AES_BLOCK_SIZE];
	/*                         */
	uint8_t buf[KFIPS_BUFFER_SIZE];
	/*                */
	int len;
	/*                 */
	uint32_t flags;
	/*                     */
	uint32_t keylen;
	/*                 */
	void *pointer;
};

/*                                                  */
#define KFIPS_FLAGS_DECRYPT 0x01
#define KFIPS_FLAGS_ENCRYPT 0x02
#define KFIPS_FLAGS_ECB 0x04
#define KFIPS_FLAGS_CBC 0x08
#define KFIPS_FLAGS_XTS 0x10

/*                                */
#ifndef KFIPS_RING_INDEX_BITS
#if KFIPS_BUFFER_SIZE <= 512
#define KFIPS_RING_INDEX_BITS 5 /*                  */
#else
#define KFIPS_RING_INDEX_BITS 2 /*                    */
#endif /*                          */
#endif /*                       */
#define KFIPS_RING_ENTRIES (1 << KFIPS_RING_INDEX_BITS)
#define KFIPS_RING_INDEX_MASK (KFIPS_RING_ENTRIES - 1)

#define KFIPS_RING_ENTRIES_SIZE_MATH \
	(16383 / sizeof(struct kfips_msg_estimate))

#define KFIPS_RING_ENTRIES_SAFE \
	((KFIPS_RING_ENTRIES < KFIPS_RING_ENTRIES_SIZE_MATH) ?	\
	 KFIPS_RING_ENTRIES :					\
	 KFIPS_RING_ENTRIES_SIZE_MATH)

#define KFIPS_DATA_SIZE (KFIPS_BUFFER_SIZE*KFIPS_RING_ENTRIES_SAFE)

/*                                                              
                 */
struct kfips_reqs_and_data {
	struct kfips_req reqs[KFIPS_RING_ENTRIES_SAFE];
	uint8_t          data[KFIPS_DATA_SIZE];
};

/*                                         */
extern int kfips_reqs_and_data_sizecheck_t[
	sizeof(struct kfips_reqs_and_data) > 16383 ? -1 : 1];

/*                                              */
#define KFIPS_KEY_ID_SIZE ((unsigned int)sizeof(uint32_t))

#endif /*                      */
