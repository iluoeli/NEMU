#ifndef __MYELF_H__
#define __MYELF_H__

#include "common.h"

typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
} PartOfStackFrame;


uint32_t search_elf_obj(char *, bool *);
void print_stack_info();

#endif
