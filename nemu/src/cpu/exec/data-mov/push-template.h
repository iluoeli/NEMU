#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
	cpu.gpr[4]._32 -= DATA_BYTE;
	MEM_W(REG(R_ESP), op_src->val);	
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
