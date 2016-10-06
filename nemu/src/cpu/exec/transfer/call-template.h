#include "cpu/exec/template-start.h"

#define instr call

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute()
{
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), cpu.eip+1+DATA_BYTE);
	cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)
#endif



#include "cpu/exec/template-end.h"
