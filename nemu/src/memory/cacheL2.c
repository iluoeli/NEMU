#include "common.h"
#include "misc.h"
#include "burst.h"
#include "random.h"


#define cacheL2_WIDTH (2 + 10 + 10)	//4MB
#define BLOCK_WIDTH 6				//64B
#define WAY_WIDTH 4					//16-way
#define SET_WIDTH (cacheL2_WIDTH - BLOCK_WIDTH - WAY_WIDTH)  

#define BLOCK_SIZE (1 << BLOCK_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_BLOCK (1 << (WAY_WIDTH + SET_WIDTH))

#define cacheL2_MASK (BLOCK_SIZE - 1)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef union{
	struct {
		uint32_t block	: BLOCK_WIDTH;
		uint32_t set	: SET_WIDTH; 
		uint32_t tag	: (27 - BLOCK_WIDTH - SET_WIDTH);
	};
	uint32_t addr;
} cacheL2_addr;


typedef struct{
	bool valid;	
	bool dirty;
	uint32_t tag;
	uint32_t set;
	uint8_t data[BLOCK_SIZE];
} cacheL2Slot;

cacheL2Slot cacheL2[NR_SET][NR_WAY];

void init_cacheL2()
{
	int i = 0;
	int j = 0;
	for (; i < NR_SET; ++i){
		for (j=0; j < NR_WAY; ++j){
			cacheL2[i][j].valid = false;
			cacheL2[i][j].dirty = false;
		}
 	}	
}

uint32_t cacheL2_read(hwaddr_t addr, size_t len)
{
//   Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cacheL2_addr temp;
	uint32_t offset = addr & 3;
	temp.addr = addr & (~3);
	uint32_t block = temp.block;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	bool hit = false;
	uint32_t buf[2];
	int i=0;
	for (; i < NR_WAY; ++i){
		if(cacheL2[set][i].valid && cacheL2[set][i].tag == tag) {
			hit = true;
			break;
		}	
	}

	if(!hit) {
		uint32_t random = randomGenerator() % NR_WAY;
		i = random;
		int j=0;
		uint32_t addr_block = addr & (~0u & ~(BLOCK_SIZE -1));

		//write back
		if(cacheL2[set][i].valid && cacheL2[set][i].dirty){
			int j=0;
			cacheL2_addr dram_addr;
			dram_addr.tag = cacheL2[set][i].tag;
			dram_addr.set = set;
			dram_addr.block = 0;
			for (; j < BLOCK_SIZE; ++j){
				dram_write(dram_addr.addr + j, cacheL2[set][i].data[j], 1);	
			}
		}

		for (; j < BLOCK_SIZE; ++j) {
			cacheL2[set][i].data[j] = dram_read(addr_block + j, 1);
		}	
		cacheL2[set][i].tag = tag;
		cacheL2[set][i].valid = true;
		cacheL2[set][i].dirty = false;
	}

	memset(buf, 0, 8);
	buf[0] = *(uint32_t *)(cacheL2[set][i].data + block);
	//if cross block
	if((temp.block+ offset + len) > BLOCK_SIZE) 
		buf[1] = cacheL2_read((temp.addr + 4), 4);
	else 
		buf[1] = *(uint32_t *)(cacheL2[set][i].data + block+4);
	return unalign_rw((uint8_t *)buf + offset, 4);
}

void cacheL2_write(hwaddr_t addr, size_t len, uint32_t data)
{
	cacheL2_addr temp;
	temp.addr = addr;
	uint32_t block = temp.block;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	bool hit = false;
	int i = 0;
	for (; i < NR_WAY; ++i){
		if(cacheL2[set][i].valid && cacheL2[set][i].tag == tag){
			hit = true;
			break;
		}	
	}
	
//write allocate
	if(!hit) {
		uint32_t random = randomGenerator() % NR_WAY;
		i = random;
		int j=0;
		uint32_t addr_block = addr & (~0u & ~(BLOCK_SIZE -1));

		//write back
		if(cacheL2[set][i].valid && cacheL2[set][i].dirty){
			int j=0;
			cacheL2_addr dram_addr;
			dram_addr.tag = cacheL2[set][i].tag;
			dram_addr.set = set;
			dram_addr.block = 0;
			for (; j < BLOCK_SIZE; ++j){
				dram_write(dram_addr.addr + j, cacheL2[set][i].data[j], 1);	
			}
		}

		for (; j < BLOCK_SIZE; ++j) {
			cacheL2[set][i].data[j] = dram_read(addr_block + j, 1);
		}	
		cacheL2[set][i].tag = tag;
		cacheL2[set][i].valid = true;
		cacheL2[set][i].dirty = false;
	}
	
//write back only to cacheL2	
	int j=0;
	for (; j < len; ++j){
		if((block + j) > BLOCK_SIZE)	
			cacheL2_write(addr+j, 1, (data >> (4*j))&0xff);
		else
			cacheL2[set][i].data[block+j] = (data >> (4*j)) & 0xff;
	}
	cacheL2[set][i].dirty = true;
}


