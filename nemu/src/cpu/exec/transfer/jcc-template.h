#include "cpu/exec/template-start.h"

#define instr jne
make_helper(concat(jne_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.ZF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr

#define instr je
make_helper(concat(je_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.ZF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr ja
make_helper(concat(ja_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.ZF == 0 && cpu.EFLAGES.CF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jae
make_helper(concat(jae_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.CF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jb
make_helper(concat(jb_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.CF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jbe
make_helper(concat(jbe_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.CF == 1 || cpu.EFLAGES.ZF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jg
make_helper(concat(jg_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.ZF == 0 || cpu.EFLAGES.SF == cpu.EFLAGES.OF)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jge
make_helper(concat(jge_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.SF == cpu.EFLAGES.OF)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jl
make_helper(concat(jl_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.SF != cpu.EFLAGES.OF)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jle
make_helper(concat(jle_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.ZF == 1 || cpu.EFLAGES.SF != cpu.EFLAGES.OF)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jno
make_helper(concat(jno_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.OF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jnp
make_helper(concat(jnp_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.PF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jns
make_helper(concat(jns_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.SF == 0)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jo
make_helper(concat(jo_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.OF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr jp
make_helper(concat(jp_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.PF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#define instr js
make_helper(concat(js_i_, SUFFIX))
{
	int len = concat(decode_i_, SUFFIX)(eip+1);
	if(cpu.EFLAGES.SF == 1)
		cpu.eip += (DATA_TYPE_S)op_src->imm;	
	print_asm_template1();
	return 1+len;
}
#undef instr 

#include "cpu/exec/template-end.h"
