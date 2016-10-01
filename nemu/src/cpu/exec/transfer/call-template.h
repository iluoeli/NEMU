#include "cpu/exec/template-start.h"

#define instr call

#if DATA_BYTE == 4
static void do_execute()
{
	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), cpu.eip);
	cpu.eip += op_src->imm;	
	print_asm_template2();
}

make_instr_helper(i)
#endif



#include "cpu/exec/template-end.h"
