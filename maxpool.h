/*

  MaxPool layer header

*/


#ifndef MAXPOOL_H
#define MAXPOOL_H


#include "neuron.h"

typedef struct MAXPOOL
{
  unsigned int in_w;
  unsigned int in_h;
  unsigned int pool_w;
  unsigned int pool_h;
  unsigned int n_neurons;
  Neuron** neurons;
} MAXPOOL;

MAXPOOL* maxpool_create(unsigned int, unsigned int, unsigned int, unsigned int,  Neuron**);
int maxpool_delete(MAXPOOL*);
int maxpool_feedforward(MAXPOOL*);
int maxpool_backpropagation(MAXPOOL*);

#endif
