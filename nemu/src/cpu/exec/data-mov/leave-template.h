#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute()
{
	REG(R_ESP) = REG(R_EBP);
	REG(R_EBP) = MEM_R(REG(R_ESP));	
	print_asm_template1();
}

make_helper(leave_l)
{
	do_execute();
	return 1;	
}

#include "cpu/exec/template-end.h"
