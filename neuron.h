/*

  Header file for neurons

*/


#ifndef NEURON_H
#define NEURON_H


typedef struct
{
  unsigned int n_in;              /* Number of inputs */
  double* weights;                /* Input weights */
  double (*activation)(double);   /* Activation function */
  double output;                  /* Neuron output */
  double z;                       /* Weighted input */
} Neuron;



Neuron* neuron_create(unsigned int, double (*activation)(double));
int neuron_delete(Neuron*);
int neuron_print(Neuron*);
double neuron_identity(double);
double neuron_sigmoid(double);
double neuron_tanh(double);



#endif
