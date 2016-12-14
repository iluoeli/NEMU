#include <setjmp.h>
#include <common.h>
#include "nemu.h"
#include "common.h"
#include "mmu.h"

extern jmp_buf jbuf;
/*
typedef union GateDescriptor{
 	struct {
		uint32_t offset_15_0      : 16;
		uint32_t segment          : 16;
		uint32_t pad0             : 8;
		uint32_t type             : 4;
		uint32_t system           : 1;
		uint32_t privilege_level  : 2;
		uint32_t present          : 1;
		uint32_t offset_31_16     : 16;
 	};
   	struct {
		uint32_t val_l;
		uint32_t val_h;	
	};
} GateDesc;
*/


void raise_intr(uint8_t NO)	
{
/* TODO: Trigger an interrupt/exception with 'NO'
 *That is, use 'NO' to index the IDT.
*/
	printf("raise_intr\n");
	uint32_t idt_addr = cpu.IDTR.base + 8 * NO;
	printf("idt_base: %x  ,idt_addr: %x\n", cpu.IDTR.base, idt_addr);
	GateDesc idt;
	idt.val_l = swaddr_read(idt_addr, 4, 1);
	idt.val_h = swaddr_read(idt_addr+4, 4, 1);
	assert(idt.present == 1);

	cpu.CS.selector = idt.segment;
	


	cpu.eip = (idt.offset_31_16 << 16) + idt.offset_15_0;
	printf("cpu.eip:%x\n", cpu.eip);
//	cpu.eip -= 2;
/*Jump back to cpu_exec() */	
	longjmp(jbuf, 1);
}
