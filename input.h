/*

  Input layer header

*/

#ifndef INPUT_H
#define INPUT_H


#include "neuron.h"


typedef struct INPUT
{
  unsigned int n_neurons;
  Neuron** neurons;
} INPUT;


INPUT* input_create(unsigned int);
int input_delete(INPUT*);
int input_feedforward(INPUT*, double*);


#endif
