/*
  
  Header file for convolutional neural nets

*/


#ifndef CNNET_H
#define CNNET_H

#include "neuron.h"


typedef struct CVLayer
{
  unsigned int cv_w;
  unsigned int cv_h;
  unsigned int n_weights;
  unsigned int n_neurons;
  double* weights;
  Neuron** neurons;
} CVLayer;


typedef struct CNNet
{
  unsigned int n_conv;
  unsigned int in_w;
  unsigned int in_h;
  Neuron** inlayer;
  CVLayer** cvlayers;
} CNNet;


CNNet* cnnet_create(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
int cnnet_delete(CNNet*);
int cnnet_feedforward(CNNet*, double*);

#endif
