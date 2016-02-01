/*

  Linear layer header

*/

#ifndef LINEAR_H
#define LINEAR_H

#include "neuron.h"


typedef struct LINEAR
{
  unsigned int n_neurons;
  Neuron** neurons;
} LINEAR;


LINEAR* linear_create(unsigned int,unsigned int, Neuron** in[]);
int linear_delete(LINEAR*);
int linear_feedforward(LINEAR*);
int linear_backpropagation(LINEAR*);

#endif
