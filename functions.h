/*

  Functions header

*/



#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "neuron.h"

inline double func_sigmoid(double);
inline double func_sigmoid_prime(double);
inline double func_tanh(double);
inline double func_tanh_prime(double);
inline double func_identity(double);
inline double func_identity_prime(double);
inline double func_relu(double);
inline double func_relu_prime(double);
inline double func_quadratic(Neuron*, double);
inline double func_crossentropy(Neuron*, double);
inline double func_regNone(double, double);
inline double func_regL1(double, double);
inline double func_regL2(double, double );


#endif
