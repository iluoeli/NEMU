#include "common.h"
#include "nemu.h"
//#include "x86.h"

typedef struct SegmentDescriptor {
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
} SegDesc;

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);

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
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
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
	if(cpu.CR0.PE == 1){
		printf("protect mod: %x\n", addr);
	 	if(cpu.sr[sreg].TI == 0){
			printf("TI == 0\n");
			uint32_t gdt_base = cpu.GDTR.base;
			printf("gdt_base: %x\n", gdt_base);
			SegDesc *gdt = NULL;
			gdt =  (SegDesc *)(void *)(gdt_base + cpu.sr[sreg].index);	
			printf("gdt: %x\n", (uint32_t)gdt);
			uint32_t base_addr = (gdt->base_31_24 << 24) + (gdt->base_23_16 << 16) + gdt->base_15_0;
			uint32_t offset_addr = addr;

			printf("offset_addr: %x", offset_addr);
			printf("base_addr: %x", base_addr);
			return (base_addr + offset_addr);
		}			
	}
	return addr;
}

