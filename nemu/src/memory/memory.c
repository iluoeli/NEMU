#include "common.h"
#include "nemu.h"
//#include "x86.h"

typedef union SegmentDescriptor {
	struct {
		uint32_t limit_15_0          : 16;
		uint32_t base_15_0           : 16;
		uint32_t base_23_16          : 8;
		uint32_t type                : 4;
		uint32_t segment_type        : 1;
		uint32_t privilege_level     : 2;
		uint32_t present             : 1;
		uint32_t limit_19_16         : 4;
		uint32_t soft_use            : 1;
		uint32_t operation_size      : 1;
		uint32_t pad0                : 1;
		uint32_t granularity         : 1;
		uint32_t base_31_24          : 8;
	};
 	struct {
		uint32_t val_1;
		uint32_t val_2;	
	};
} SegDesc;


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
				
 /* the 32bit Page Table Entry(second level page table) data structure */
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

typedef union{
	struct{
		uint32_t offset	:		12;
		uint32_t pte_index:		10;	
		uint32_t pde_index:		10;
	};
	uint32_t addr;	
} PAGE_ADDR;

											    

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);
uint32_t page_translate(hwaddr_t addr);
uint32_t tlb_read(lnaddr_t);

int is_mmio(hwaddr_t addr);
uint32_t mmio_read(hwaddr_t addr, size_t len, int map_NO);
void mmio_write(hwaddr_t addr, size_t len, uint32_t data, int map_NO);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int map_NO = is_mmio(addr);
	if(map_NO == -1)
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	else 
		return mmio_read(addr, len, map_NO);

//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int map_NO = is_mmio(addr);
	if(map_NO == -1)
		dram_write(addr, len, data);
	else 
		mmio_write(addr, len , data, map_NO);
//	dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
/*	if(data corss the page boundary){
		assert(0);
 	}*/
//	else 
	{ 
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	
 	}


	//return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
//	hwaddr_write(addr, len, data);
	assert(len == 1 || len == 2 || len == 4);
/*	if(data corss the page boundary){
		assert(0);
	}*/
//	else 
	{ 
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
	//return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
	assert(sreg <= 5 && sreg >= 0);
	
 	if(cpu.CR0.PE == 1){
  	  	if(cpu.sr[sreg].TI == 0){
 	/*		uint32_t gdt_base = cpu.GDTR.base;
			SegDesc gdt;
			uint32_t tmp_addr =  (gdt_base + 8*cpu.sr[sreg].index);	
			gdt.val_1 = hwaddr_read(tmp_addr, 4);
			gdt.val_2 = hwaddr_read(tmp_addr+4, 4);
			uint32_t base_addr = (gdt.base_31_24 << 24) + (gdt.base_23_16 << 16) + gdt.base_15_0;

			assert(base_addr == 0);
	 		return (base_addr + addr);*/
			assert(cpu.sr[sreg].DPL >= cpu.CS.DPL && cpu.sr[sreg].DPL >= cpu.sr[sreg].RPL);
			return (cpu.sr[sreg].base+addr);
		}			
	}
	return addr;
}


uint32_t page_translate(hwaddr_t addr)
{	
	if(cpu.CR0.PE == 1 && cpu.CR0.PG == 1){
		PAGE_ADDR paddr;
		paddr.addr = addr;
 /*		uint32_t tmp_addr  = ((cpu.CR3.page_directory_base << 12) + 4*paddr.pde_index);			
		uint32_t val = hwaddr_read(tmp_addr, 4);
		PDE pde;
		pde.val = val;
		assert(pde.present == 1);		
		
		tmp_addr = ((pde.page_frame << 12) + 4*paddr.pte_index);
		PTE pte;
		val = hwaddr_read(tmp_addr, 4);
		pte.val = val;
		assert(pte.present == 1);	
		return ((pte.page_frame << 12) + paddr.offset);
*/
		return (tlb_read(addr)<<12) + paddr.offset;
	}

	else return addr;	
}

// for ui.c
void print_page(uint32_t addr)
{
	uint32_t target_addr = addr;
	bool success = true;	
 	if(cpu.CR0.PE == 1 && cpu.CR0.PG == 1){
		PAGE_ADDR paddr;
		paddr.addr = addr;
		uint32_t tmp_addr = ((cpu.CR3.page_directory_base << 12) + 4*paddr.pde_index);			
		uint32_t val = hwaddr_read(tmp_addr, 4);
		PDE pde;
		pde.val = val;
		if(pde.present == 0){
			success = false;
		}
		
		tmp_addr = ((pde.page_frame << 12) + 4*paddr.pte_index);
		PTE pte;
		val = hwaddr_read(tmp_addr, 4);
		pte.val = val;
		if(pte.present == 0){
			success = false;	
		}	
		target_addr = ((pte.page_frame << 12) + paddr.offset);
	}
	if(success){
		printf("0x%x\n", target_addr);	
	}
	else 
		printf("Error:	translate failed\n");
	
}

