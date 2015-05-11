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
 *
 */

#ifndef __MSM_FIPS_DRBG_H__
#define __MSM_FIPS_DRBG_H__

#include "ctr_drbg.h"
#include "msm_rng.h"

#define FIPS140_PRNG_OK  (0)
#define FIPS140_PRNG_ERR (-1)

typedef int (*get_entropy_callback_t)(void *ctx, void *buf);

struct fips_drbg_ctx_s {
	uint32_t magic;		/*                                       */
	get_entropy_callback_t get_entropy_callback;
	void *get_entropy_callback_ctx;
	struct ctr_drbg_ctx_s ctr_drbg_ctx;
	uint8_t fips_drbg_started;
	uint8_t prev_hw_drbg_block[Q_HW_DRBG_BLOCK_BYTES];
};

/*
                                                                     
                                                                 
                                                                   
                                                                     
                                                          
                               
 */
int fips_drbg_init(struct msm_rng_device *msm_rng_ctx);

/*                                                              */
int fips_drbg_gen(struct fips_drbg_ctx_s *ctx, void *tgt, size_t len);


/*                                  */
/*                                                               
                                                                  
                                                               
                                                                    
                                                                   
               */
void fips_drbg_final(struct fips_drbg_ctx_s *ctx);

#endif /*                     */
