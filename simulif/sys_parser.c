/*
* sys_parser.c
*
*  Created on: March 28, 2015
*      Author: Yury Ushakov (yushakov.com)
*/
#include "sys_parser.h"

#include <ctype.h>
#include <stdlib.h>

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
int parseArguments(int acnt, char *avec[], SysParam *sys_param)
{
	int argIdx;
	int nxtArg;
	int optCnt = 0;
	Neuron *nrnPtr = NULL;
	IniCon *icPtr = NULL;
	Link   *lnkPtr = NULL;
	Neuron *pN;
	Link   *pK;
	IniCon *pIC;

	for (argIdx = 1; argIdx < acnt; argIdx++)
	{
		if (strcmp(avec[argIdx], "-n") == 0)
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
			optCnt = NEURON_IC;
		}
		else if (strcmp(avec[argIdx], "-k") == 0)
		{
			pK = (Link*)malloc(sizeof(Link));
			memset(pK, 0, sizeof(Link)); // all fields set to zero
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
						nrnPtr->infun = getCos;
						nxtArg = 1;
						optCnt++;
					}
					else
					{
						nrnPtr->infun = zeroFun;
						nxtArg = 1;
						optCnt++;
					}
					break;
				case NEURON_FNC_AMP:
					setVal(&nrnPtr->Amp, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case NEURON_FNC_OMG:
					setVal(&nrnPtr->Omg, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case NEURON_FNC_PHI:
					setVal(&nrnPtr->Phi, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case LINK_WGT:
					setVal(&lnkPtr->weight, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
					break;
				case LINK_NUM:
					setVal(&lnkPtr->intNum, avec[argIdx], &nxtArg, &optCnt, LAST_OPTION);
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