#include "cpu/exec/template-start.h"

#define instr movzb
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movzb_, SUFFIX))
{
	int len = concat(decode_rmb2r_, SUFFIX)(eip+1);
	uint32_t result = (uint32_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
	return 1+len;
}
#endif
#undef instr

#define instr movzw
#if DATA_BYTE == 4
make_helper(movzw_l)
{
	int len = decode_rmw2r_l(eip+1);
	uint32_t result = (uint32_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
	return 1+len;
}
#endif
#undef instr


#include "cpu/exec/template-end.h"
