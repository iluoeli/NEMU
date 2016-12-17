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
//	printf("raise_intr\n");
	uint32_t idt_addr = cpu.IDTR.base + 8 * NO;
//	printf("idt_base: %x  ,idt_addr: %x\n", cpu.IDTR.base, idt_addr);
	GateDesc idt;
	idt.val_l = swaddr_read(idt_addr, 4, 1);
	idt.val_h = swaddr_read(idt_addr+4, 4, 1);
	assert(idt.present == 1);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGES.eflages, 1);
	cpu.EFLAGES.IF = 0;
	cpu.EFLAGES.TF = 0;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 2, cpu.CS.selector, 1);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, 1);

	cpu.CS.selector = idt.segment;
	//updata cs cache	
 	if(cpu.CR0.PE == 1 && cpu.CS.TI == 0){
		uint32_t gdt_base = cpu.GDTR.base;
		SegDesc gdt;
		uint32_t tmp_addr =  (gdt_base + 8*cpu.CS.index);
		gdt.val_l = lnaddr_read(tmp_addr, 4);
		gdt.val_h = lnaddr_read(tmp_addr+4, 4);
		assert(gdt.present == 1);
		cpu.CS.base = (gdt.base_31_24 << 24) + (gdt.base_23_16 << 16)     + gdt.base_15_0;
		cpu.CS.limit = (gdt.limit_19_16 << 16) + gdt.limit_15_0;
		cpu.CS.DPL = gdt.privilege_level;
 	}

	cpu.eip = (idt.offset_31_16 << 16) + idt.offset_15_0;
//	printf("cpu.eip:%x\n", cpu.eip);
//	cpu.eip -= 2;
/*Jump back to cpu_exec() */	
	longjmp(jbuf, 1);
}
