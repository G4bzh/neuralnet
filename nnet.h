/*
  
  Header file for neural nets

*/


#ifndef NNET_H
#define NNET_H

#include "neuron.h"
#include "dataset.h"



typedef enum Cost
  {
    COST_QUADRATIC,
    COST_CROSSENTROPY,
  } Cost ;

typedef enum Reg
  {
    REG_NONE,
    REG_L1,
    REG_L2,
  } Reg ;


typedef struct NNet
{
  unsigned int n_layers;                               /* Number of layers */
  unsigned int* n_neurons;                             /* Number of neurons per layer */
  Neuron*** layers;                                    /* Per layer array of neurons */ 
  double* biases;                                      /* Per layer biases */
  Cost cost;                                           /* Cost function */
  Reg reg;                                             /* Regularization function */
} NNet;




NNet* nnet_create(Cost, Reg, unsigned int, unsigned int*);
int nnet_delete(NNet*);
int nnet_print(NNet*);
int nnet_feedforward(NNet*, double*);
int nnet_backpropagation(NNet*, double*);
int nnet_update(NNet*, double, double);
int nnet_minibatch(NNet*, Dataset*, unsigned int, double, double);
int nnet_dump(NNet*, char*);

#endif
