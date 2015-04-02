/*
 * rk4.h
 *
 *  Created on: 28 сент. 2014 г.
 *      Author: user1
 */

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
