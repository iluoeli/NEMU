#include "cpu/exec/helper.h"

void raise_intr(uint8_t NO);

make_helper(int_i_b)
{
//	printf("int, eip:%x\n", cpu.eip);
	int len = decode_i_b(cpu.eip+1);
	//real-address-mode
	//push eflages, push cs, push eip
//	assert(cpu.CR0.PE == 0);
/*	cpu.esp -= 4;	
	swaddr_write(cpu.esp, 4, cpu.EFLAGES.eflages, 1);

	cpu.EFLAGES.IF = 0;
	cpu.EFLAGES.TF = 0;

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 2, cpu.CS.selector, 1);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip+len+1, 1);
*/
//	printf("int 0x%x", op_src->val);

//warning: should I add this ?
//	cpu.eip += len+1;

	print_asm("int 0x%x", op_src->val);

	//then jmp eroor process
//	cpu.eip -= (len + 1);
	raise_intr(op_src->val);

//	cpu.eip -= (len+1);

	return len+1;	
}

make_helper(iret)
{
//	printf("iret\n");
	//pop %eip
	cpu.eip = swaddr_read(cpu.esp, 4, 1)-1;
	cpu.esp += 4;
	
	cpu.CS.selector = swaddr_read(cpu.esp, 2, 1);
	cpu.esp += 4;

	cpu.EFLAGES.eflages = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;

	print_asm("iret");

	return 1;		
}

make_helper(cli)
{
//	printf("cli\n");
	cpu.EFLAGES.IF = 0;	

	print_asm("cli");

	return 1;
}

make_helper(sti)
{
	cpu.EFLAGES.IF = 1;

	print_asm("sti");

	return 1;	
}

make_helper(pusha)
{
//	printf("pusha\n");
	uint32_t temp = cpu.esp;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eax, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ecx, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edx, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebx, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, temp, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebp, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.esi, 1);	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edi, 1);	

	print_asm("pusha");

	return 1;
}

make_helper(popa)
{
//	printf("popa\n");
	cpu.edi = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	cpu.esi = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	cpu.ebp = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	//thorow away
	cpu.esp += 4;		
	cpu.ebx = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	cpu.edx = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	cpu.ecx = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		
	cpu.eax = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;		

	print_asm("popa");
	return 1;
}


