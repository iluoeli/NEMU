#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip+ 1, 4);
	MEM_W(addr, REG(R_EAX), 3);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, 3);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4
make_helper(mov_cr2r)
{
	int len = decode_r_l(cpu.eip+2);	
	OPERAND_W(op_src, cpu.CR0._32);

	print_asm("movl" " CR0,%%%s", REG_NAME(op_src->reg));
	return len+2;
}

make_helper(mov_r2cr)
{
	int len = decode_r_l(cpu.eip+2);	
	cpu.CR0._32 = REG(R_EAX);
//	printf("cpu.eip: %x\n", cpu.eip);
//	printf("mov_r2cr: %x\n", op_src->reg);

	print_asm("movl" " %%%s, CR0", REG_NAME(op_src->reg));
	return len+2;
}
#endif

//opcode 8e; mov rw to segment registers
#if DATA_BYTE == 2
make_helper(mov_r2sr_w)
{
	uint8_t modRM = instr_fetch(eip+1, 1);
	uint8_t nr_sr = (modRM & 0x38) >> 3;
	uint8_t nr_gpr = (modRM & 0x07);
	cpu.sr[nr_sr].selector = cpu.gpr[nr_gpr]._16;	
	char sr_name[3];
	sr_name[2] = '\0';
	sr_name[1] = 'S';
	switch(nr_sr){
		case 0:	sr_name[0] = 'E';	break;
		case 1:	sr_name[0] = 'C';	break;
		case 2:	sr_name[0] = 'S';	break;
		case 3:	sr_name[0] = 'D';	break;
		case 4:	sr_name[0] = 'F';	break;
		case 5:	sr_name[0] = 'G';	break;
		default :sr_name[0] = 'E';
				sr_name[1] = 'R';
	}
	

	//print_asm("movw" " %%%s, sr[%d]", REG_NAME(nr_gpr), nr_sr);
	print_asm("movw" " %%%s,%s",  REG_NAME(nr_gpr), sr_name);
	return 2;
}

#endif 

#include "cpu/exec/template-end.h"
