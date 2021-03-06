/*                                      
                                                  
   */
#ifndef __ARCH_ARM_MACH_MSM_BOARD_HALIBUT_H
#define __ARCH_ARM_MACH_MSM_BOARD_HALIBUT_H

#define MSM_PMEM_GPU0_BASE      (0x10000000 + 64*SZ_1M)
#define MSM_PMEM_GPU0_SIZE      0x800000
#define MSM_PMEM_MDP_BASE       (MSM_PMEM_GPU0_BASE + MSM_PMEM_GPU0_SIZE)
#define MSM_PMEM_MDP_SIZE       0x800000
#define MSM_PMEM_ADSP_BASE      (MSM_PMEM_MDP_BASE + MSM_PMEM_MDP_SIZE)
#define MSM_PMEM_ADSP_SIZE      0x800000
#define MSM_PMEM_GPU1_BASE      (MSM_PMEM_ADSP_BASE + MSM_PMEM_ADSP_SIZE)
#define MSM_PMEM_GPU1_SIZE      0x800000
#define MSM_FB_BASE		(MSM_PMEM_GPU1_BASE + MSM_PMEM_GPU1_SIZE)
#define MSM_FB_SIZE             0x200000
#define MSM_PMEM_CAMERA_BASE	(MSM_FB_BASE + MSM_FB_SIZE)
#define MSM_PMEM_CAMERA_SIZE	0xA00000

#endif
