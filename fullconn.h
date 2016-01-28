/*

  Fully Connected Layer header

*/


#ifndef FULLCONN_H
#define FULLCONN_H


#include "neuron.h"

typedef struct FULLCONN
{
  unsigned int n_prev;
  unsigned int n_neurons;
  Neuron** neurons;
} FULLCONN;


FULLCONN* fullconn_create(unsigned int, unsigned int);
int fullconn_delete(FULLCONN*);

#endif
