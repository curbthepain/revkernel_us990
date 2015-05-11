/*
 *  linux/arch/arm/mm/mmu.c
 *
 *  Copyright (C) 1995-2005 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/mman.h>
#include <linux/nodemask.h>
#include <linux/memblock.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

#include <asm/cp15.h>
#include <asm/cputype.h>
#include <asm/sections.h>
#include <asm/cachetype.h>
#include <asm/setup.h>
#include <asm/sizes.h>
#include <asm/smp_plat.h>
#include <asm/tlb.h>
#include <asm/highmem.h>
#include <asm/system_info.h>
#include <asm/traps.h>
#include <asm/mmu_writeable.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/user_accessible_timer.h>

#include "mm.h"

/*
                                                     
                                 
 */
struct page *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);

/*
                                                 
 */
pmd_t *top_pmd;

#define CPOLICY_UNCACHED	0
#define CPOLICY_BUFFERED	1
#define CPOLICY_WRITETHROUGH	2
#define CPOLICY_WRITEBACK	3
#define CPOLICY_WRITEALLOC	4

#define RX_AREA_START           _text
#define RX_AREA_END             __start_rodata

static unsigned int cachepolicy __initdata = CPOLICY_WRITEBACK;
static unsigned int ecc_mask __initdata = 0;
pgprot_t pgprot_user;
pgprot_t pgprot_kernel;

EXPORT_SYMBOL(pgprot_user);
EXPORT_SYMBOL(pgprot_kernel);

struct cachepolicy {
	const char	policy[16];
	unsigned int	cr_mask;
	pmdval_t	pmd;
	pteval_t	pte;
};

static struct cachepolicy cache_policies[] __initdata = {
	{
		.policy		= "uncached",
		.cr_mask	= CR_W|CR_C,
		.pmd		= PMD_SECT_UNCACHED,
		.pte		= L_PTE_MT_UNCACHED,
	}, {
		.policy		= "buffered",
		.cr_mask	= CR_C,
		.pmd		= PMD_SECT_BUFFERED,
		.pte		= L_PTE_MT_BUFFERABLE,
	}, {
		.policy		= "writethrough",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WT,
		.pte		= L_PTE_MT_WRITETHROUGH,
	}, {
		.policy		= "writeback",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WB,
		.pte		= L_PTE_MT_WRITEBACK,
	}, {
		.policy		= "writealloc",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WBWA,
		.pte		= L_PTE_MT_WRITEALLOC,
	}
};

/*
                                                   
                                                  
                                                  
                                              
 */
static int __init early_cachepolicy(char *p)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cache_policies); i++) {
		int len = strlen(cache_policies[i].policy);

		if (memcmp(p, cache_policies[i].policy, len) == 0) {
			cachepolicy = i;
			cr_alignment &= ~cache_policies[i].cr_mask;
			cr_no_alignment &= ~cache_policies[i].cr_mask;
			break;
		}
	}
	if (i == ARRAY_SIZE(cache_policies))
		printk(KERN_ERR "ERROR: unknown or unsupported cache policy\n");
	/*
                                                                
                                                                   
                                                                    
                                                                   
                
  */
	if (cpu_architecture() >= CPU_ARCH_ARMv6) {
		printk(KERN_WARNING "Only cachepolicy=writeback supported on ARMv6 and later\n");
		cachepolicy = CPOLICY_WRITEBACK;
	}
	flush_cache_all();
	set_cr(cr_alignment);
	return 0;
}
early_param("cachepolicy", early_cachepolicy);

static int __init early_nocache(char *__unused)
{
	char *p = "buffered";
	printk(KERN_WARNING "nocache is deprecated; use cachepolicy=%s\n", p);
	early_cachepolicy(p);
	return 0;
}
early_param("nocache", early_nocache);

static int __init early_nowrite(char *__unused)
{
	char *p = "uncached";
	printk(KERN_WARNING "nowb is deprecated; use cachepolicy=%s\n", p);
	early_cachepolicy(p);
	return 0;
}
early_param("nowb", early_nowrite);

#ifndef CONFIG_ARM_LPAE
static int __init early_ecc(char *p)
{
	if (memcmp(p, "on", 2) == 0)
		ecc_mask = PMD_PROTECTION;
	else if (memcmp(p, "off", 3) == 0)
		ecc_mask = 0;
	return 0;
}
early_param("ecc", early_ecc);
#endif

static int __init noalign_setup(char *__unused)
{
	cr_alignment &= ~CR_A;
	cr_no_alignment &= ~CR_A;
	set_cr(cr_alignment);
	return 1;
}
__setup("noalign", noalign_setup);

#ifndef CONFIG_SMP
void adjust_cr(unsigned long mask, unsigned long set)
{
	unsigned long flags;

	mask &= ~CR_A;

	set &= mask;

	local_irq_save(flags);

	cr_no_alignment = (cr_no_alignment & ~mask) | set;
	cr_alignment = (cr_alignment & ~mask) | set;

	set_cr((get_cr() & ~mask) | set);

	local_irq_restore(flags);
}
#endif

#define PROT_PTE_DEVICE		L_PTE_PRESENT|L_PTE_YOUNG|L_PTE_DIRTY|L_PTE_XN
#define PROT_SECT_DEVICE	PMD_TYPE_SECT|PMD_SECT_AP_WRITE

