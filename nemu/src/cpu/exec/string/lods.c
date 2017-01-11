#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(lods_b)
{
	reg_b(R_AL) = swaddr_read(reg_l(R_ESI), 1, 0);
	reg_l(R_ESI) += cpu.EFLAGES.DF ? -1:1;
	print_asm("lodsb");
	return 1;
}

make_helper(lods_w)
{
	reg_w(R_AX) = swaddr_read(reg_l(R_ESI), 2, 0);
	reg_l(R_ESI) += cpu.EFLAGES.DF ? -2:2;
	print_asm("lodsw");
	return 1;
}

make_helper(lods_l)
{
	reg_l(R_EAX) = swaddr_read(reg_l(R_ESI), 4, 0);
	reg_l(R_ESI) += cpu.EFLAGES.DF ? -4:4;
	print_asm("lodsl");
	return 1;
}

make_helper(lods_v)
{
	return (ops_decoded.is_operand_size_16?lods_w:lods_l)(eip);	
}
