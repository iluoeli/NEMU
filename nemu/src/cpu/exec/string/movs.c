#include "cpu/exec/helper.h"

make_helper(movs_b)
{
	swaddr_write(reg_l(R_EDI), 1, swaddr_read(reg_l(R_ESI), 1));
	if(cpu.EFLAGES.DF == 0){
		reg_l(R_EDI) ++;
		reg_l(R_ESI) ++;
	}
 	else{
		reg_l(R_EDI) --;
		reg_l(R_ESI) --;
	}
	print_asm("movsb");
	return 1;
}

make_helper(movs_w)
{
	swaddr_write(reg_l(R_EDI), 2, swaddr_read(reg_l(R_ESI), 2));
 	if(cpu.EFLAGES.DF == 0){
		reg_l(R_EDI) += 2;
		reg_l(R_ESI) += 2;
	}
 	else{
		reg_l(R_EDI) -= 2;
		reg_l(R_ESI) -= 2;
	}
	print_asm("movsw");
	return 1;
}

make_helper(movs_l)
{
	swaddr_write(reg_l(R_EDI), 4, instr_fetch(reg_l(R_ESI), 4));
 	if(cpu.EFLAGES.DF == 0){
		reg_l(R_EDI) += 4;
		reg_l(R_ESI) += 4;
	}
  	else{
		reg_l(R_EDI) -= 4;
		reg_l(R_ESI) -= 4;
	}
	print_asm("movsl");
	return 1;
}

make_helper(movs_v)
{
	return (ops_decoded.is_operand_size_16 ? movs_w:movs_l)(eip); 	
}
