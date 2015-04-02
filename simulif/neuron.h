/*
 * neuron.h
 *
 *  Created on: March 19, 2015
 *      Author: Yury Ushakov (yushakov.com)
 */

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

typedef enum OptionEnum
{
	NEURON_IC = 1,
	NEURON_MU,
	NEURON_SQRTD,
	NEURON_RST,
	NEURON_SPK,
	NEURON_THRESHOLD,
	NEURON_FNC,
	NEURON_FNC_AMP,
	NEURON_FNC_OMG,
	NEURON_FNC_PHI,
	LINK_WGT,
	LINK_NUM,
	CALC_TIME,
	SKP_PNT,

	LAST_OPTION
} Option;

double getCos(void *pars);
double zeroFun(void *pars);
void   freeNeurons(Neuron *lst);
void   freeIniCons(IniCon *lst);

#endif /* NEURON_H_ */
