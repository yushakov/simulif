/*
 * main.c
 *
 *  Created on: 12 oct 2014
 *      Author: Yury Ushakov (yushakov.com)
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include "rk4.h"

#include "sim_system.h"
#include "sys_parser.h"

#ifndef NULL
  #define NULL 0
#endif

static char version[] = "1.1.5";

#define PRINT(fp, str, ...) fprintf(fp, str, __VA_ARGS__); printf(str, __VA_ARGS__)


/*
 *
 */
char *dummy_argv[] = {
		"simulif.exe",
		"-t", "70.0",
		"-s", "10000",
		      //IC      mu    sqrtD   rst   spk  thr  fnm     A       Omg    Phi          wgt    num
		"-n", "0.001", "0.5", "4",    "-1", "5", "1", "cos", "0.98", "0.5", "0.0", "-k", "0.33", "7",
		"-n", "0.002", "0.5", "4",    "-1", "5", "1", "cos", "0.75", "0.6", "0.1", "-k", "0.2",  "9", "-k", "-0.37", "3"};
int main(int argc, char *argv[])
{
	Neuron *neurons = NULL;
	double *x       = NULL;
	double *spikes  = NULL;
	double *inpVec  = NULL;
	int     order   = 0;

	Link   *pK;
	Neuron *pN;
	IniCon *pIC;
	int i;
	double   dot_interval, next_dot;
	SysParam system_parameters;
	int      skip_points = 1000;
	double   calc_time   = 5.0;

	double t     = 0.0;
	double tprev = 0.0;
	int    cnt   = 0;
	FILE  *fp = fopen("output.txt", "w");

	if(argc < 2)
	{
		argc = 37;
		argv = dummy_argv;
		printf("Version: %s\n", version);
	}

	fprintf(fp, "# Version: %s\n\n", version);

	system_parameters.neuron       = NULL;
	system_parameters.spikes       = NULL;
	system_parameters.ini_cons     = NULL;
	system_parameters.input_vector = NULL;
	system_parameters.lstIniCons   = NULL;

	parseArguments(argc, argv, &system_parameters);
	calc_time          = system_parameters.calc_time;
	skip_points        = system_parameters.skip_points;
	neurons            = system_parameters.neuron;

	fprintf(fp, "# Calculation time: %f\n\n", calc_time);
	fprintf(fp, "# Skipping output points: %d\n", skip_points);

	pN = neurons;
	pIC = system_parameters.lstIniCons;
	while(pN != 0)
	{
		PRINT(fp, "\n# neuron:  %d\n", order);
		PRINT(fp, "# Initial:   %f\n", pIC->val);
		PRINT(fp, "# mu:        %f\n", pN->mu);
		PRINT(fp, "# sqrtD:     %f\n", pN->sqrtD);
		PRINT(fp, "# A:         %f\n", pN->Amp);
		PRINT(fp, "# Omega:     %f\n", pN->Omg);
		PRINT(fp, "# Phi:       %f\n", pN->Phi);

		pK = pN->k;
		while(pK != 0)
		{
			PRINT(fp, "# link from %d: %f\n", (int)pK->intNum, pK->weight);
			pK = pK->nxtLink;
		}

		pN  = pN->nxtNeuron;
		pIC = pIC->nxtIniCon;
		order++;
	}
	fflush(fp);

	// allocate spikes array
	spikes = (double*)calloc(order, sizeof(double));
	inpVec = (double*)calloc(order, sizeof(double));

	// set initial conditions to dynamic variables
	x   = (double*)calloc(order, sizeof(double));
	i   = -1;
	pIC = system_parameters.lstIniCons;
	while(pIC != 0)
	{
		x[++i] = pIC->val;
		pIC    = pIC->nxtIniCon;
	}
	freeIniCons(system_parameters.lstIniCons);

	system_parameters.spikes       = spikes;
	system_parameters.input_vector = inpVec;
	system_parameters.ini_cons     = x;

	// calculation cycle
	rk4_init(order);

	dot_interval = calc_time / 50.0;
	next_dot     = dot_interval;
	printf("Calculation time: %f\n", calc_time);
	printf("..................................................\n");
	while(t <= calc_time)
	{
		system_parameters.spike_flag = 0;
		rk4_calc(x, (void*)(&system_parameters), STEP, t, sys_function);
		t += STEP;

		int j = 0;
		if(cnt < skip_points && system_parameters.spike_flag == 0) cnt++;
		else
		{
			if(system_parameters.spike_flag == 0)
			{
				fprintf(fp, "%14.7f", t);
				for(j = 0; j < order; j++) fprintf(fp, ", %14.7f", x[j]);
				fprintf(fp, "\n");
			}
			else
			{
				fprintf(fp, "%14.7f", t);
				pN = neurons;
				for(j = 0; j < order; j++)
				{
					if(spikes[j] > 0.0) fprintf(fp, ", %14.7f", pN->spk_level);
					else                fprintf(fp, ", %14.7f", x[j]);
					pN = pN->nxtNeuron;
				}
				fprintf(fp, "\n");
			}
			cnt = 0;

			if(t > next_dot)
			{
				printf(".");
				fflush(0);
				next_dot += dot_interval;
			}
		}
	}
	rk4_destroy();
	fclose(fp);
	printf("\ndone\n");

	// free memory
	freeNeurons(neurons);
	free(x);
	free(spikes);
	free(inpVec);

	return 0;
}
