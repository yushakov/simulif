#include <stdio.h>
#include "../simulif/spike.h"

int main()
{
	int i;
	SpikeQ spikeQ;
	Spike  s, sout;
	spikeQ_new(5, &spikeQ);

	for (i = 0; i < 100; i += 2)
	{
		s.height    = (double)i;
		s.spikeTime = (double)i;
		spikeQ_push(s, &spikeQ);

		s.height = (double)(i+1);
		s.spikeTime = (double)(i+1);
		spikeQ_push(s, &spikeQ);

		spikeQ_first(&sout, &spikeQ);
		printf("first height, time: %f, %f\n", sout.height, sout.spikeTime);

		sout.height = -1.0;
		sout.spikeTime = -1.0;

		spikeQ_pop(&sout, &spikeQ);
		printf("popped height, time: %f, %f\n", sout.height, sout.spikeTime);
		printf("size: %d\n\n", spikeQ.size);
	}

	while (spikeQ_pop(&sout, &spikeQ) >= 0)
	{		
		printf("popped height, time: %f, %f\n", sout.height, sout.spikeTime);
	}

	spikeQ_destroy(&spikeQ);

	return 0;
}