#include "common.h"
#include "nemu.h"
#include <stdlib.h>
#include <elf.h>
#include "monitor/elf.h"

#define SIZE 32

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

static PartOfStackFrame statab[SIZE];
static int func_info[SIZE][3];	//fun_info[n][0]: start addr; fun_info[n][1]: end addr: fun _info[n][2]: nr in symtab
static int nr_func = 0;	

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[sizeof(Elf32_Ehdr)];
	ret = fread(buf, sizeof(Elf32_Ehdr), 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
	 			strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
 		}
		else if(sh[i].sh_type == SHT_STRTAB && 
	 			strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
	 	}
 	}

	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

uint32_t search_elf_obj(char *objName, bool *success)
{
	int i=0;
	//	printf("execFile:: %s\n", exec_file);
	//	printf("objName:: %s\n", objName);
	for (; i < nr_symtab_entry; ++i){
	//	printf("Name:: %d\n", symtab[i].st_name);
	//	printf("strtab:: %s\n", strtab+symtab[i].st_name);
		if (symtab[i].st_info == 17 && strcmp(objName, strtab+symtab[i].st_name) == 0){
			*success = true;
			return symtab[i].st_value;
		}
	}	
	*success = false;
	return 0;
}

static void load_func_info()
{
	int i = 0;
	nr_func = 0;
	for (; i < nr_symtab_entry; ++i){
		if (symtab[i].st_info == 18){
			func_info[nr_func][2] = i;
			func_info[nr_func][0] = symtab[i].st_value;
			func_info[nr_func][1] = func_info[nr_func][0] + symtab[i].st_size;		
			nr_func ++;
		}
	}	
}

int is_func(swaddr_t addr)
{
	int i=0;	
	for ( ; i < nr_func; i++){
		if(addr >= func_info[i][0] && addr <= func_info[i][1])
			return i;
	}
	return -1;
}

static void load_stack_info()
{
	int i=0, j=0;
	uint32_t ebp=cpu.ebp;

	for(i=0; ebp != 0; ++i){ 
			printf("ebp %d:%x\n", i, ebp);
			statab[i].ret_addr = swaddr_read(ebp+4, 4);
			statab[i].prev_ebp = swaddr_read(ebp, 4);
			for(j=0; j < 4; ++j)
				statab[i].args[j] = swaddr_read(ebp+8+i*4, 4);
			ebp = statab[i].prev_ebp;	
	}
}

void print_stack_info()
{
	int i=0;
	int func=-1;
	load_stack_info();
	load_func_info();	
	printf("print_stack_info\n");
	while(statab[i].prev_ebp !=  0){
		if((func=is_func(statab[i].ret_addr) != -1)){
			printf("prev_ebp: %x\tret_addr: %x\t", statab[i].prev_ebp, statab[i].ret_addr);
			printf("#%d\t0x%x  in  %s  \n", i, func_info[func][0], strtab+symtab[func_info[i][2]].st_name);
		}
		i++;
	}
}
