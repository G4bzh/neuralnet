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


INPUT* input_create(unsigned int i);
int input_delete(INPUT*);
int input_init(INPUT*, double*);


#endif
