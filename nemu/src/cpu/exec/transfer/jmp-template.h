#include "cpu/exec/template-start.h"

#define instr jmp


//jmp direct 
static void do_execute()
{
	cpu.eip += op_src->val;
	if(ops_decoded.is_operand_size_16 == true)
		cpu.eip = cpu.eip & 0x0000ffff;
	print_asm_template1();
}

make_instr_helper(i)

//jmp indirect
make_helper(concat(jmp_rm_, SUFFIX))
{
	concat(decode_rm_, SUFFIX)(eip+1);
	if(ops_decoded.is_operand_size_16 == true){
		cpu.eip = reg_l(op_src->reg) & 0x0000ffff;
	}
	else
		cpu.eip = reg_l(op_src->reg);
	print_asm_template1();
	return 0;
}



#include "cpu/exec/template-end.h"
