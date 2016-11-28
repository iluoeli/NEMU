#include "cpu/exec/helper.h"

make_helper(lgdt)
{
//	int len = decode_rm_l(cpu.eip+2);
//	uint32_t addr = op_src->addr;
//	uint8_t sreg = op_src->sreg;
	uint32_t addr = instr_fetch(cpu.eip+2, 4);
	cpu.GDTR.base = instr_fetch(addr, 4);
	cpu.GDTR.limit = instr_fetch(addr+4, 2);
	return 4;
}

