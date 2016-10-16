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
#endif


//call indirect
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(call_rm_, SUFFIX))
{
	concat(decode_rm_, SUFFIX)(eip+1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	cpu.eip = op_src->val;
	print_asm_template1();
	return 0;
}
#endif



#include "cpu/exec/template-end.h"
