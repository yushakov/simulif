/****************************************************************************
*  spike.c                                                                  *
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
#include "spike.h"
#include <malloc.h>

int spikeQ_new(int size, SpikeQ *sq)
{
	sq->q = (Spike*)calloc(size, sizeof(Spike));
	sq->first =  0;
	sq->last  = -1;
	sq->size  = size;

	return 0;
}

int spikeQ_destroy(SpikeQ *sq)
{
	if (sq->q)
	{
		free(sq->q);
		sq->first =  0;
		sq->last  = -1;
		sq->size  =  0;
	}
	return 0;
}

int spikeQ_push(Spike s, SpikeQ *sq)
{
	if (sq->last < sq->size - 1)
	{
		sq->q[++sq->last] = s;
	}
	else
	{
		if (sq->first > 0)
		{
			// move queue
			int i;
			Spike *to = &sq->q[0];
			for (i = sq->first; i <= sq->last; i++)
			{
				*to++ = sq->q[i];
			}
			sq->last -= sq->first + 1;
			sq->first = 0;
			sq->q[sq->last] = s;
		}
		else
		{
			// resize queue
			Spike *new_q = 0;
			new_q = calloc(2 * sq->size, sizeof(Spike));
			if (new_q)
			{
				// copy queue to the new memory location
				int i;
				Spike *to = &new_q[0];
				for (i = sq->first; i <= sq->last; i++)
				{
					*to++ = sq->q[i];
				}
				sq->last -= sq->first + 1;
				sq->first = 0;
				new_q[sq->last] = s;
				free(sq->q);
				sq->q = new_q;
				sq->size *= 2;
			}
			else
			{
				return -1;
			}
		}
	}
	return 0;
}

int spikeQ_pop(Spike *s, SpikeQ *sq)
{
	if (sq->first <= sq->last)
	{
		*s = sq->q[sq->first];
		sq->first++;
	}
	return 0;
}

int spikeQ_first(Spike *s, SpikeQ *sq)
{
	int last = sq->last;
	if (last >= 0) *s = sq->q[last];

	return 0;
}