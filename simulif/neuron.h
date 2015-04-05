/****************************************************************************
*  neuron.h                                                                 *
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

#ifndef NEURON_H_
#define NEURON_H_

typedef struct LinkTag
{
	double weight;
	double intNum;
	struct LinkTag *nxtLink;
} Link;

typedef struct NeuronTag
{
	double mu;
	double sqrtD;
	double rst_level;
	double spk_level;
	double threshold;
	double (*infun)(void *pars);
	double Amp;
	double Omg;
	double Phi;
	Link   *k;
	struct NeuronTag *nxtNeuron;
} Neuron;

typedef struct IniConTag
{
	double val;
	struct IniConTag *nxtIniCon;
} IniCon;

double getCos(void *pars);
double zeroFun(void *pars);
void   freeNeurons(Neuron *lst);
void   freeIniCons(IniCon *lst);

#endif /* NEURON_H_ */
