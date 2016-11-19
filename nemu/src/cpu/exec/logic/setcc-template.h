#include "cpu/exec/template-start.h"

//#define instr setne
make_helper(concat(instr,_rm_b))
{
	int len = decode_rm_b(eip+1);
	DATA_TYPE result = instr_condition;
	OPERAND_W(op_src, result);
	print_asm_template1();
	return len+1;	
}
//#undef instr
	

#include "cpu/exec/template-end.h"
