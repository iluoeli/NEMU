#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
	OPERAND_W(op_src, MEM_R(reg_l(R_ESP), 2));
	//reg_l(R_ESP) += DATA_BYTE;
//	REG(op_src->reg) = MEM_R(REG(R_ESP));
	reg_l(R_ESP) += 4;
/*	if(instr_fetch(cpu.eip, 1) == 0xc2)
		reg_l(R_ESP) += instr_fetch(cpu.eip+1, 2);
*/
	print_asm_template1();
}


make_instr_helper(r)
/*
#if DATA_BYTE == 2
make_instr_helper(i)
#endif*/
/*
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(i)
#endif
*/
/*
#if DATA_BYTE == 4
make_helper(pop_b)
{
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_EBP;	
	op_src->val = op_src->reg;
	do_execute();
	return 1;		
}
#endif
*/
#include "cpu/exec/template-end.h"
