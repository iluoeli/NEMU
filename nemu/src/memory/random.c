#include "random.h"

uint32_t random = seed;

uint32_t randomGenerator()
{
	random = (random * factor);
	return random % mod;
}