static struct mem_type mem_types[] = {
	[MT_DEVICE] = {		  /*                                        */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_SHARED |
				  L_PTE_SHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_S,
		.domain		= DOMAIN_IO,
	},
	[MT_DEVICE_NONSHARED] = { /*                         */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_NONSHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
	[MT_DEVICE_CACHED] = {	  /*                */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_CACHED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_WB,
		.domain		= DOMAIN_IO,
	},	
	[MT_DEVICE_WC] = {	/*            */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_WC,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
	[MT_UNCACHED] = {
		.prot_pte	= PROT_PTE_DEVICE,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PMD_TYPE_SECT | PMD_SECT_XN,
		.domain		= DOMAIN_IO,
	},
	[MT_CACHECLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
#ifndef CONFIG_ARM_LPAE
	[MT_MINICLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN | PMD_SECT_MINICACHE,
		.domain    = DOMAIN_KERNEL,
	},
#endif
	[MT_LOW_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_USER,
	},
	[MT_HIGH_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_USER | L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_USER,
	},
	[MT_MEMORY] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_R] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_RW] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_RX] = {
		.prot_sect = PMD_TYPE_SECT,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_ROM] = {
		.prot_sect = PMD_TYPE_SECT,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_NONCACHED] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_MT_BUFFERABLE,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_DTCM] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_XN,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_ITCM] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_SO] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_MT_UNCACHED,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_S |
				PMD_SECT_UNCACHED | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_DMA_READY] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_XN,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_DEVICE_USER_ACCESSIBLE] = {
		.prot_pte  = PROT_PTE_DEVICE | L_PTE_MT_DEV_SHARED |
				L_PTE_SHARED | L_PTE_USER | L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PROT_SECT_DEVICE | PMD_SECT_S,
		.domain    = DOMAIN_IO,
	},
};

const struct mem_type *get_mem_type(unsigned int type)
{
	return type < ARRAY_SIZE(mem_types) ? &mem_types[type] : NULL;
}
EXPORT_SYMBOL(get_mem_type);

#define PTE_SET_FN(_name, pteop) \
static int pte_set_##_name(pte_t *ptep, pgtable_t token, unsigned long addr, \
			void *data) \
{ \
	pte_t pte = pteop(*ptep); \
\
	set_pte_ext(ptep, pte, 0); \
	return 0; \
} \

#define SET_MEMORY_FN(_name, callback) \
int set_memory_##_name(unsigned long addr, int numpages) \
{ \
	unsigned long start = addr; \
	unsigned long size = PAGE_SIZE*numpages; \
	unsigned end = start + size; \
\
	if (!IS_ENABLED(CONFIG_FORCE_PAGES)) { \
		if (start < MODULES_VADDR || start >= MODULES_END) \
			return -EINVAL;\
\
		if (end < MODULES_VADDR || end >= MODULES_END) \
			return -EINVAL; \
	} \
\
	apply_to_page_range(&init_mm, start, size, callback, NULL); \
	flush_tlb_kernel_range(start, end); \
	return 0;\
}

PTE_SET_FN(ro, pte_wrprotect)
PTE_SET_FN(rw, pte_mkwrite)
PTE_SET_FN(x, pte_mkexec)
PTE_SET_FN(nx, pte_mknexec)

SET_MEMORY_FN(ro, pte_set_ro)
EXPORT_SYMBOL(set_memory_ro);
SET_MEMORY_FN(rw, pte_set_rw)
EXPORT_SYMBOL(set_memory_rw);
SET_MEMORY_FN(x, pte_set_x)
EXPORT_SYMBOL(set_memory_x);
SET_MEMORY_FN(nx, pte_set_nx)
EXPORT_SYMBOL(set_memory_nx);

/*
                                                              
 */
static void __init build_mem_type_table(void)
{
	struct cachepolicy *cp;
	unsigned int cr = get_cr();
	pteval_t user_pgprot, kern_pgprot, vecs_pgprot;
	int cpu_arch = cpu_architecture();
	int i;

	if (cpu_arch < CPU_ARCH_ARMv6) {
#if defined(CONFIG_CPU_DCACHE_DISABLE)
		if (cachepolicy > CPOLICY_BUFFERED)
			cachepolicy = CPOLICY_BUFFERED;
#elif defined(CONFIG_CPU_DCACHE_WRITETHROUGH)
		if (cachepolicy > CPOLICY_WRITETHROUGH)
			cachepolicy = CPOLICY_WRITETHROUGH;
#endif
	}
	if (cpu_arch < CPU_ARCH_ARMv5) {
		if (cachepolicy >= CPOLICY_WRITEALLOC)
			cachepolicy = CPOLICY_WRITEBACK;
		ecc_mask = 0;
	}
	if (is_smp())
		cachepolicy = CPOLICY_WRITEALLOC;

	/*
                                                            
                                                                
                                                             
  */
	if (cpu_arch < CPU_ARCH_ARMv5)
		for (i = 0; i < ARRAY_SIZE(mem_types); i++)
			mem_types[i].prot_sect &= ~PMD_SECT_TEX(7);
	if ((cpu_arch < CPU_ARCH_ARMv6 || !(cr & CR_XP)) && !cpu_is_xsc3())
		for (i = 0; i < ARRAY_SIZE(mem_types); i++)
			mem_types[i].prot_sect &= ~PMD_SECT_S;

	/*
                                                                  
                                                               
                                           
  */
	if (cpu_is_xscale() || cpu_is_xsc3()) {
		for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
			mem_types[i].prot_sect &= ~PMD_BIT4;
			mem_types[i].prot_l1 &= ~PMD_BIT4;
		}
	} else if (cpu_arch < CPU_ARCH_ARMv6) {
		for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
			if (mem_types[i].prot_l1)
				mem_types[i].prot_l1 |= PMD_BIT4;
			if (mem_types[i].prot_sect)
				mem_types[i].prot_sect |= PMD_BIT4;
		}
	}

	/*
                                                            
  */
	if (cpu_is_xsc3() || (cpu_arch >= CPU_ARCH_ARMv6 && (cr & CR_XP))) {
		if (!cpu_is_xsc3()) {
			/*
                                                    
                                                 
    */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_CACHED].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_XN;
		}
		if (cpu_arch >= CPU_ARCH_ARMv7 && (cr & CR_TRE)) {
			/*
                                   
                                  
                                     
                                             
                              
    */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_TEX(1);
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(1);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_BUFFERABLE;
		} else if (cpu_is_xsc3()) {
			/*
                  
                                    
                                       
                                               
                                                
    */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_TEX(1) | PMD_SECT_BUFFERED;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(2);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_TEX(1);
		} else {
			/*
                                                
                                    
                                       
                                               
                                          
    */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_BUFFERED;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(2);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_TEX(1);
		}
	} else {
		/*
                                                      
   */
		mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_BUFFERABLE;
	}

	/*
                                          
  */
	cp = &cache_policies[cachepolicy];
	vecs_pgprot = kern_pgprot = user_pgprot = cp->pte;

	/*
                                              
  */
	if (!is_smp() && cpu_arch >= CPU_ARCH_ARMv5 && cachepolicy > CPOLICY_WRITETHROUGH)
		vecs_pgprot = cache_policies[CPOLICY_WRITETHROUGH].pte;

	/*
                                               
                                           
  */
	if (arch_is_coherent() && cpu_is_xsc3()) {
		mem_types[MT_MEMORY].prot_sect |= PMD_SECT_S;
		mem_types[MT_MEMORY].prot_pte |= L_PTE_SHARED;
		mem_types[MT_MEMORY_DMA_READY].prot_pte |= L_PTE_SHARED;
		mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_S;
		mem_types[MT_MEMORY_NONCACHED].prot_pte |= L_PTE_SHARED;
	}
	/*
                                              
  */
	if (cpu_arch >= CPU_ARCH_ARMv6 && (cr & CR_XP)) {
#ifndef CONFIG_ARM_LPAE
		/*
                                                 
                                                
   */
		mem_types[MT_ROM].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MEMORY_RX].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MEMORY_R].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MINICLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
