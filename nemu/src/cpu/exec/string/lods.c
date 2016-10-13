#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(lods_b)
{
  	if(ops_decoded.is_operand_size_16 == true) {
		reg_b(R_AL) = instr_fetch(reg_w(R_SI), 1);
		reg_w(R_SI) += cpu.EFLAGES.DF ? -1:1;
  	}
 	else{
		reg_b(R_AL) = instr_fetch(reg_l(R_ESI), 1);
		reg_l(R_ESI) += cpu.EFLAGES.DF ? -1:1;
 	}
	print_asm("lodsb");
	return 1;
}

make_helper(lods_w)
{
  	if(ops_decoded.is_operand_size_16 == true) {
		reg_w(R_AX) = instr_fetch(reg_w(R_SI), 2);
		reg_w(R_SI) += cpu.EFLAGES.DF ? -2:2;
 	}
 	else{
		reg_w(R_AX) = instr_fetch(reg_l(R_ESI), 2);
		reg_l(R_ESI) += cpu.EFLAGES.DF ? -2:2;
 	}
	print_asm("lodsw");
	return 1;
}

make_helper(lods_l)
{
  	if(ops_decoded.is_operand_size_16 == true) {
		reg_l(R_EAX) = instr_fetch(reg_w(R_SI), 4);
		reg_w(R_SI) += cpu.EFLAGES.DF ? -4:4;
 	}
 	else{
		reg_l(R_EAX) = instr_fetch(reg_l(R_ESI), 4);
		reg_l(R_ESI) += cpu.EFLAGES.DF ? -4:4;
	}
	print_asm("lodsl");
	return 1;
}

make_helper(lods_v)
{
	return (ops_decoded.is_operand_size_16?lods_w:lods_l)(eip);	
}
