#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef union {
	struct {
		uint32_t CF:1;
		uint32_t :1;
		uint32_t PF:1;
		uint32_t :1;
		uint32_t AF:1;
		uint32_t :1;
		uint32_t ZF:1;
		uint32_t SF:1;
		uint32_t TF:1;
		uint32_t IF:1;
		uint32_t DF:1;
		uint32_t OF:1;
		uint32_t :2;
		uint32_t NT:1;
		uint32_t :1;
		uint32_t RF:1;
		uint32_t VM:1;
	};
	uint32_t eflages;
}Eflages;

typedef struct{
	uint16_t limit;	
	uint32_t base;
} SEG_GDTR;

typedef union{
	struct {
		uint32_t PE:	1;	
		uint32_t MP:	1;	
		uint32_t EM:	1;	
		uint32_t TS:	1;	
		uint32_t ET:	1;	
		uint32_t RESERVED:	26;	
		uint32_t PG:	1;	
	};
	uint32_t _32;
}SEG_CR0;
	


typedef union {
	struct {
		uint32_t _32;
		uint16_t selector;
	};
	struct {
		uint32_t cache:	32;
		uint16_t RPL:	2;
		uint16_t TI:	1;
		uint16_t index:	13;		
	};
}SEG_SR;



typedef	struct {
	//General Registers define here
	union {
 	 	union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
  			} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
   		 struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	
	union{
		SEG_SR sr[6];
		struct {
			SEG_SR ES;		
			SEG_SR CS;		
			SEG_SR SS;		
			SEG_SR DS;		
			SEG_SR FS;		
			SEG_SR GS;		
		};	
	};

	Eflages EFLAGES;	
	SEG_GDTR GDTR;
	SEG_CR0 CR0;
/*
	union{
		struct {
			uint32_t PE:	1;	
			uint32_t MP:	1;	
			uint32_t EM:	1;	
			uint32_t TS:	1;	
			uint32_t ET:	1;	
			uint32_t RESERVED:	26;	
			uint32_t PG:	1;	
		};
		uint32_t CR0;
	};
*/

	//EIP
	swaddr_t eip;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
