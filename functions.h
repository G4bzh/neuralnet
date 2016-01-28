/*

  Functions header

*/



#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "neuron.h"

double func_sigmoid(double);
double func_quadratic(Neuron*, double);
double func_crossentropy(Neuron*, double);
double func_regNone(double, double);
double func_regL1(double, double);
double func_regL2(double, double );


#endif
