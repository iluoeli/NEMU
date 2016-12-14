#include "cpu/exec/helper.h"

void raise_intr(uint8_t NO);

make_helper(int_i_b)
{
	int len = decode_i_b(cpu.eip+1);
	//real-address-mode
	//push eflages, push cs, push eip
	assert(cpu.CR0.PE == 0);
	cpu.esp -= 4;	
	swaddr_write(cpu.esp, 4, cpu.EFLAGES.eflages, 1);

	cpu.EFLAGES.IF = 0;
	cpu.EFLAGES.TF = 0;

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 2, cpu.CS.selector, 1);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 2, cpu.eip+len+1, 1);

	//then jmp eroor process
	raise_intr(op_src->val);

	return len+1;	
}

make_helper(iret)
{
	//pop %eip
	cpu.eip = swaddr_read(cpu.esp, 4, 1)-1;
	cpu.esp += 4;
	
	cpu.CS.selector = swaddr_read(cpu.esp, 2, 1);
	cpu.esp += 4;

	cpu.EFLAGES.eflages = swaddr_read(cpu.esp, 4, 1);
	cpu.esp += 4;

	return 1;		
}

make_helper(cli)
{
	cpu.EFLAGES.IF = 0;	
	return 1;
}

make_helper(pusha)
{
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

	return 1;
}

