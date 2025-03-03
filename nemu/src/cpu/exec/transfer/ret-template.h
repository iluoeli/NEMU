#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE == 4
static void do_execute()
{
	if(ops_decoded.is_operand_size_16) {
		cpu.eip = MEM_R(REG(R_ESP), 2) & 0x0000ffff;
		reg_w(R_SP) += 2;
	 }
   	else {
		cpu.eip = MEM_R(REG(R_ESP), 2);
		reg_l(R_ESP) += 4;
	}
//	REG(R_ESP) += DATA_BYTE;
	cpu.eip -= 1;	
	print_asm("ret");
}

make_helper(concat(ret_, SUFFIX))
{
	do_execute();	
	return 1;	
}
make_helper(ret_i_b)
{
	decode_i_b(eip+1);
	do_execute();
	reg_l(R_ESP) += op_src->val;	
	return 1;	
}
#endif

#include "cpu/exec/template-end.h"
