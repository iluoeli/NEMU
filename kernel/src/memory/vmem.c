#include "common.h"
#include "memory.h"
#include <string.h>
#include "x86.h"

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
PDE* get_updir();

static PTE vptable[NR_PTE] align_to_page;

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	PDE *kpdir = get_updir();	
	PDE *pdir = va_to_pa(kpdir);
	PTE *ptable = va_to_pa(vptable);

	uint32_t nr_ptable = SCR_SIZE/PAGE_SIZE + 1;
	uint32_t pdir_idx = (VMEM_ADDR >> 22) & 0x3ff;
	uint32_t pte_idx = (VMEM_ADDR >> 12) & 0x3ff;

	pdir[pdir_idx].val = make_pde(ptable);
	pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);

	assert(pdir[pdir_idx].present == 1);

 //   uint32_t pte_addr = (pdir[pdir_idx].page_frame << 12) + 4 * (pte_idx+nr_ptable);

//	ptable = (PTE *)(pte_addr);
	ptable += nr_ptable-1;	
//	uint32_t pframe_addr = VMEM_ADDR+PAGE_SIZE * (nr_ptable-1);
	uint32_t pframe_addr = NR_PTE * PAGE_SIZE-PAGE_SIZE;

    for (; pframe_addr >= 0; pframe_addr -= PAGE_SIZE) {
		ptable->val = make_pte(pframe_addr);
		ptable --;
	} 



//	panic("please implement me");
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
 	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
 	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
 	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