#endif

		if (is_smp()) {
			/*
                                             
                     
    */
			user_pgprot |= L_PTE_SHARED;
			kern_pgprot |= L_PTE_SHARED;
			vecs_pgprot |= L_PTE_SHARED;
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_S;
			mem_types[MT_DEVICE_WC].prot_pte |= L_PTE_SHARED;
			mem_types[MT_DEVICE_CACHED].prot_sect |= PMD_SECT_S;
			mem_types[MT_DEVICE_CACHED].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_DMA_READY].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_R].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RW].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RX].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_NONCACHED].prot_pte |= L_PTE_SHARED;
		}
	}

	/*
                                                              
                                                   
  */
	if (cpu_arch >= CPU_ARCH_ARMv6) {
		if (cpu_arch >= CPU_ARCH_ARMv7 && (cr & CR_TRE)) {
			/*                                   */
			mem_types[MT_MEMORY_NONCACHED].prot_sect |=
				PMD_SECT_BUFFERED;
		} else {
			/*                                            */
			mem_types[MT_MEMORY_NONCACHED].prot_sect |=
				PMD_SECT_TEX(1);
		}
	} else {
		mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_BUFFERABLE;
	}

#ifdef CONFIG_ARM_LPAE
	/*
                                                               
  */
	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		mem_types[i].prot_pte |= PTE_EXT_AF;
		if (mem_types[i].prot_sect)
			mem_types[i].prot_sect |= PMD_SECT_AF;
	}
	kern_pgprot |= PTE_EXT_AF;
	vecs_pgprot |= PTE_EXT_AF;
#endif

	for (i = 0; i < 16; i++) {
		unsigned long v = pgprot_val(protection_map[i]);
		protection_map[i] = __pgprot(v | user_pgprot);
	}

	mem_types[MT_LOW_VECTORS].prot_pte |= vecs_pgprot;
	mem_types[MT_HIGH_VECTORS].prot_pte |= vecs_pgprot;

	pgprot_user   = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG | user_pgprot);
	pgprot_kernel = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG |
				 L_PTE_DIRTY | kern_pgprot);

	mem_types[MT_LOW_VECTORS].prot_l1 |= ecc_mask;
	mem_types[MT_HIGH_VECTORS].prot_l1 |= ecc_mask;
	mem_types[MT_MEMORY].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_DMA_READY].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_NONCACHED].prot_sect |= ecc_mask;
	mem_types[MT_MEMORY_R].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RW].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RX].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_ROM].prot_sect |= cp->pmd;

	switch (cp->pmd) {
	case PMD_SECT_WT:
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_WT;
		break;
	case PMD_SECT_WB:
	case PMD_SECT_WBWA:
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_WB;
		break;
	}
	printk("Memory policy: ECC %sabled, Data cache %s\n",
		ecc_mask ? "en" : "dis", cp->policy);

	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		struct mem_type *t = &mem_types[i];
		if (t->prot_l1)
			t->prot_l1 |= PMD_DOMAIN(t->domain);
		if (t->prot_sect)
			t->prot_sect |= PMD_DOMAIN(t->domain);
	}
}

#ifdef CONFIG_ARM_DMA_MEM_BUFFERABLE
pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
			      unsigned long size, pgprot_t vma_prot)
{
	if (!pfn_valid(pfn))
		return pgprot_noncached(vma_prot);
	else if (file->f_flags & O_SYNC)
		return pgprot_writecombine(vma_prot);
	return vma_prot;
}
EXPORT_SYMBOL(phys_mem_access_prot);
#endif

#define vectors_base()	(vectors_high() ? 0xffff0000 : 0)

