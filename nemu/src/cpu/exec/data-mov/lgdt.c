#include "cpu/exec/helper.h"

make_helper(lgdt)
{
	int len = decode_rm_l(cpu.eip+2);
	uint32_t addr = op_src->addr;
	uint8_t sreg = op_src->sreg;
	cpu.GDTR.base = swaddr_read(addr, 4, sreg);
	cpu.GDTR.limit = swaddr_read(addr+4, 2, sreg);
	return len;
}

