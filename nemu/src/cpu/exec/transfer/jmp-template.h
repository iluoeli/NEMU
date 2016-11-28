#include "cpu/exec/template-start.h"

#define instr jmp


//jmp direct 
static void do_execute()
{
	cpu.eip += (DATA_TYPE_S)op_src->val;
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
		cpu.eip = op_src->val & 0x0000ffff;
	}
	else
		cpu.eip = op_src->val;
	print_asm_template1();
	return 0;
}


#if DATA_BYTE == 4
make_helper(jmp_ptrw_l)
{
	uint32_t addr_cs = instr_fetch(cpu.eip+1, 2);
	uint16_t addr_eip = instr_fetch(cpu.eip+3, 4);
	cpu.eip = addr_eip;
	cpu.sr[1].selector = addr_cs;
	
	return 7;	
}

#endif

#include "cpu/exec/template-end.h"
