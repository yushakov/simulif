/****************************************************************************
*  rk4.c                                                                    *
*                                                                           *
*  Created on:   September 28, 2014                                         *
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

#include "rk4.h"
#include <malloc.h>

#ifndef NULL
#define NULL 0
#endif

int     g_varNum  = 0;
double *g_kout    = NULL;
double *g_xin     = NULL;
double *g_xresult = NULL;

int  rk4_init(int var_num)
{
	g_varNum = var_num;

	g_kout    = (double*)calloc(g_varNum, sizeof(double));
	g_xin     = (double*)calloc(g_varNum, sizeof(double));
	g_xresult = (double*)calloc(g_varNum, sizeof(double));

	return 0;
}

void rk4_destroy()
{
	if(g_kout != NULL)    free(g_kout);
	if(g_xin != NULL)     free(g_xin);
	if(g_xresult != NULL) free(g_xresult);
}

int  rk4_calc(
		double *x,
		void   *param,
		double  step,
		double  t,
		int (*fun)(
				double *xin,
				void   *par,
				double  tin,
				double *kout))
{
	int i;
	double tin;
	double one6step = step * 0.166666666666666666666666;
	double one2step = step * 0.5;
	double one3step = step * 0.333333333333333333333333;

	fun(x, param, t, g_kout);
	for(i = 0; i < g_varNum; i++)
	{
		g_xresult[i] = x[i] +   one6step*g_kout[i];
		g_xin[i]     = x[i] +   one2step*g_kout[i];
	}

	tin = t + one2step;

	fun(g_xin, param, tin, g_kout);
	for(i = 0; i < g_varNum; i++)
	{
		g_xresult[i] +=        one3step*g_kout[i];
		g_xin[i]      = x[i] + one2step*g_kout[i];
	}

	fun(g_xin, param, tin, g_kout);
	for(i = 0; i < g_varNum; i++)
	{
		g_xresult[i] +=        one3step*g_kout[i];
		g_xin[i]      = x[i] +     step*g_kout[i];
	}

	tin = t + step;

	fun(g_xin, param, tin, g_kout);
	for(i = 0; i < g_varNum; i++)
	{
		x[i] = g_xresult[i] + one6step*g_kout[i];
	}

	return 0;
}

void rk4_reset_var(int var_num, double value)
{
	g_xresult[var_num] = value;
	g_kout[var_num]    = 0.0;
}
