#include "cpu/exec/helper.h"

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
	uint32_t addr = op_src->addr;
	uint8_t sreg = op_src->sreg;
	printf("lgdt: addr = %d, sreg = %d", addr, sreg);
//	uint32_t addr = instr_fetch(cpu.eip+2, 4);
	cpu.GDTR.limit = swaddr_read(addr, 2, sreg);
	cpu.GDTR.base = swaddr_read(addr+2, 4, sreg);
	return len;
}

