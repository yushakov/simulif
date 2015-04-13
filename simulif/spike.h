/****************************************************************************
*  spike.h                                                                  *
*                                                                           *
*  Created on:   April 10, 2015                                             *
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
#ifndef SPIKE_H
#define SPIKE_H

typedef struct SpikeTag {
	double height;
	double spikeTime;
} Spike;

typedef struct SpikeQtag {
	Spike *q;
	int    size;
	int    first;
	int    last;
} SpikeQ;

int spikeQ_new(int size, SpikeQ *sq);
int spikeQ_destroy(SpikeQ *sq);
int spikeQ_push(Spike s, SpikeQ *sq);
int spikeQ_pop(Spike *s, SpikeQ *sq);
int spikeQ_first(Spike *s, SpikeQ *sq);

#endif//SPIKE_H