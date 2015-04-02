/*
* sim_system.h
*
*  Created on: March 28, 2015
*      Author: Yury Ushakov (yushakov.com)
*/

#ifndef SIM_SYSTEM_H
#define SIM_SYSTEM_H

#include "neuron.h"

#define STEP     1.e-4
#define INV_STEP 1.e+4

typedef struct ParTag
{
	Neuron *neuron;
	double *spikes;
	double *ini_cons;
	double *input_vector;
	int     spike_flag;
	double  calc_time;
	int     skip_points;
	IniCon  *lstIniCons;
} SysParam;

int  sys_function(double *xin, void   *par, double  tin, double *kout);

#endif//SIM_SYSTEM_H