/*

  Functions header

*/


#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "neuron.h"


/*

  Sigmoid

*/

inline double func_sigmoid(double x)
{
  return (1/(1+exp(-x)));
}

/*

  Sigmoid Prime

*/

inline double func_sigmoid_prime(double x)
{
  double y=func_sigmoid(x);
  return y*(1-y);
}


/*
  
  Tanh

*/

inline double func_tanh(double x)
{
  return tanh(x);
}


/*

  Tanh Prime

*/

inline double func_tanh_prime(double x)
{
  double y=func_tanh(x);
  return (1-y*y);
}


/*

  Identity

*/

inline double func_identity(double x)
{
  return x;
}

/*
  
  Identity Prime

*/

inline double func_identity_prime(double x)
{
  return 1.0;
}


/*

  Quadratic cost

*/


inline double func_quadratic(Neuron* N, double out)
{
  assert(N != NULL);

  return  (N->output - out)* N->z_derivative;
}


/*

  CrossEntropy cost

*/

inline double func_crossentropy(Neuron* N, double out)
{
  assert(N != NULL);

  return  (N->output - out);
}


/*

  No Regularization

*/

inline double func_regNone(double r, double w)
{
  return 0.0;
}


/*

  L1 regularization

*/

inline double func_regL1(double r, double w)
{
  if (w)
    {
      return r*(w>0?1:-1);
    }

  return 0.0;
}


/*

  L2 regularization

*/

inline double func_regL2(double r, double w)
{
  return r*w;
}

