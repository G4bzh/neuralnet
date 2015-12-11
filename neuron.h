/*

  Header file for neurons

*/


#ifndef NEURON_H
#define NEURON_H


typedef struct
{
  unsigned int n_in;
  double* weights;
  double (*activation)(double);
  double output;
} Neuron;


Neuron* neuron_create(unsigned int, double (*activation)(double));
int neuron_delete(Neuron*);
Neuron* neuron_init(Neuron*);
double neuron_output(Neuron*);


#endif
