/****************************************************************************
*  sim_system.h                                                             *
*                                                                           *
*  Created on:   March 28, 2015                                             *
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

#ifndef SIM_SYSTEM_H
#define SIM_SYSTEM_H

#include "randistrs.h"
#include "neuron.h"
#include "spike.h"

#define STEP     1.e-4
#define INV_STEP 1.e+4

typedef enum OptionEnum
{
	NEURON_IC = 1,
	NEURON_MU,
	NEURON_SQRTD,
	NEURON_RST,
	NEURON_SPK,
	NEURON_THRESHOLD,
	NEURON_FNC,
	NEURON_FNC_PAR,
	LINK_WGT,
	LINK_NUM,
	LINK_DELAY,
	CALC_TIME,
	SKP_PNT,

	LAST_OPTION
} Option;

typedef struct SystemTag
{
	Neuron *neuron;
	Spike  *spikes;
	double *ini_cons;
	double *input_vector;
	int     spike_flag;
	double  calc_time;
	int     skip_points;
	IniCon  *lstIniCons;
} System;

int  sys_function(double *xin, void   *par, double  tin, double *kout);

typedef enum FunIDtag {
	COS,
	ZERO
} FunID;

typedef struct getCosParTag {
	double Amp;
	double Omg;
	double Phi;
} getCosPar;
double getCos(void *pars, double time);
void getCosAddPar(double value, int parNum, void *pars);


double zeroFun(void *pars, double time);

char *getInpFncDescription(char *str, Neuron *np);

#endif//SIM_SYSTEM_H
