/****************************************************************************
*  sys_parser.c                                                             *
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
#include "sys_parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
  #define NULL 0
#endif

/*
*
*/
int isNumeric(const char * s)
{
	if (s == NULL
		|| *s == '\0'
		|| isspace((int)s[0]))
		return 0;
	char * p;
	strtod(s, &p);
	return *p == '\0';
}

/*
*
*/
void setVal(double *val, char *str, int *nxt_arg, int *opt_cnt, Option opt)
{
	if (isNumeric(str))
	{
		*val = atof(str);
		*nxt_arg = 1;
		(*opt_cnt)++;
	}
	else
	{
		*nxt_arg = 0;
		*opt_cnt = opt;
	}
}

/*
*
*/
void (*nrnSetFun)(double value, int parNum, void *pars) = NULL;
void setFunPar(char *str, int parNum, void *pars, int *nxt_arg, int *opt_cnt)
{
	double val;
	if (isNumeric(str))
	{
		val = atof(str);
		nrnSetFun(val, parNum, pars);
		*nxt_arg = 1;
	}
	else
	{
		*nxt_arg = 0;
		*opt_cnt = LAST_OPTION;
	}
}

/*
*
*/
int parseArguments(int acnt, char *avec[], System *sys_param)
{
	int argIdx;
	int nxtArg;
	int optCnt = 0;
	int nrnSetFunParNum = 0;
	Neuron *nrnPtr = NULL;
	IniCon *icPtr = NULL;
	Link   *lnkPtr = NULL;
	Neuron *pN;
	Link   *pK;
	IniCon *pIC;

	for (argIdx = 1; argIdx < acnt; argIdx++)
	{
		if (avec[argIdx][0] == '-' && avec[argIdx][1] == 'n') // add neuron (some comment may be attched after n)
		{
			pN = (Neuron*)malloc(sizeof(Neuron));
			if (sys_param->neuron == 0)
			{
				sys_param->neuron = pN;
				nrnPtr = pN;
			}
			else
			{
				nrnPtr->nxtNeuron = pN;
				nrnPtr = pN;
			}
			memset(nrnPtr, 0, sizeof(Neuron)); // all fields set to zero
			nrnPtr->infun = zeroFun;
			nrnPtr->fun_id = (int)ZERO;
			nrnPtr->infun_params = NULL;
			optCnt = NEURON_IC;
		}
		else if (strcmp(avec[argIdx], "-k") == 0) // add link
		{
			pK = (Link*)malloc(sizeof(Link));
			pK->weight  =  0.0;
			pK->intNum  = -1.0; // if not updated, then should not be accessible
			pK->delay   =  0.0;
			pK->nxtLink = NULL;
			spikeQ_new(10, &pK->spike_q);
			if (nrnPtr->k == NULL)
			{
				nrnPtr->k = pK;
				lnkPtr = pK;
			}
			else
			{
				lnkPtr->nxtLink = pK;
				lnkPtr = pK;
			}
			optCnt = LINK_WGT;
		}
		else if (strcmp(avec[argIdx], "-f") == 0) // add function
		{
			optCnt = NEURON_FNC;
		}
		else if (strcmp(avec[argIdx], "-t") == 0)
		{
			optCnt = CALC_TIME;
		}
		else if (strcmp(avec[argIdx], "-s") == 0)
		{
			optCnt = SKP_PNT;
		}
		else
		{
			nxtArg = 0;
			do
			{
				switch (optCnt)
				{
				case NEURON_IC:
					if (isNumeric(avec[argIdx]))
					{
						pIC = (IniCon*)malloc(sizeof(IniCon));
						if (sys_param->lstIniCons == NULL)
						{
							sys_param->lstIniCons = pIC;
							icPtr = pIC;
						}
						else
						{
							icPtr->nxtIniCon = pIC;
							icPtr = pIC;
						}
						icPtr->nxtIniCon = NULL;
						icPtr->val = atof(avec[argIdx]);
						nxtArg = 1;
						optCnt++;
					}
					else
					{
						nxtArg = 0;
						optCnt = NEURON_FNC;
					}
					break;
				case NEURON_MU:
					setVal(&nrnPtr->mu, avec[argIdx], &nxtArg, &optCnt, NEURON_FNC);
					break;
				case NEURON_RST:
					setVal(&nrnPtr->rst_level, avec[argIdx], &nxtArg, &optCnt, NEURON_FNC);
					break;
				case NEURON_SPK:
					setVal(&nrnPtr->spk_level, avec[argIdx], &nxtArg, &optCnt, NEURON_FNC);
					break;
				case NEURON_SQRTD:
					setVal(&nrnPtr->sqrtD, avec[argIdx], &nxtArg, &optCnt, NEURON_FNC);
					break;
				case NEURON_THRESHOLD:
					setVal(&nrnPtr->threshold, avec[argIdx], &nxtArg, &optCnt, NEURON_FNC);
					break;
				case NEURON_FNC:
					if (strcmp(avec[argIdx], "cos") == 0)
					{
						nrnSetFun = getCosAddPar;
						nrnPtr->infun = getCos;
						nrnSetFunParNum = 0;
						nrnPtr->infun_params = (void*)malloc(sizeof(getCosPar));
						nrnPtr->fun_id = (int)COS;
						nxtArg = 1;
						optCnt = NEURON_FNC_PAR;
					}
					else
					{
						nrnPtr->infun = zeroFun;
						nxtArg = 1;
						optCnt++;
					}
					break;
				case NEURON_FNC_PAR:
					setFunPar(avec[argIdx], nrnSetFunParNum, nrnPtr->infun_params, &nxtArg, &optCnt);
					nrnSetFunParNum++;
					break;
				case LINK_WGT:
					setVal(&lnkPtr->weight, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case LINK_NUM:
					setVal(&lnkPtr->intNum, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case LINK_DELAY:
					setVal(&lnkPtr->delay, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case CALC_TIME:
					setVal(&(sys_param->calc_time), avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case SKP_PNT:
					sys_param->skip_points = atoi(avec[argIdx]);
					nxtArg = 1;
					optCnt = LAST_OPTION;
					break;
				case LAST_OPTION:
					nxtArg = 1;
					break;
				default:
					nxtArg = 1;
					break;
				}
			} while (!nxtArg);
		}
	}

	return 0;
}