static void __init *early_alloc_aligned(unsigned long sz, unsigned long align)
{
	void *ptr = __va(memblock_alloc(sz, align));
	memset(ptr, 0, sz);
	return ptr;
}

static void __init *early_alloc(unsigned long sz)
{
	return early_alloc_aligned(sz, sz);
}

static pte_t * __init early_pte_alloc(pmd_t *pmd, unsigned long addr, unsigned long prot)
{
	if (pmd_none(*pmd)) {
		pte_t *pte = early_alloc(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE);
		__pmd_populate(pmd, __pa(pte), prot);
	}
	BUG_ON(pmd_bad(*pmd));
	return pte_offset_kernel(pmd, addr);
}

static void __init alloc_init_pte(pmd_t *pmd, unsigned long addr,
				  unsigned long end, unsigned long pfn,
				  const struct mem_type *type)
{
	pte_t *pte = early_pte_alloc(pmd, addr, type->prot_l1);
	do {
		set_pte_ext(pte, pfn_pte(pfn, __pgprot(type->prot_pte)), 0);
		pfn++;
	} while (pte++, addr += PAGE_SIZE, addr != end);
}

static void __init map_init_section(pmd_t *pmd, unsigned long addr,
			unsigned long end, phys_addr_t phys,
			const struct mem_type *type)
{
#ifndef CONFIG_ARM_LPAE
	/*
                                                         
                                                             
                                                        
                                                            
                                                        
                                
                                               
  */
	if (addr & SECTION_SIZE)
		pmd++;
#endif
	do {
		*pmd = __pmd(phys | type->prot_sect);
		phys += SECTION_SIZE;
	} while (pmd++, addr += SECTION_SIZE, addr != end);

	flush_pmd_entry(pmd);
}

static void __init alloc_init_pmd(pud_t *pud, unsigned long addr,
				      unsigned long end, phys_addr_t phys,
				      const struct mem_type *type)
{
	pmd_t *pmd = pmd_offset(pud, addr);
	unsigned long next;

	do {
		/*
                                        
                                      
   */
		next = pmd_addr_end(addr, end);

		/*
                                                            
                                   
   */
		if (type->prot_sect &&
				((addr | next | phys) & ~SECTION_MASK) == 0) {
			map_init_section(pmd, addr, next, phys, type);
		} else {
			alloc_init_pte(pmd, addr, next,
						__phys_to_pfn(phys), type);
		}

		phys += next - addr;

	} while (pmd++, addr = next, addr != end);
}

static void __init alloc_init_pud(pgd_t *pgd, unsigned long addr,
	unsigned long end, unsigned long phys, const struct mem_type *type)
{
	pud_t *pud = pud_offset(pgd, addr);
	unsigned long next;

	do {
		next = pud_addr_end(addr, end);
		alloc_init_pmd(pud, addr, next, phys, type);
		phys += next - addr;
	} while (pud++, addr = next, addr != end);
}

