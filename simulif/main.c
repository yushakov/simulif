/****************************************************************************
*  main.c                                                                   *
*                                                                           *
*  Created on:   October 12, 2014                                           *
*  Author:       Yury Ushakov                                               *
*  Email:        nomah@list.ru                                              *
*                                                                           *
*                                                                           *
*  Copyright (C) 2015 by Yury Ushakov                                       *
*                                                                           *
*  This file is the part of the program "simulif".                          *
*                                                                           *
*  "simulif" is free software: you can redistribute it and/or modify        *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation, either version 3 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                           *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*  GNU General Public License for more details.                             *
*                                                                           *
*  You should have received a copy of the GNU General Public License        *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*                                                                           *
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include "rk4.h"

#include "sim_system.h"
#include "sys_parser.h"

#ifndef NULL
  #define NULL 0
#endif

static char version[] = "1.1.14";

#define PRINT(fp, str, ...) fprintf(fp, str, __VA_ARGS__); printf(str, __VA_ARGS__)


/*
 *
 */
char *dummy_argv[] = {
		"simulif.exe",
		"-t", "70.0",
		"-s", "10000",
		      //IC      mu    sqrtD   rst   spk  thr         fnm     A       Omg    Phi          wgt    num
			  "-n", "0.001", "0.5", "4", "-1", "5", "1", "-k", "0.33", "1", "0.1",
		//"-n", "0.001", "0.5", "4",    "-1", "5", "1", "-f", "cos", "0.98", "0.5", "0.0", "-k", "0.33", "7",
		//"-n", "0.002", "0.5", "4",    "-1", "5", "1", "-f", "cos", "0.75", "0.6", "0.1", "-k", "0.2",  "9", "-k", "-0.37", "3"
		"-n", "0.002", "0.5", "4", "-1", "5", "1", "-f", "cos", "1.2", "0.6", "0.1"
};
		
int main(int argc, char *argv[])
{
	double *x     = NULL;
	int     order = 0;

	Link   *pK;
	Neuron *pN;
	IniCon *pIC;
	int     i;
	double  dot_interval, next_dot;
	System  system;
	int     skip_points = 1000;
	double  calc_time   = 5.0;

	double t     = 0.0;
	double tprev = 0.0;
	int    cnt   = 0;
	FILE  *fp;
	long long out_cnt = 0LL;
	char out_file_name[500];
	struct tm _time;

	if(argc < 2)
	{
		argc = 28;
		//argc = 33;
		//argc = 38;
		argv = dummy_argv;
		printf("Version: %s\n", version);
	}

	sprintf(out_file_name, "out_%ld.txt", out_cnt);
	fp = fopen(out_file_name, "w");
	PRINT(fp, "# Version: %s\n\n", version);

	system.neuron       = NULL;
	system.spikes       = NULL;
	system.ini_cons     = NULL;
	system.input_vector = NULL;
	system.lstIniCons   = NULL;

	parseArguments(argc, argv, &system);
	calc_time   = system.calc_time;
	skip_points = system.skip_points;

	fprintf(fp, "# Calculation time: %f\n\n", calc_time);
	fprintf(fp, "# Skipping output points: %d\n", skip_points);

	pN  = system.neuron;
	pIC = system.lstIniCons;
	while(pN != 0)
	{
		char str[500];
		PRINT(fp, "\n# neuron:  %d\n", order);
		PRINT(fp, "# Initial:   %f\n", pIC->val);
		PRINT(fp, "# mu:        %f\n", pN->mu);
		PRINT(fp, "# sqrtD:     %f\n", pN->sqrtD);
		PRINT(fp, getInpFncDescription(str, pN), 0);

		pK = pN->k;
		while(pK != 0)
		{
			PRINT(fp, "# link from %d: %f; delay: %f\n", (int)pK->intNum, pK->weight, pK->delay);
			pK = pK->nxtLink;
		}

		pN  = pN->nxtNeuron;
		pIC = pIC->nxtIniCon;
		order++;
	}
	fflush(fp);

	// allocate spikes array
	system.spikes       = (Spike*)calloc(order, sizeof(Spike));
	system.input_vector = (double*)calloc(order, sizeof(double));

	// set initial conditions to dynamic variables
	x   = (double*)calloc(order, sizeof(double));
	i   = -1;
	pIC = system.lstIniCons;
	while(pIC != 0)
	{
		x[++i] = pIC->val;
		pIC    = pIC->nxtIniCon;
	}
	freeIniCons(system.lstIniCons);

	system.ini_cons = x;

	// calculation cycle
	rk4_init(order);
	{
		// initial time output
		time_t now = time(0);
		_time = *localtime(&now);
		fprintf(fp, "# dot %d:%d:%d\n", _time.tm_hour, _time.tm_min, _time.tm_sec);
	}
	dot_interval = calc_time / 50.0;
	next_dot     = dot_interval;
	printf("Calculation time: %f\n", calc_time);
	printf("..................................................\n");
	while(t <= calc_time)
	{
		system.spike_flag = 0;
		rk4_calc(x, (void*)(&system), STEP, t, sys_function);
		t += STEP;

		int j = 0;
		if (cnt < skip_points && system.spike_flag == 0) cnt++;
		else
		{
			if (system.spike_flag == 0)
			{
				fprintf(fp, "%14.7f", t);
				for(j = 0; j < order; j++) fprintf(fp, ", %14.7f", x[j]);
				fprintf(fp, "\n");
			}
			else
			{
				fprintf(fp, "%14.7f", t);
				pN = system.neuron;
				for(j = 0; j < order; j++)
				{
					if(system.spikes[j].height > 0.0) fprintf(fp, ", %14.7f", pN->spk_level);
					else                fprintf(fp, ", %14.7f", x[j]);
					pN = pN->nxtNeuron;
				}
				fprintf(fp, "\n");
			}
			cnt = 0;

			if(t > next_dot)
			{
				time_t now = time(0);
				_time = *localtime(&now);
				fclose(fp);
				sprintf(out_file_name, "out_%ld.txt", ++out_cnt);
				fp = fopen(out_file_name, "w");
				fprintf(fp, "# dot %d:%d:%d\n", _time.tm_hour, _time.tm_min, _time.tm_sec);
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
	free(x);
	freeNeurons(system.neuron);
	free(system.spikes);
	free(system.input_vector);

	return 0;
}
