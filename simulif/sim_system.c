/****************************************************************************
*  sim_system.c                                                             *
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

#include "sim_system.h"
#include "rk4.h"
#include "mtwist-1.5/randistrs.h"
#include <math.h>

/*
*
*/
int sys_function(double *xin, void   *par, double  tin, double *kout)
{
	int     i = 0;
	System *sys_param = (System*)par;
	Neuron *pNrn = sys_param->neuron;
	Link   *pLnk;
	double  s = 0;
	Spike  *_Spike = sys_param->spikes;
	double *x = sys_param->ini_cons;
	double *inpVec = sys_param->input_vector;
	static int callCnt = 0;

	while (pNrn != 0)
	{
		// get input spikes
		if (callCnt == 0)
		{
			s = 0.0;
			pLnk = pNrn->k;
			while (pLnk != 0)
			{
				Spike old_spike;
				Spike curr_spike = _Spike[(int)pLnk->intNum];
				if (curr_spike.height > 0.0)
				{
					spikeQ_push(curr_spike, &pLnk->spike_q);
				}
				
				if (spikeQ_first(&old_spike, &pLnk->spike_q) >= 0
					&& old_spike.spikeTime + pLnk->delay < tin)
				{
					if (xin[i] > -0.1) // no reaction in refractory period
					{
						s += pLnk->weight * old_spike.height;
					}
					spikeQ_pop(&old_spike, &pLnk->spike_q);
				}
				pLnk = pLnk->nxtLink;
			}
			inpVec[i] = s;
		}

		kout[i] = -pNrn->mu*xin[i] + inpVec[i] + pNrn->infun(pNrn->infun_params, tin) + rd_normal(0, pNrn->sqrtD);

		// track spikes
		if (callCnt == 3)
		{
			if (x[i] > pNrn->threshold)
			{
				_Spike[i].height    = INV_STEP;
				_Spike[i].spikeTime = tin;
				rk4_reset_var(i, pNrn->rst_level);
				((System*)par)->spike_flag = 1;
			}
			else
			{
				_Spike[i].height    =  0.0;
				_Spike[i].spikeTime = -1.0;
			}
		}
		i++;
		pNrn = pNrn->nxtNeuron;
	}

	callCnt++;
	if (callCnt == 4) callCnt = 0;

	return 0;
}

/*
*
*/
char *getInpFncDescription(char *str, Neuron *np)
{
	sprintf(str, "# No external input\n");
	switch (np->fun_id){
	case COS:
		if (np->infun_params)
		{
			getCosPar par = *(getCosPar*)np->infun_params;
			sprintf(str, "# Input function: %f * COS(%f * t + %f)\n", par.Amp, par.Omg, par.Phi);
		}
		break;
	default:
		break;
	}
	return str;
}

/*
*
*/
double getCos(void *pars, double time)
{
	getCosPar *pD = (getCosPar*)pars;
	return pD->Amp * cos(pD->Omg * time + pD->Phi);
}

/*
*
*/
void getCosAddPar(double value, int parNum, void *pars)
{
	getCosPar *pD = (getCosPar*)pars;

	switch (parNum) {
	case 0:
		pD->Amp = value;
		break;
	case 1:
		pD->Omg = value;
		break;
	case 2:
		pD->Phi = value;
	default:
		break;
	}
}

/*
*
*/
double zeroFun(void *pars, double time)
{
	return 0.0;
}