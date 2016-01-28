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


FULLCONN* fullconn_create(unsigned int, unsigned int, Neuron**);
int fullconn_delete(FULLCONN*);
int fullconn_feedforward(FULLCONN*);
int fullconn_backpropagation(FULLCONN*, double*, double (*cost)(Neuron*,double));
int fullconn_update(FULLCONN*, double, double, double (*reg)(double,double));

#endif
