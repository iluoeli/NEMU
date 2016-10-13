#include "cpu/exec/helper.h"


make_helper(scas)
{
	uint8_t dest;
	uint8_t al = reg_b(R_AL);
	uint16_t result;
	dest = swaddr_read(reg_l(R_EDI), 1);
	result = al - dest;
	cpu.EFLAGES.OF =  ( (dest>>7 & 1) == (al>>7 & 1) && ((al>>7 & 1) != (result>>7 & 1)));
	cpu.EFLAGES.SF = (result>>7 & 1);
	cpu.EFLAGES.ZF = ((uint8_t)result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (result>>8 & 1) ^ 1;

	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI)++;
	else
		reg_l(R_EDI)--;

	print_asm("scas");
	return 1;
}
