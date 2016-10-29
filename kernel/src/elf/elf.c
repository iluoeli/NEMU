#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
void ramdisk_write(uint8_t *buf, uint32_t offset, uint32_t len);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);

#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
//	panic("please implement me");
	int i;
	uint8_t buf_ph[elf->e_phentsize];
	for(i=0; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
		ramdisk_read(buf_ph, ELF_OFFSET_IN_DISK+elf->e_phoff+i*elf->e_phentsize, elf->e_phentsize);
		ph = (void *)buf_ph;
 		if(ph->p_type == PT_LOAD) {

 			/* TO DO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
 			 */
			uint8_t buf_file[ph->p_filesz];
			ramdisk_read(buf_file, ELF_OFFSET_IN_DISK+ph->p_offset, ph->p_filesz);	
			ramdisk_write(buf_file, ph->p_vaddr, ph->p_filesz);	
			 
  			/* TOD O: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			int margin = ph->p_memsz-ph->p_filesz;
			uint8_t buf_zero[margin];	
			int j;
			for (j=0; j < margin; ++j)
				buf_zero[j] = 0;
			ramdisk_write(buf_zero, ph->p_vaddr+ph->p_filesz, margin);

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
 		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
