#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
#if DATA_BYTE == 1
	reg_l(R_ESP) -= 4;
#undef DATA_BYTE
#define DATA_BYTE 4
	MEM_W(reg_l(R_ESP), (DATA_TYPE_S)op_src->val, 2);
#undef DATA_BYTE
#define DATA_BYTE 1

#elif DATA_BYTE == 2
	reg_l(R_ESP) -= 4;
#undef DATA_BYTE
#define DATA_BYTE 4
	MEM_W(reg_l(R_ESP), (DATA_TYPE_S)op_src->val, 2);
#undef DATA_BYTE
#define DATA_BYTE 2

#elif DATA_BYTE == 4
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP), op_src->val, 2);
#endif
 /*	REG(R_ESP) -= DATA_BYTE; 
	MEM_W(REG(R_ESP), op_src->val);	*/
	print_asm_template1();
}


make_instr_helper(r)
make_instr_helper(rm)
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
