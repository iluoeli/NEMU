#include "cpu/exec/helper.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
//	int len = 5;
//	uint32_t addr = instr_fetch(cpu.eip+3, 4);
	uint32_t addr = op_src->addr;
	uint8_t sreg = op_src->sreg;
	printf("lgdt: %x, %x\n", addr, sreg);
//	cpu.GDTR.limit = lnaddr_read(addr, 2);
//	cpu.GDTR.base = lnaddr_read(addr+2, 4);
	cpu.GDTR.base = swaddr_read(addr, 4, sreg);
	cpu.GDTR.limit = swaddr_read(addr+3, 2, sreg);
	cpu.GDTR.base &= 0x00ffffff;
	cpu.GDTR.base |= (swaddr_read(addr+5, 1,sreg)<<24);
	print_asm("lgdt %x:%x,gdtr", cpu.GDTR.limit, cpu.GDTR.base);
	return len + 1;
}

