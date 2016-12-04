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

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write(addr, len, data);
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
		return hwaddr_write(hwaddr, len, data);
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
#endif
	return lnaddr_read(lnaddr, len);
	//return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
#endif
	lnaddr_write(lnaddr, len, data);
}

uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
	assert(sreg <= 5 && sreg >= 0);
	// mode
/*	if(cpu.CR0.PE == 1){
	//	printf("protect mod: %x\n", addr);
	 	if(cpu.sr[sreg].TI == 0){
	//		printf("TI == 0\n");
			uint32_t gdt_base = cpu.GDTR.base;
	//		printf("gdt_base: %x\n", gdt_base);
			SegDesc gdt;
			uint32_t addr =  (gdt_base + cpu.sr[sreg].index);	
			gdt.val_1 = lnaddr_read(addr, 4);
			gdt.val_2 = lnaddr_read(addr+4, 4);
			uint32_t base_addr = (gdt.base_31_24 << 24) + (gdt.base_23_16 << 16) + gdt.base_15_0;
			uint32_t offset_addr = addr;

//			printf("offset_addr: %x", offset_addr);
	//		printf("base_addr: %x", base_addr);
			return (base_addr + offset_addr);
		}			
	}*/
	return addr;
}


uint32_t page_translate(hwaddr_t addr)
{	
	if(cpu.CR0.PE == 1 && cpu.CR0.PG == 1){
		PAGE_ADDR paddr;
		paddr.addr = addr;

		uint32_t addr = ((cpu.CR3.page_directory_base << 12) + paddr.pde_index);			
		uint32_t val = hwaddr_read(addr, 4);
		PDE pde;
		pde.val = val;
		assert(pde.present == 1);		
		
		addr = ((pde.page_frame << 12) + paddr.pte_index);
		PTE pte;
		val = hwaddr_read(addr, 4);
		pte.val = val;
		assert(pte.present == 1);	
		return ((pte.page_frame << 12) + paddr.offset);
	}

	else return addr;	
}
