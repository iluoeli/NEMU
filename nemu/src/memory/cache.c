#include "common.h"
#include "misc.h"
#include "burst.h"
#include "random.h"


#define CACHE_WIDTH (6 + 10)
#define BLOCK_WIDTH 6
#define WAY_WIDTH 3
#define SET_WIDTH (CACHE_WIDTH - BLOCK_WIDTH - WAY_WIDTH)  

#define BLOCK_SIZE (1 << BLOCK_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)

#define CACHE_MASK (BLOCK_SIZE - 1)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef union{
	struct {
		uint32_t block	: BLOCK_WIDTH;
		uint32_t set	: SET_WIDTH; 
		uint32_t tag	: (27 - BLOCK_WIDTH - SET_WIDTH);
	};
	uint32_t addr;
} cache_addr;


typedef struct{
	bool valid;	
//	bool dirty;
	uint32_t tag;
	uint8_t set;
	uint8_t data[BLOCK_SIZE];
} CacheSlot;

CacheSlot cache[NR_SET][NR_WAY];

uint32_t cache_read(hwaddr_t addr, size_t len)
{
//   Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr;
	uint32_t block = temp.block;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	bool hit = false;
	int i=0;
	for (; i < NR_WAY; ++i){
		if(cache[set][i].valid && cache[set][i].tag == tag) {
			hit = true;
			return cache[set][i].data[block];	
		}	
	}

	uint32_t random = randomGenerator() % NR_WAY;
	if(!hit) {
		int j=0;
		uint32_t addr_block = addr & ~CACHE_MASK;
		for (; 4*j < BLOCK_SIZE; ++j) {
			*((uint32_t *)cache[set][random].data + 4*j) = dram_read(addr_block + 4*j, 4);
		}	
		//cache[set][random].data = dram_read(addr_block , BLOCK_SIZE);
		//memcpy(cache[set][random].data, dram_read(addr_block, BLOCK_SIZE), BLOCK_SIZE);
		cache[set][random].tag = tag;
		cache[set][random].valid = true;
	}
	return *((uint32_t *)cache[set][random].data + block);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data)
{
	cache_addr temp;
	temp.addr = addr;
	uint32_t block = temp.block;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	bool hit = false;
	int i = 0;
	for (; i < NR_WAY; ++i){
		if(cache[set][i].valid && cache[set][i].tag == tag){
			hit = true;
			//write through
			*((uint32_t *)cache[set][i].data + block) = data;			
			dram_write(addr, len, data);
		}	
	}
	
	if(!hit){
		//not write allocate
		dram_write(addr, len, data);
		
	}
}

