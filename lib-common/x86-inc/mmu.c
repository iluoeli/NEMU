#include "commom.h"
#include "mmu.h"


uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
    assert(sreg <= 5 && sreg >= 0);
	if(cpu.sr[sreg].TI == 0){
		uint32_t gdt_addr = cpu.GDTR.gdt_addr;
		SegmentDescriptor *SegDesc = (void *)(gdt_addr + cpu.sr[sreg].index)    ;
		uint32_t base_addr = (SegDesc->base_31_24 << 24) + (SegDesc->base_23    _16 << 16) + SegDesc->base_15_0;
		uint32_t offset_addr = addr;
		return (base_addr = offset_addr);
	}
}
