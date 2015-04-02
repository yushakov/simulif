/*
 * neuron.c
 *
 *  Created on: March 19, 2015
 *      Author: Yury Ushakov (yushakov.com)
 */

#include "neuron.h"
#include <malloc.h>
#include <math.h>

#ifndef NULL
#define NULL 0
#endif

/*
*
*/
double getCos(void *pars)
{
	double *pD = (double*)pars;
	return pD[0] * cos(pD[1] * pD[3] + pD[2]);
}

/*
*
*/
double zeroFun(void *pars)
{
	return 0.0;
}

/*
 *
 */
void freeNeurons(Neuron *lst)
{
	Neuron *pN1, *pN2;
	Link *pL1, *pL2;

	pN1 = lst;
	while(pN1 != NULL)
	{
		pL1 = pN1->k;
		while(pL1 != NULL)
		{
			pL2 = pL1;
			pL1 = pL1->nxtLink;
			free(pL2);
			pL2 = NULL;
		}
		pN2 = pN1;
		pN1 = pN1->nxtNeuron;
		free(pN2);
		pN2 = NULL;
	}
}

/*
 *
 */
void freeIniCons(IniCon *lst)
{
	IniCon *pIC1, *pIC2;
	pIC1 = lst;
	while(pIC1 != NULL)
	{
		pIC2 = pIC1;
		pIC1 = pIC1->nxtIniCon;
		free(pIC2);
		pIC2 = NULL;
	}
}
