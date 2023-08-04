#pragma once
#include <stdlib.h>

#define Random(x) Random::RandomizeByte(sizeof(x), (char *) &(x))

class Random
{
public:
	static void SetSeed(unsigned int seed);
	static void RandomizeByte(size_t size, char * loc);
	static void RemoveSeed();
};

