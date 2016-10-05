#include "cpu/exec/template-start.h"

#define instr movzx

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute()
{
	DATA_TYPE result = (DATA_TYPE)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rmb2r)

#endif
//make_instr_helper()

#include "cpu/exec/template-end.h"
