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
  double z_derivative;            /* Activation Derivative applied to z */
} Neuron;



Neuron* neuron_create(unsigned int);
int neuron_delete(Neuron*);
int neuron_print(Neuron*);
double neuron_sigmoid(double);




#endif
