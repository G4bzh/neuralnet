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
int neuron_init(Neuron*);
int neuron_print(Neuron*);
double neuron_identity(double);
double neuron_sigmoid(double);
double neuron_tanh(double);

unsigned int neuron_get_n(Neuron*);
double neuron_get_weight(Neuron*, unsigned int);
int neuron_set_weight(Neuron*,unsigned int, double);
int neuron_set_activation(Neuron*, double (*activation)(double));
double neuron_get_output(Neuron*);


#endif
