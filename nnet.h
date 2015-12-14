/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "neuron.h"

typedef struct
{
  unsigned int n_layers;
  unsigned int* n_neurons;
  Neuron*** layers;
  double* biases;
} NNet;

NNet* nnet_create(unsigned int,...);
int nnet_delete(NNet*);
int nnet_print(NNet*);

#endif
