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

double func_sigmoid(double x)
{
  return (1/(1+exp(-x)));
}


/*

  Quadratic cost

*/


double func_quadratic(Neuron* N, double out)
{
  assert(N != NULL);

  return  (N->output - out)* N->z_derivative;
}


/*

  CrossEntropy cost

*/

double func_crossentropy(Neuron* N, double out)
{
  assert(N != NULL);

  return  (N->output - out);
}


/*

  No Regularization

*/

double func_regNone(double r, double w)
{
  return 0.0;
}


/*

  L1 regularization

*/

double func_regL1(double r, double w)
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

double func_regL2(double r, double w)
{
  return r*w;
}

