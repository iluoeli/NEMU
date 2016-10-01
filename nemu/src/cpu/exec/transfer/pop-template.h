#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute()
{
	cpu.eip = REG(R_ESP);
	REG(R_ESP) -= DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
