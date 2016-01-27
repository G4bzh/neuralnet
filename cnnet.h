/*
  
  Header file for convolutional neural nets

*/


#ifndef CNNET_H
#define CNNET_H

#include "neuron.h"


typedef struct CVLayer
{
  unsigned int width;
  unsigned int height;
  unsigned n_weights;
  double* weights;
  Neuron* layer;
} CVLayer;


typedef struct CNNet
{
  unsigned int n_in;
  unsigned int n_conv;
  Neuron* inlayer;
  CVLayer* cvlayers;
} CNNet;


CNNet* cnnet_create(unsigned int, unsigned int, unsigned int, unsigned int);
int cnnet_delete(CNNet*);

#endif
