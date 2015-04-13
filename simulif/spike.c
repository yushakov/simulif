#include "spike.h"
#include <malloc.h>

int spikeQ_new(int size, SpikeQ *sq)
{
	sq->q = (Spike*)calloc(size, sizeof(Spike));

	return 0;
}

int spikeQ_destroy(SpikeQ *sq)
{
	if (sq->q)
	{
		free(sq->q);
	}
	return 0;
}

int spikeQ_push(Spike s, SpikeQ *sq)
{
	return 0;
}

int spikeQ_pop(Spike *s, SpikeQ *sq)
{
	return 0;
}

int spikeQ_last(Spike *s, SpikeQ *sq)
{
	return 0;
}