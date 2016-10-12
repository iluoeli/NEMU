#include "cpu/exec/helper.h"

make_helper(stos)
{
	if(ops_decoded.is_operand_size_16 == true)
		reg_w(R_DI) = reg_w(R_AL);
	else
		reg_l(R_EDI) = reg_b(R_AL);
	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI) ++;
	else
		reg_l(R_EDI) --;
	
	print_asm("stos");
	return 1;
}
