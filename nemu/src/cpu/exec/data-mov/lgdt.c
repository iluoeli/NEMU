#include "cpu/exec/helper.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
	uint32_t addr = op_src->addr;
	cpu.GDTR.limit = swaddr_read(addr, 2, 1);
	cpu.GDTR.base = swaddr_read(addr+2, 4, 1);
	print_asm("lgdt %x:%x,gdtr", cpu.GDTR.limit, cpu.GDTR.base);
	return len + 1;
}

make_helper(lidt)
{
	int len = decode_rm_l(cpu.eip+2);
	uint32_t addr = op_src->addr;
	cpu.IDTR.limit = swaddr_read(addr, 2, 1);
	cpu.IDTR.base = swaddr_read(addr+2, 4, 1);
	print_asm("lidt %x:%x,idtr", cpu.IDTR.limit, cpu.IDTR.base);
	return len + 1;
}

