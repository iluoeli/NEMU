#include "cpu/exec/helper.h"

make_helper(movs)
{
	swaddr_write(reg_l(R_EDI), 1, swaddr_read(reg_l(R_ESI), 1));
	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI) ++;
	else
		reg_l(R_EDI) --;
	print_asm("movs");
	return 1;
}
