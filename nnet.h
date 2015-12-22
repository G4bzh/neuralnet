/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "neuron.h"
#include "dataset.h"

typedef struct NNet
{
  unsigned int n_layers;                               /* Number of layers */
  unsigned int* n_neurons;                             /* Number of neurons per layer */
  Neuron*** layers;                                    /* Per layer array of neurons */ 
  double* biases;                                      /* Per layer biases */
  double (*cost)(struct NNet*, double*, unsigned int); /* Cost function */
} NNet;


typedef enum Cost
  {
    COST_QUADRATIC,
    COST_CROSSENTROPY,
  } Cost ;


NNet* nnet_create(Cost, unsigned int, ...);
int nnet_delete(NNet*);
int nnet_print(NNet*);
int nnet_feedforward(NNet*, double*);
int nnet_backpropagation(NNet*, double*);
int nnet_update(NNet*, double);
int nnet_minibatch(NNet*, Dataset*, unsigned int, double);

#endif
