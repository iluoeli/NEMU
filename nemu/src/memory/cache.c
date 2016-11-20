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
#define NR_BLOCK (1 << (WAY_WIDTH + SET_WIDTH))

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
	uint32_t set;
	uint8_t data[BLOCK_SIZE];
} CacheSlot;

CacheSlot cache[NR_SET][NR_WAY];

void init_cache()
{
	int i = 0;
	int j = 0;
	for (; i < NR_SET; ++i){
		for (j=0; j < NR_WAY; ++j)
			cache[i][j].valid = false;
	}	
}

uint32_t cache_read(hwaddr_t addr, size_t len)
{
//   Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr;
	uint32_t block = temp.block;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	bool hit = false;
	uint32_t buf[2];
	int i=0;
	for (; i < NR_WAY; ++i){
		if(cache[set][i].valid && cache[set][i].tag == tag) {
			hit = true;
			break;
		}	
	}

	if(!hit) {
		uint32_t random = randomGenerator() % NR_WAY;
		i = random;
		int j=0;
		uint32_t addr_block = addr & (~0u & ~(BLOCK_SIZE -1));
		for (; j < BLOCK_SIZE; ++j) {
			cache[set][i].data[j] = dram_read(addr_block + j, 1);
		}	
		cache[set][i].tag = tag;
		cache[set][i].valid = true;
	}
	memset(buf, 0, 8);
	uint32_t offset = block & 3;
	block = block & (~3);
	buf[0] = *(uint32_t *)(cache[set][i].data + block);
	//if cross block
	if((temp.block + len) > BLOCK_SIZE) 
		buf[1] = cache_read((addr & ~3) + 4, 4);
	else 
		buf[1] = *(uint32_t *)(cache[set][i].data + block+4);
	return unalign_rw((uint8_t *)buf + offset, 4);
//	return unalign_rw(cache[set][random].data + block, 4);
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
			int j=0;
			for (; j < len; ++j) {
				cache[set][i].data[block+j] = (data >> (8 * j)) & 0xff;
			}
			//*((uint32_t *)cache[set][i].data + block) = data;			
			dram_write(addr, len, data);
		}	
	}
	
	if(!hit){
		//not write allocate
		dram_write(addr, len, data);
	}
}


