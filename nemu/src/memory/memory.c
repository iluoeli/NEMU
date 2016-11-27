#include "common.h"

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

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
	//Inaddr_t Inaddr = seg_translate(addr, len, sreg);
#endif
	//return lnaddr_read(Inaddr, len);
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}
/*
uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
	assert(sreg <= 5 && sreg >= 0);
	if(cpu.sr[sreg].TI == 0){
		uint32_t gdt_addr = cpu.GDTR.gdt_addr;
		SegmentDescriptor *SegDesc = (void *)(gdt_addr + cpu.sr[sreg].index);	
		uint32_t base_addr = (SegDesc->base_31_24 << 24) + (SegDesc->base_23_16 << 16) + SegDesc->base_15_0;
		uint32_t offset_addr = addr;
		return (base_addr = offset_addr);
	}			
}
*/
