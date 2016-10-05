#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
	REG(op_src->reg) = MEM_R(REG(R_ESP));
	REG(R_ESP) += 4;
	print_asm_template2();
}


make_instr_helper(r)

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

#include "cpu/exec/template-end.h"
