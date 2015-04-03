/****************************************************************************
*  rk4.h                                                                    *
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

#ifndef RK4_H_
#define RK4_H_

int  rk4_init(int var_num);
void rk4_destroy();
int  rk4_calc(
		double *x,
		void   *param,
		double  step,
		double  t,
		int (*fun)(
				double *xin,
				void   *par,
				double  tin,
				double *kout));

/*
 * Can actually work only at the 4th step of system function call.
 */
void rk4_reset_var(int var_num, double value);
#endif /* RK4_H_ */
