#include "common.h"
#include "nemu.h"
#include "random.h"

#define TLB_WIDTH 6
#define TLB_SIZE (1 << TLB_WIDTH)


typedef struct {
	bool valid;				//1b
	uint32_t tag	;		//20b
	uint32_t page_frame;	//20b
}TLBSlot;

typedef union{
	struct {
		uint32_t offset	:12;
		uint32_t tag	:20;	
	};
	struct {
		uint32_t :	12;
		uint32_t pte_index:	10;
		uint32_t pde_index:	10;	
	};	
	uint32_t addr;
}tlb_addr;


typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
	    uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
        uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
    };
	uint32_t val;
} PDE;
					  

typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
	    uint32_t read_write          : 1;
	    uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;


TLBSlot tlb[TLB_SIZE];

void init_TLB()
{
	int i;
	for	(i=0; i < TLB_SIZE; ++i)
		tlb[i].valid = false;
}



uint32_t tlb_read(hwaddr_t addr)
{
	tlb_addr temp;
	temp.addr = addr;
	uint32_t tag = temp.tag;
	int i;
	bool hit = false;
	for (i=0; i < TLB_SIZE; ++i){
		if(tlb[i].valid && tlb[i].tag == tag){
			hit = true;	break;	
		}
	}
	
	//if not hit, random replace
	if(!hit) {
		if(cpu.CR0.PE == 1 && cpu.CR0.PG == 1){
			i = randomGenerator() % TLB_SIZE;
			uint32_t tmp_addr = (cpu.CR3.page_directory_base << 12) + 4*temp.pde_index;
			PDE pde;
			pde.val = hwaddr_read(tmp_addr, 4);
			assert(pde.present == 1);

			tmp_addr = (pde.page_frame << 12) + 4*temp.pte_index;
			PTE pte;
			pte.val = hwaddr_read(tmp_addr, 4);
			assert(pte.present == 1);
			
			tlb[i].tag = temp.tag;
			tlb[i].page_frame = pte.page_frame;
			tlb[i].valid = true;
		}	
	}	
	return tlb[i].page_frame;
}
