#include "Genome.h"

Genome::Genome()
{
	gene = "";
	weight = 0;
}

Genome::Genome(string gene) : gene(gene)
{
	weight = 0;
}

Genome::~Genome()
{
}


