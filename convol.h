/*

  Convolutional layer header

*/

#ifndef CONVOL_H
#define CONVOL_H

#include "neuron.h"


typedef struct CONVOL
{
  unsigned int in_w;
  unsigned int in_h;
  unsigned int cv_w;
  unsigned int cv_h;
  unsigned int n_weights;
  unsigned int n_neurons;
  double* weights;
  Neuron** neurons;
} CONVOL;


CONVOL* convol_create(unsigned int, unsigned int, unsigned int, unsigned int, Neuron**);
int convol_delete(CONVOL*);
int convol_feedforward(CONVOL*);
int convol_backpropagation(CONVOL*);

#endif
