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

unsigned int layer_get_n(Layer*);
Neuron* layer_get_neuron(Layer*, unsigned int);
int layer_set_neuron(Layer*, unsigned int, Neuron*);

#endif
