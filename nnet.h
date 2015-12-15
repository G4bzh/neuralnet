/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "neuron.h"

typedef struct
{
  unsigned int n_layers;    /* Number of layers */
  unsigned int* n_neurons;  /* Number of neurons per layer */
  Neuron*** layers;         /* Per layer array of neurons */ 
  double* biases;           /* Per layer biases */
} NNet;

NNet* nnet_create(unsigned int,...);
int nnet_delete(NNet*);
int nnet_print(NNet*);

#endif
