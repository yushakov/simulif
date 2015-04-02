/*
* sim_system.c
*
*  Created on: March 28, 2015
*      Author: Yury Ushakov (yushakov.com)
*/

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