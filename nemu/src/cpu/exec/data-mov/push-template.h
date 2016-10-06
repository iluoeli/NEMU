#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
//#if DATA_BYTE == 1 || DATA_BYTE == 4
//	reg_l(R_ESP) -= 4;
//	MEM_W(reg_l(R_ESP), op_src->val);
//#elif DATA_BYTE == 2
	reg_w(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP), op_src->val);
//#endif
/*	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), op_src->val);	*/
	print_asm_template1();
}


make_instr_helper(r)
make_instr_helper(i)

/*
make_helper(push_a)
{
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_EAX;
	op_src->val = op_src->reg;
	do_execute();	
	return 1;
}
*/

#include "cpu/exec/template-end.h"
