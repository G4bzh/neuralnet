/*
  
  Header file for feed forward neural nets

*/


#ifndef FFNNET_H
#define FFNNET_H

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


typedef struct FFNNet
{
  unsigned int n_layers;                               /* Number of layers */
  unsigned int* n_neurons;                             /* Number of neurons per layer */
  Neuron*** layers;                                    /* Per layer array of neurons */ 
  Cost cost;                                           /* Cost function */
  Reg reg;                                             /* Regularization function */
} FFNNet;




FFNNet* ffnnet_create(Cost, Reg, unsigned int, unsigned int*);
int ffnnet_delete(FFNNet*);
int ffnnet_print(FFNNet*);
int ffnnet_feedforward(FFNNet*, double*);
int ffnnet_backpropagation(FFNNet*, double*);
int ffnnet_update(FFNNet*, double, double);
int ffnnet_minibatch(FFNNet*, Dataset*, unsigned int, double, double);
int ffnnet_dump(FFNNet*, char*);
FFNNet* ffnnet_restore(char*);

#endif
