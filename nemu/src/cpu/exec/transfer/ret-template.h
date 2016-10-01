#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE == 4
static void do_execute()
{
	cpu.eip = MEM_R(REG(R_ESP));
	REG(R_ESP) += DATA_BYTE;	
	print_asm_template2();
}

make_helper(concat(pop_, SUFFIX))
{
	do_execute();	
	return 0;	
}
#endif

#include "cpu/exec/template-end.h"
