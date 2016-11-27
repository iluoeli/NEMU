#include "cpu/exec/helper.h"

make_helper(lgdt)
{
	uint32_t addr = swaddr_read(cpu.eip, 4,  3);
	cpu.GDTR.base = swaddr_read(addr, 4, 3);
	cpu.GDTR.limit = swaddr_read(addr+2, 2, 3);
	return 6;
}

