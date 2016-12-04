#include "cpu/exec/helper.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
	uint32_t addr = op_src->addr;
	cpu.GDTR.limit = lnaddr_read(addr, 2);
	cpu.GDTR.base = lnaddr_read(addr+2, 4);
	print_asm("lgdt %x:%x,gdtr", cpu.GDTR.limit, cpu.GDTR.base);
	return len + 1;
}

