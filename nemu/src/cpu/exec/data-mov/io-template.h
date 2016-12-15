#include "cpu/exec/template-start.h"

#define instr in
static void do_execute()
{
	REG(R_EAX) = pio_read(op_src->val, DATA_BYTE);	
}

make_helper(concat(in_, SUFFIX))
{
	int len = decode_i_b(cpu.eip+1);
	do_execute();

	print_asm("in" str(SUFFIX) "0x%x, %%%s", op_src->val, REG_NAME(R_EAX));

	return len+1;
}


#include "cpu/exec/template-end.h"
