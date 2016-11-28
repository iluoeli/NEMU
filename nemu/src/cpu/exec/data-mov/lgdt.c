#include "cpu/exec/helper.h"

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
//	uint32_t addr = op_src->addr;
	uint32_t addr = instr_fetch(cpu.eip+2, 4);
	uint8_t sreg = op_src->sreg;
	printf("lgdt: %x, %x\n", addr, sreg);
/*	cpu.GDTR.limit = swaddr_read(addr, 2, sreg);
	cpu.GDTR.base = swaddr_read(addr+2, 4, sreg);
*/	cpu.GDTR.base = swaddr_read(addr, 4, sreg);
	cpu.GDTR.limit = swaddr_read(addr+4, 2, sreg);
	print_asm("lgdt %x:%x,gdtr", cpu.GDTR.limit, cpu.GDTR.base);
//	printf("%x, %x\n", cpu.GDTR.limit, cpu.GDTR.base);
	return len + 1;
}

