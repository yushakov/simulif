/****************************************************************************
*  neuron.c                                                                 *
*                                                                           *
*  Created on:   March 19, 2015                                             *
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

#include "neuron.h"
#include <malloc.h>

#ifndef NULL
#define NULL 0
#endif

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
		if (pN2->infun_params)
		{
			free(pN2->infun_params);
			pN2->infun_params = NULL;
		}
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
