#include "cpu/exec/helper.h"

make_helper(stos)
{
	if(ops_decoded.is_operand_size_16 == true) {
		swaddr_write(reg_w(R_DI), reg_b(R_AL), 1);
		reg_w(R_DI) += cpu.EFLAGES.DF?-1:1;
	}
	else{
		swaddr_write(reg_l(R_EDI), reg_b(R_AL), 1);
		reg_l(R_EDI) += cpu.EFLAGES.DF?-1:1;
	}
	
	print_asm("stos");
	return 1;
}
