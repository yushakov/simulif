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

/*
*
*/
int sys_function(double *xin, void   *par, double  tin, double *kout)
{
	int     i = 0;
	SysParam *sys_param = (SysParam*)par;
	Neuron *pNrn = sys_param->neuron;
	Link   *pLnk;
	double  s = 0;
	double  infun_param[4];
	double *Spike = sys_param->spikes;
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
			while (pLnk != 0 &&
				xin[i] > -0.1 // no reaction in refractory period
				)
			{
				s += pLnk->weight * Spike[(int)pLnk->intNum];
				pLnk = pLnk->nxtLink;
			}
			inpVec[i] = s;
		}

		infun_param[0] = pNrn->Amp;
		infun_param[1] = pNrn->Omg;
		infun_param[2] = pNrn->Phi;
		infun_param[3] = tin;
		kout[i] = -pNrn->mu*xin[i] + inpVec[i] + pNrn->infun((void*)infun_param) + rd_normal(0, pNrn->sqrtD);

		// track spikes
		if (callCnt == 3)
		{
			if (x[i] > pNrn->threshold)
			{
				Spike[i] = INV_STEP;
				rk4_reset_var(i, pNrn->rst_level);
				((SysParam*)par)->spike_flag = 1;
			}
			else
			{
				Spike[i] = 0.0;
			}
		}
		i++;
		pNrn = pNrn->nxtNeuron;
	}

	callCnt++;
	if (callCnt == 4) callCnt = 0;

	return 0;
}