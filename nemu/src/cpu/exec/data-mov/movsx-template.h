#include "cpu/exec/template-start.h"


#define instr movsb
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movsb_, SUFFIX))
{
	int len = concat(decode_rmb2r_, SUFFIX)(eip);
	int32_t result = (int32_t)(int8_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
	return 1+len;
}
#endif
#undef instr

#define instr movsw
#if DATA_BYTE == 4
make_helper(movsw_l)
{
	int len = decode_rmw2r_l(eip);	
	int32_t result = (int32_t)(int16_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
	return 1+len;	
}
#endif
#undef instr

#include "cpu/exec/template-end.h"
