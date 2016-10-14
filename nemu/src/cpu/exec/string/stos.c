#include "cpu/exec/helper.h"

make_helper(stos_b)
{
	swaddr_write(reg_l(R_EDI), reg_b(R_AL), 1);
	reg_l(R_EDI) += cpu.EFLAGES.DF?-1:1;
	
	print_asm("stosb");
	return 1;
}

make_helper(stos_w)
{
	swaddr_write(reg_l(R_EDI), reg_w(R_AX), 2);
	reg_l(R_EDI) += cpu.EFLAGES.DF?-2:2;
	
	print_asm("stosw");
	return 1;
}

make_helper(stos_l)
{
	swaddr_write(reg_l(R_EDI), reg_l(R_EAX), 4);
	reg_l(R_EDI) += cpu.EFLAGES.DF?-4:4;
	
	print_asm("stosl");
	return 1;
}
make_helper(stos_v)
{
	return (ops_decoded.is_operand_size_16?stos_w:stos_l)(eip);	
}
