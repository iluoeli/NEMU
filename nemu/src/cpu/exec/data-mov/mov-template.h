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
	uint8_t cr_reg = instr_fetch(cpu.eip+2, 1);
	uint8_t reg = cr_reg & 0x07;
	cr_reg &= 0x38;
	cr_reg = cr_reg >> 3;
	if(cr_reg == 0) {
		OPERAND_W(op_src, cpu.CR0._32);
		print_asm("movl" " CR0,%%%s", REG_NAME(reg));
	}
 	else if(cr_reg == 3) {
		OPERAND_W(op_src, cpu.CR3.val);	
		print_asm("movl" " CR3,%%%s", REG_NAME(reg));
	}
	return len+2;
}

void init_TLB();
make_helper(mov_r2cr)
{
	int len = decode_r_l(cpu.eip+2);	
	uint8_t cr_reg = instr_fetch(cpu.eip+2, 1);
	uint8_t reg = cr_reg & 0x07;
	cr_reg &= 0x38;
	cr_reg = cr_reg >> 3;
 	if(cr_reg == 0){
		cpu.CR0._32 = REG(R_EAX);
		print_asm("movl" " %%%s, CR0", REG_NAME(reg));
	}
	else if(cr_reg == 3){
		cpu.CR3.val = REG(R_EAX);
		//when updata CR3, brush TLB in case of error
		init_TLB();
		print_asm("movl" " %%%s, CR3", REG_NAME(reg));
  	}
//	printf("cpu.eip: %x\n", cpu.eip);
//	printf("mov_r2cr: %x\n", op_src->reg);

	return len+2;
}
#endif




#if DATA_BYTE == 2
typedef union SegmentDescriptor {
	struct {
		uint32_t limit_15_0          : 16;
		uint32_t base_15_0           : 16;
		uint32_t base_23_16          : 8;
		uint32_t type                : 4;
		uint32_t segment_type        : 1;
		uint32_t privilege_level     : 2;
		uint32_t present             : 1;
		uint32_t limit_19_16         : 4;
		uint32_t soft_use            : 1;
		uint32_t operation_size      : 1;
		uint32_t pad0                : 1;
		uint32_t granularity         : 1;
		uint32_t base_31_24          : 8;
	};
	struct {
		uint32_t val_1;
		uint32_t val_2;
	};
} SegDesc;


//opcode 8e; mov rw to segment registers
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

//every time sr changed, cache will  update
	 assert(nr_sr <= 5 && nr_sr >= 0);

	 if(cpu.CR0.PE == 1 && cpu.sr[nr_sr].TI == 0){
		uint32_t gdt_base = cpu.GDTR.base;
		SegDesc gdt;
		uint32_t tmp_addr =  (gdt_base + 8*cpu.sr[nr_sr].index);
		gdt.val_1 = hwaddr_read(tmp_addr, 4);
		gdt.val_2 = hwaddr_read(tmp_addr+4, 4);
		assert(gdt.present == 1);
		cpu.sr[nr_sr].base = (gdt.base_31_24 << 24) + (gdt.base_23_16 << 16) + gdt.base_15_0;
		cpu.sr[nr_sr].limit = (gdt.limit_19_16 << 16) + gdt.limit_15_0;	 
		cpu.sr[nr_sr].DPL = gdt.privilege_level;
	}	

	//print_asm("movw" " %%%s, sr[%d]", REG_NAME(nr_gpr), nr_sr);
	print_asm("movw" " %%%s,%s",  REG_NAME(nr_gpr), sr_name);
	return 2;
}

#endif 

#include "cpu/exec/template-end.h"
