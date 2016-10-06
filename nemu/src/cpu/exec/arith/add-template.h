#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define RET_DATA_TYPE int8_t
#elif DATA_BYTE == 2
#define RET_DATA_TYPE int16_t
#elif DATA_BYTE == 4
#define RET_DATA_TYPE int32_t
#endif

#define instr add

static void do_execute()
{	
	DATA_TYPE result;
	if(op_src->type == OP_TYPE_IMM)
		result = (RET_DATA_TYPE)op_dest->imm + (RET_DATA_TYPE)op_src->imm;
	else 
		result = op_dest->val + op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(r2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm_imm)
#endif

#undef RET_DATA_TYPE

#include "cpu/exec/template-end.h"
