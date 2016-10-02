#include "cpu/exec/template-start.h"

#define instr call

#if DATA_BYTE == 4
static void do_execute()
{
	REG(R_ESP) -= DATA_BYTE;
	MEM_W(REG(R_ESP), (cpu.eip+1+DATA_BYTE) & (0xff<<DATA_BYTE));
	cpu.eip = (op_src->val + cpu.eip) & (0xff<<DATA_BYTE);	
	print_asm_template1();
}

make_instr_helper(i)
#endif



#include "cpu/exec/template-end.h"
