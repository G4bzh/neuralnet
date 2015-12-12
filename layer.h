/*

  Layer header

*/

#ifndef LAYER_H
#define LAYER_H

#include "neuron.h"

typedef struct
{
  Neuron** neurons;
  unsigned int n;
} Layer;

Layer* layer_create(unsigned int);
int layer_delete(Layer*);
int layer_print(Layer*, unsigned  int);

#endif
