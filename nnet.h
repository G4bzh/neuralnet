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

unsigned int nnet_get_n(NNet*);
Layer* nnet_get_layer(NNet*, unsigned int);
int nnet_set_layer(NNet*, unsigned int, Layer*);
double nnet_get_bias(NNet*, unsigned int);
int nnet_set_bias(NNet*, unsigned int, double);

#endif
