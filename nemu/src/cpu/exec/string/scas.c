#include "cpu/exec/helper.h"


make_helper(scas_b)
{
	uint8_t dest;
	uint8_t al = reg_b(R_AL);
	uint16_t result;
	dest = swaddr_read(reg_l(R_EDI), 1);
	result = al - dest;
	cpu.EFLAGES.OF =  ( ((!dest)>>7 & 1) == (al>>7 & 1) && ((al>>7 & 1) != (result>>7 & 1)));
	cpu.EFLAGES.SF = (result>>7 & 1);
	cpu.EFLAGES.ZF = ((uint8_t)result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	//cpu.EFLAGES.CF = (result>>8 & 1) ^ 1;
	cpu.EFLAGES.CF = (al < dest);

	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI)++;
	else
		reg_l(R_EDI)--;

	print_asm("scasb");
	return 1;
}


make_helper(scas_w)
{
	uint16_t dest;
	uint16_t ax = reg_w(R_AX);
	uint32_t result;
	dest = swaddr_read(reg_l(R_EDI), 2);
	result = ax - dest;
	cpu.EFLAGES.OF =  ( ((!dest)>>15 & 1) == (ax>>15 & 1) && ((ax>>15 & 1) != (result>>15 & 1)));
	cpu.EFLAGES.SF = (result>>15 & 1);
	cpu.EFLAGES.ZF = ((uint16_t)result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (result>>16 & 1) ^ 1;

	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI) += 2;
	else
		reg_l(R_EDI) -= 2;

	print_asm("scasw");
	return 1;
}


make_helper(scas_l)
{
	uint32_t dest;
	uint32_t eax = reg_l(R_EAX);
	uint64_t result;
	dest = swaddr_read(reg_l(R_EDI), 4);
	result = eax - dest;
	cpu.EFLAGES.OF =  ( ((!dest)>>31 & 1) == (eax>>31 & 1) && ((eax>>31 & 1) != (result>>31 & 1)));
	cpu.EFLAGES.SF = (result>>31 & 1);
	cpu.EFLAGES.ZF = ((uint32_t)result == 0);
	uint8_t ret = (result>>4) ^ result;
	ret = (ret>>2) ^ ret;
	ret = (ret>>1) ^ ret;
	cpu.EFLAGES.PF = ret & 1;
	cpu.EFLAGES.CF = (result>>32 & 1) ^ 1;

	if(cpu.EFLAGES.DF == 0)
		reg_l(R_EDI) += 4;
	else
		reg_l(R_EDI) -= 4;

	print_asm("scasl");
	return 1;
}
make_helper(scas_v)
{
	return (ops_decoded.is_operand_size_16? scas_w:scas_l)(eip);	
}