#ifndef CONFIG_ARM_LPAE
static void __init create_36bit_mapping(struct map_desc *md,
					const struct mem_type *type)
{
	unsigned long addr, length, end;
	phys_addr_t phys;
	pgd_t *pgd;

	addr = md->virtual;
	phys = __pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length);

	if (!(cpu_architecture() >= CPU_ARCH_ARMv6 || cpu_is_xsc3())) {
		printk(KERN_ERR "MM: CPU does not support supersection "
		       "mapping for 0x%08llx at 0x%08lx\n",
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	/*                                                                 
                                                          
                                                              
                                                            
                                            
  */
	if (type->domain) {
		printk(KERN_ERR "MM: invalid domain in supersection "
		       "mapping for 0x%08llx at 0x%08lx\n",
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	if ((addr | length | __pfn_to_phys(md->pfn)) & ~SUPERSECTION_MASK) {
		printk(KERN_ERR "MM: cannot create mapping for 0x%08llx"
		       " at 0x%08lx invalid alignment\n",
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	/*
                                                          
                     
  */
	phys |= (((md->pfn >> (32 - PAGE_SHIFT)) & 0xF) << 20);

	pgd = pgd_offset_k(addr);
	end = addr + length;
	do {
		pud_t *pud = pud_offset(pgd, addr);
		pmd_t *pmd = pmd_offset(pud, addr);
		int i;

		for (i = 0; i < 16; i++)
			*pmd++ = __pmd(phys | type->prot_sect | PMD_SECT_SUPER);

		addr += SUPERSECTION_SIZE;
		phys += SUPERSECTION_SIZE;
		pgd += SUPERSECTION_SIZE >> PGDIR_SHIFT;
	} while (addr != end);
}
#endif	/*                  */

/*
                                                      
                                                     
                                                       
                                                      
                 
 */
static void __init create_mapping(struct map_desc *md)
{
	unsigned long addr, length, end;
	phys_addr_t phys;
	const struct mem_type *type;
	pgd_t *pgd;

	if ((md->virtual != vectors_base() &&
		md->virtual != get_user_accessible_timers_base()) &&
			md->virtual < TASK_SIZE) {
		printk(KERN_WARNING "BUG: not creating mapping for 0x%08llx"
		       " at 0x%08lx in user region\n",
		       (long long)__pfn_to_phys((u64)md->pfn), md->virtual);
		return;
	}

	if ((md->type == MT_DEVICE || md->type == MT_ROM) &&
	    md->virtual >= PAGE_OFFSET &&
	    (md->virtual < VMALLOC_START || md->virtual >= VMALLOC_END)) {
		printk(KERN_WARNING "BUG: mapping for 0x%08llx"
		       " at 0x%08lx out of vmalloc space\n",
		       (long long)__pfn_to_phys((u64)md->pfn), md->virtual);
	}

	type = &mem_types[md->type];

#ifndef CONFIG_ARM_LPAE
	/*
                          
  */
	if (md->pfn >= 0x100000) {
		create_36bit_mapping(md, type);
		return;
	}
#endif

	addr = md->virtual & PAGE_MASK;
	phys = __pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));

	if (type->prot_l1 == 0 && ((addr | phys | length) & ~SECTION_MASK)) {
		printk(KERN_WARNING "BUG: map for 0x%08llx at 0x%08lx can not "
		       "be mapped using pages, ignoring.\n",
		       (long long)__pfn_to_phys(md->pfn), addr);
		return;
	}

	pgd = pgd_offset_k(addr);
	end = addr + length;
	do {
		unsigned long next = pgd_addr_end(addr, end);

		alloc_init_pud(pgd, addr, next, phys, type);

		phys += next - addr;
		addr = next;
	} while (pgd++, addr != end);
}

/*
                                            
 */
void __init iotable_init(struct map_desc *io_desc, int nr)
{
	struct map_desc *md;
	struct vm_struct *vm;
	int rc = 0;

	if (!nr)
		return;

	vm = early_alloc_aligned(sizeof(*vm) * nr, __alignof__(*vm));

	for (md = io_desc; nr; md++, nr--) {
		create_mapping(md);
		vm->addr = (void *)(md->virtual & PAGE_MASK);
		vm->size = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));
		vm->phys_addr = __pfn_to_phys(md->pfn);
		vm->flags = VM_IOREMAP | VM_ARM_STATIC_MAPPING;
		vm->flags |= VM_ARM_MTYPE(md->type);
		vm->caller = iotable_init;
		rc = vm_area_check_early(vm);
		if (!rc)
			vm_area_add_early(vm++);
	}
}

#ifndef CONFIG_ARM_LPAE

/*
                                                                        
                                                                       
                                                                    
                                                                     
                                                                        
                                                                       
                                                            
  
                                                                          
                                                    
 */

static void __init pmd_empty_section_gap(unsigned long addr)
{
	struct vm_struct *vm;

	vm = early_alloc_aligned(sizeof(*vm), __alignof__(*vm));
	vm->addr = (void *)addr;
	vm->size = SECTION_SIZE;
	vm->flags = VM_IOREMAP | VM_ARM_EMPTY_MAPPING;
	vm->caller = pmd_empty_section_gap;
	vm_area_add_early(vm);
}

static void __init fill_pmd_gaps(void)
{
	struct vm_struct *vm;
	unsigned long addr, next = 0;
	pmd_t *pmd;

	/*                                                       */
	for (vm = vmlist; vm; vm = vm->next) {
		if (!(vm->flags & (VM_ARM_STATIC_MAPPING | VM_ARM_EMPTY_MAPPING)))
			continue;
		addr = (unsigned long)vm->addr;
		if (addr < next)
			continue;

		/*
                                                        
                                                           
                                                     
   */
		if ((addr & ~PMD_MASK) == SECTION_SIZE) {
			pmd = pmd_off_k(addr);
			if (pmd_none(*pmd))
				pmd_empty_section_gap(addr & PMD_MASK);
		}

		/*
                                                           
                                                             
                                                     
   */
		addr += vm->size;
		if ((addr & ~PMD_MASK) == SECTION_SIZE) {
			pmd = pmd_off_k(addr) + 1;
			if (pmd_none(*pmd))
				pmd_empty_section_gap(addr);
		}

		/*                                                           */
		next = (addr + PMD_SIZE - 1) & PMD_MASK;
	}
}

#else
#define fill_pmd_gaps() do { } while (0)
#endif

static void * __initdata vmalloc_min =
	(void *)(VMALLOC_END - (240 << 20) - VMALLOC_OFFSET);

/*
                                                            
                                                                
                              
 */
static int __init early_vmalloc(char *arg)
{
	unsigned long vmalloc_reserve = memparse(arg, NULL);

	if (vmalloc_reserve < SZ_16M) {
		vmalloc_reserve = SZ_16M;
		printk(KERN_WARNING
			"vmalloc area too small, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}

	if (vmalloc_reserve > VMALLOC_END - (PAGE_OFFSET + SZ_32M)) {
		vmalloc_reserve = VMALLOC_END - (PAGE_OFFSET + SZ_32M);
		printk(KERN_WARNING
			"vmalloc area is too big, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}

	vmalloc_min = (void *)(VMALLOC_END - vmalloc_reserve);
	return 0;
}
early_param("vmalloc", early_vmalloc);

phys_addr_t arm_lowmem_limit __initdata = 0;

void __init sanity_check_meminfo(void)
{
	int i, j, highmem = 0;

#ifdef CONFIG_ENABLE_VMALLOC_SAVING
	unsigned long hole_start;
	for (i = 0; i < (meminfo.nr_banks - 1); i++) {
		hole_start = meminfo.bank[i].start + meminfo.bank[i].size;
		if (hole_start != meminfo.bank[i+1].start) {
			if (hole_start <= MAX_HOLE_ADDRESS) {
				vmalloc_min = (void *) (vmalloc_min +
				(meminfo.bank[i+1].start - hole_start));
			}
		}
	}
#endif
#ifdef CONFIG_DONT_MAP_HOLE_AFTER_MEMBANK0
	find_memory_hole();
#endif

	for (i = 0, j = 0; i < meminfo.nr_banks; i++) {
		struct membank *bank = &meminfo.bank[j];
		*bank = meminfo.bank[i];

		if (bank->start > ULONG_MAX)
			highmem = 1;

#ifdef CONFIG_HIGHMEM
		if (__va(bank->start) >= vmalloc_min ||
		    __va(bank->start) < (void *)PAGE_OFFSET)
			highmem = 1;

		bank->highmem = highmem;

		/*
                                                             
                                                       
   */
		if (!highmem && __va(bank->start) < vmalloc_min &&
		    bank->size > vmalloc_min - __va(bank->start)) {
			if (meminfo.nr_banks >= NR_BANKS) {
				printk(KERN_CRIT "NR_BANKS too low, "
						 "ignoring high memory\n");
			} else {
				memmove(bank + 1, bank,
					(meminfo.nr_banks - i) * sizeof(*bank));
				meminfo.nr_banks++;
				i++;
				bank[1].size -= vmalloc_min - __va(bank->start);
				bank[1].start = __pa(vmalloc_min - 1) + 1;
				bank[1].highmem = highmem = 1;
				j++;
			}
			bank->size = vmalloc_min - __va(bank->start);
		}
#else
		bank->highmem = highmem;

		/*
                                                    
   */
		if (highmem) {
			printk(KERN_NOTICE "Ignoring RAM at %.8llx-%.8llx "
			       "(!CONFIG_HIGHMEM).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1);
			continue;
		}

		/*
                                                          
                      
   */
		if (__va(bank->start) >= vmalloc_min ||
		    __va(bank->start) < (void *)PAGE_OFFSET) {
			printk(KERN_NOTICE "Ignoring RAM at %.8llx-%.8llx "
			       "(vmalloc region overlap).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1);
			continue;
		}

		/*
                                                           
                      
   */
		if (__va(bank->start + bank->size) > vmalloc_min ||
		    __va(bank->start + bank->size) < __va(bank->start)) {
			unsigned long newsize = vmalloc_min - __va(bank->start);
			printk(KERN_NOTICE "Truncating RAM at %.8llx-%.8llx "
			       "to -%.8llx (vmalloc region overlap).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1,
			       (unsigned long long)bank->start + newsize - 1);
			bank->size = newsize;
		}
#endif
		if (!bank->highmem && bank->start + bank->size > arm_lowmem_limit)
			arm_lowmem_limit = bank->start + bank->size;

		j++;
	}
#ifdef CONFIG_HIGHMEM
	if (highmem) {
		const char *reason = NULL;

		if (cache_is_vipt_aliasing()) {
			/*
                                                  
                                                    
                       
    */
			reason = "with VIPT aliasing cache";
		}
		if (reason) {
			printk(KERN_CRIT "HIGHMEM is not supported %s, ignoring high memory\n",
				reason);
			while (j > 0 && meminfo.bank[j - 1].highmem)
				j--;
		}
	}
#endif
	meminfo.nr_banks = j;
	high_memory = __va(arm_lowmem_limit - 1) + 1;
	memblock_set_current_limit(arm_lowmem_limit);
}

static inline void prepare_page_table(void)
{
	unsigned long addr;
	phys_addr_t end;

	/*
                                                      
  */
	for (addr = 0; addr < MODULES_VADDR; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

#ifdef CONFIG_XIP_KERNEL
	/*                                                             */
	addr = ((unsigned long)_etext + PMD_SIZE - 1) & PMD_MASK;
#endif
	for ( ; addr < PAGE_OFFSET; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

	/*
                                              
  */
	end = memblock.memory.regions[0].base + memblock.memory.regions[0].size;
	if (end >= arm_lowmem_limit)
		end = arm_lowmem_limit;

	/*
                                                                 
                                          
  */
	for (addr = __phys_to_virt(end);
	     addr < VMALLOC_START; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));
}

#ifdef CONFIG_ARM_LPAE
/*                                    */
#define SWAPPER_PG_DIR_SIZE	(PAGE_SIZE + \
				 PTRS_PER_PGD * PTRS_PER_PMD * sizeof(pmd_t))
#else
#define SWAPPER_PG_DIR_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))
#endif

/*
                                        
 */
void __init arm_mm_memblock_reserve(void)
{
	/*
                                                       
                              
  */
	memblock_reserve(__pa(swapper_pg_dir), SWAPPER_PG_DIR_SIZE);

#ifdef CONFIG_SA1111
	/*
                                                          
                               
  */
	memblock_reserve(PHYS_OFFSET, __pa(swapper_pg_dir) - PHYS_OFFSET);
#endif
}

/*
                                                                          
                                                                          
                                                                        
                                                                       
                                                                        
 */
static void __init devicemaps_init(struct machine_desc *mdesc)
{
	struct map_desc map;
	unsigned long addr;
	void *vectors;

	/*
                                   
  */
	vectors = early_alloc(PAGE_SIZE * 2);

	early_trap_init(vectors);

	for (addr = VMALLOC_START; addr; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

	/*
                                
                                         
  */
#ifdef CONFIG_XIP_KERNEL
	map.pfn = __phys_to_pfn(CONFIG_XIP_PHYS_ADDR & SECTION_MASK);
	map.virtual = MODULES_VADDR;
	map.length = ((unsigned long)_etext - map.virtual + ~SECTION_MASK) & SECTION_MASK;
	map.type = MT_ROM;
	create_mapping(&map);
#endif

	/*
                                   
  */
#ifdef FLUSH_BASE
	map.pfn = __phys_to_pfn(FLUSH_BASE_PHYS);
	map.virtual = FLUSH_BASE;
	map.length = SZ_1M;
	map.type = MT_CACHECLEAN;
	create_mapping(&map);
#endif
#ifdef FLUSH_BASE_MINICACHE
	map.pfn = __phys_to_pfn(FLUSH_BASE_PHYS + SZ_1M);
	map.virtual = FLUSH_BASE_MINICACHE;
	map.length = SZ_1M;
	map.type = MT_MINICLEAN;
	create_mapping(&map);
#endif

	/*
                                                                
                                                                 
                                                        
  */
	map.pfn = __phys_to_pfn(virt_to_phys(vectors));
	map.virtual = 0xffff0000;
	map.length = PAGE_SIZE;
#ifdef CONFIG_KUSER_HELPERS
	map.type = MT_HIGH_VECTORS;
#else
	map.type = MT_LOW_VECTORS;
#endif
	create_mapping(&map);

	if (!vectors_high()) {
		map.virtual = 0;
		map.length = PAGE_SIZE * 2;
		map.type = MT_LOW_VECTORS;
		create_mapping(&map);
	}

	/*                                       */
	map.pfn += 1;
	map.virtual = 0xffff0000 + PAGE_SIZE;
	map.length = PAGE_SIZE;
	map.type = MT_LOW_VECTORS;
	create_mapping(&map);

	/*
                                                                    
  */
	if (mdesc->map_io)
		mdesc->map_io();
	fill_pmd_gaps();

	if (use_user_accessible_timers()) {
		/*
                                           
   */
		int page_addr = get_timer_page_address();
		if (page_addr != ARM_USER_ACCESSIBLE_TIMERS_INVALID_PAGE) {
			map.pfn = __phys_to_pfn(page_addr);
			map.virtual = CONFIG_ARM_USER_ACCESSIBLE_TIMER_BASE;
			map.length = PAGE_SIZE;
			map.type = MT_DEVICE_USER_ACCESSIBLE;
			create_mapping(&map);
		}
	}

	/*
                                                              
                                                                 
                                                                  
                                                                 
  */
	local_flush_tlb_all();
	flush_cache_all();
}

static void __init kmap_init(void)
{
#ifdef CONFIG_HIGHMEM
	pkmap_page_table = early_pte_alloc(pmd_off_k(PKMAP_BASE),
		PKMAP_BASE, _PAGE_KERNEL_TABLE);
#endif
}

#ifdef CONFIG_STRICT_MEMORY_RWX
static struct {
	pmd_t *pmd_to_flush;
	pmd_t *pmd;
	unsigned long addr;
	pmd_t saved_pmd;
	bool made_writeable;
} mem_unprotect;

static DEFINE_SPINLOCK(mem_text_writeable_lock);

void mem_text_writeable_spinlock(unsigned long *flags)
{
	spin_lock_irqsave(&mem_text_writeable_lock, *flags);
}

void mem_text_writeable_spinunlock(unsigned long *flags)
{
	spin_unlock_irqrestore(&mem_text_writeable_lock, *flags);
}

/*
                                                              
                                                        
                                                                     
                                                          
                                                                    
                                                          
                                                             
                                 
                                                              
                                      
  
                                                               
                                                      
                                                                
                                                
                                                                     
                     
 */

/*                                                                */
void mem_text_address_writeable(unsigned long addr)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->active_mm;
	pgd_t *pgd = pgd_offset(mm, addr);
	pud_t *pud = pud_offset(pgd, addr);

	mem_unprotect.made_writeable = 0;

	if ((addr < (unsigned long)RX_AREA_START) ||
	    (addr >= (unsigned long)RX_AREA_END))
		return;

	mem_unprotect.pmd = pmd_offset(pud, addr);
	mem_unprotect.pmd_to_flush = mem_unprotect.pmd;
	mem_unprotect.addr = addr & PAGE_MASK;

	if (addr & SECTION_SIZE)
			mem_unprotect.pmd++;

	mem_unprotect.saved_pmd = *mem_unprotect.pmd;
	if ((mem_unprotect.saved_pmd & PMD_TYPE_MASK) != PMD_TYPE_SECT)
		return;

	*mem_unprotect.pmd &= ~PMD_SECT_APX;

	flush_pmd_entry(mem_unprotect.pmd_to_flush);
	flush_tlb_kernel_page(mem_unprotect.addr);
	mem_unprotect.made_writeable = 1;
}

/*                                                                */
void mem_text_address_restore(void)
{
	if (mem_unprotect.made_writeable) {
		*mem_unprotect.pmd = mem_unprotect.saved_pmd;
		flush_pmd_entry(mem_unprotect.pmd_to_flush);
		flush_tlb_kernel_page(mem_unprotect.addr);
	}
}
#endif

void mem_text_write_kernel_word(unsigned long *addr, unsigned long word)
{
	unsigned long flags;

	mem_text_writeable_spinlock(&flags);
	mem_text_address_writeable((unsigned long)addr);
	*addr = word;
	flush_icache_range((unsigned long)addr,
			   ((unsigned long)addr + sizeof(long)));
	mem_text_address_restore();
	mem_text_writeable_spinunlock(&flags);
}
EXPORT_SYMBOL(mem_text_write_kernel_word);

static void __init map_lowmem(void)
{
	struct memblock_region *reg;
	struct vm_struct *vm;
	phys_addr_t start;
	phys_addr_t end;
	unsigned long vaddr;
	unsigned long pfn;
	unsigned long length;
	unsigned int type;
	int nr = 0;

	/*                                  */
	for_each_memblock(memory, reg) {
		struct map_desc map;
		nr++;
		start = reg->base;
		end = start + reg->size;

		if (end > arm_lowmem_limit)
			end = arm_lowmem_limit;
		if (start >= end)
			break;

		map.pfn = __phys_to_pfn(start);
		map.virtual = __phys_to_virt(start);
#ifdef CONFIG_STRICT_MEMORY_RWX
		if (start <= __pa(_text) && __pa(_text) < end) {
			map.length = SECTION_SIZE;
			map.type = MT_MEMORY_RW;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(start + SECTION_SIZE);
			map.virtual = __phys_to_virt(start + SECTION_SIZE);
			map.length = (unsigned long)RX_AREA_END - map.virtual;
			map.type = MT_MEMORY_RX;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__start_rodata));
			map.virtual = (unsigned long)__start_rodata;
			map.length = __init_begin - __start_rodata;
			map.type = MT_MEMORY_R;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__init_begin));
			map.virtual = (unsigned long)__init_begin;
			map.length = (char *)__arch_info_begin - __init_begin;
			map.type = MT_MEMORY_RX;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__arch_info_begin));
			map.virtual = (unsigned long)__arch_info_begin;
			map.length = __phys_to_virt(end) -
				(unsigned long)__arch_info_begin;
			map.type = MT_MEMORY_RW;
		} else {
			map.length = end - start;
			map.type = MT_MEMORY_RW;
		}
#else
		map.length = end - start;
		map.type = MT_MEMORY;
#endif

		create_mapping(&map);
	}

	vm = early_alloc_aligned(sizeof(*vm) * nr, __alignof__(*vm));

	for_each_memblock(memory, reg) {

		start = reg->base;
		end = start + reg->size;

		if (end > arm_lowmem_limit)
			end = arm_lowmem_limit;
		if (start >= end)
			break;

		pfn = __phys_to_pfn(start);
		vaddr = __phys_to_virt(start);
		length = end - start;
		type = MT_MEMORY;

		vm->addr = (void *)(vaddr & PAGE_MASK);
		vm->size = PAGE_ALIGN(length + (vaddr & ~PAGE_MASK));
		vm->phys_addr = __pfn_to_phys(pfn);
		vm->flags = VM_LOWMEM | VM_ARM_STATIC_MAPPING;
		vm->flags |= VM_ARM_MTYPE(type);
		vm->caller = map_lowmem;
		vm_area_add_early(vm);
		mark_vmalloc_reserved_area(vm->addr, vm->size);
		vm++;
	}
}

#ifdef CONFIG_FORCE_PAGES
/*
                         
                                                           
 */
static noinline void split_pmd(pmd_t *pmd, unsigned long addr,
				unsigned long end, unsigned long pfn,
				const struct mem_type *type)
{
	pte_t *pte, *start_pte;
	pmd_t *base_pmd;

	base_pmd = pmd_offset(
			pud_offset(pgd_offset(&init_mm, addr), addr), addr);

	if (pmd_none(*base_pmd) || pmd_bad(*base_pmd)) {
		start_pte = early_alloc(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE);
#ifndef CONFIG_ARM_LPAE
		/*
                                                             
                                                            
                  
   */
		if (addr & SECTION_SIZE)
			start_pte += pte_index(addr);
#endif
	} else {
		start_pte = pte_offset_kernel(base_pmd, addr);
	}

	pte = start_pte;

	do {
		set_pte_ext(pte, pfn_pte(pfn, type->prot_pte), 0);
		pfn++;
	} while (pte++, addr += PAGE_SIZE, addr != end);

	*pmd = __pmd((__pa(start_pte) + PTE_HWTABLE_OFF) | type->prot_l1);
	mb();
	flush_pmd_entry(pmd);
	flush_tlb_all();
}

/*
                                                                        
                                                               
 */
static void __init remap_pages(void)
{
	struct memblock_region *reg;

	for_each_memblock(memory, reg) {
		phys_addr_t phys_start = reg->base;
		phys_addr_t phys_end = reg->base + reg->size;
		unsigned long addr = (unsigned long)__va(phys_start);
		unsigned long end = (unsigned long)__va(phys_end);
		pmd_t *pmd = NULL;
		unsigned long next;
		unsigned long pfn = __phys_to_pfn(phys_start);
		bool fixup = false;
		unsigned long saved_start = addr;

		if (phys_end > arm_lowmem_limit)
			end = (unsigned long)__va(arm_lowmem_limit);
		if (phys_start >= phys_end)
			break;

		pmd = pmd_offset(
			pud_offset(pgd_offset(&init_mm, addr), addr), addr);

#ifndef	CONFIG_ARM_LPAE
		if (addr & SECTION_SIZE) {
			fixup = true;
			pmd_empty_section_gap((addr - SECTION_SIZE) & PMD_MASK);
			pmd++;
		}

		if (end & SECTION_SIZE)
			pmd_empty_section_gap(end);
#endif

		do {
			next = addr + SECTION_SIZE;

			if (pmd_none(*pmd) || pmd_bad(*pmd))
				split_pmd(pmd, addr, next, pfn,
						&mem_types[MT_MEMORY]);
			pmd++;
			pfn += SECTION_SIZE >> PAGE_SHIFT;

		} while (addr = next, addr < end);

		if (fixup) {
			/*
                                                          
                                                      
                                                         
                 
    */
			pmd = pmd_off_k(saved_start);
			pmd[0] = pmd[1] & ~1;
		}
	}
}
#else
static void __init remap_pages(void)
{

}
#endif

/*
                                                                     
                                                                 
 */
void __init paging_init(struct machine_desc *mdesc)
{
	void *zero_page;

	memblock_set_current_limit(arm_lowmem_limit);

	build_mem_type_table();
	prepare_page_table();
	map_lowmem();
	dma_contiguous_remap();
	remap_pages();
	devicemaps_init(mdesc);
	kmap_init();

	top_pmd = pmd_off_k(0xffff0000);

	/*                         */
	zero_page = early_alloc(PAGE_SIZE);

	bootmem_init();

	empty_zero_page = virt_to_page(zero_page);
	__flush_dcache_page(NULL, empty_zero_page);
}