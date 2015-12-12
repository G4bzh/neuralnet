/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "layer.h"

typedef struct
{
  unsigned int n_layers;
  Layer** layers;
  double* biases;
} NNet;

NNet* nnet_create(unsigned int);
int nnet_delete(NNet*);
int nnet_print(NNet*);

#endif
