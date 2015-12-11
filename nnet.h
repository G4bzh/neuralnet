/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "neuron.h"

typedef struct
{
  Neuron** neurons;
  unsigned int n;
} Layer;

typedef struct
{
  unsigned int n_layers;
  Layer** layers;
  double* biases;
} NNet;

NNet* nnet_create(unsigned int);
int nnet_delete(NNet*);
int nnet_setNeuron(NNet*,Neuron*,unsigned int, unsigned int);
int nnet_print(NNet*);

#endif
