#include "World/WorldRandom.h"


void Random::SetSeed(unsigned int seed)
{
	srand(seed);
}

void Random::RandomizeByte(size_t size, char * loc) {
	char* p;
	for (p = loc; p < loc + size; p++)
		*p = (char)(rand()%256);
}

void Random::RemoveSeed()
{
	srand(1);
}
