#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define instr lods

make_helper(lods)
{
 	if(ops_decoded.is_operand_size_16 == true) {
		reg_b(R_AL) = instr_fetch(reg_w(R_SI), 1);
		reg_w(R_SI) += cpu.EFLAGES.DF ? -1:1;
	}
	else{
		reg_b(R_AL) = instr_fetch(reg_l(R_ESI), 1);
		reg_l(R_SI) += cpu.EFLAGES.DF ? -1:1;
	}
	print_asm("lods");
	return 1;
}

