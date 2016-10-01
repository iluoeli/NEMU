#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
	MEM_W(REG(R_ESP),REG(op_src->reg));
	REG(R_ESP) -= DATA_BYTE;
	print_asm_template2();
}


make_instr_helper(r)

#if DATA_BYTE == 4
make_helper(pop_b)
{
	op_src->type = OP_TYPE_REG;
	op_src->reg = R_EBP;	
	op_src->val = op_src->reg;
	return 1;		
}
#endif

#include "cpu/exec/template-end.h"
