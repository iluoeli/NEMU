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
	cpu.eip -= 1;
	print_asm_template1();
	return 1;
}


#if DATA_BYTE == 4
make_helper(jmp_ptrw_l)
{
	uint32_t addr_eip = instr_fetch(cpu.eip+1, 4);
	uint16_t addr_cs = instr_fetch(cpu.eip+5, 2);
	cpu.eip = addr_eip - 7;
	cpu.sr[1].selector = addr_cs;
	
	print_asm("jmp %x:%x", addr_cs,addr_eip);
	return 7;	
}

#endif

#include "cpu/exec/template-end.h"
