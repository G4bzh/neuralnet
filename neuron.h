/*

  Header file for neurons

*/


#ifndef NEURON_H
#define NEURON_H


typedef struct Neuron
{
  unsigned int n_in;              /* Number of inputs */
  double* weights;                /* Input weights */
  struct Neuron** prevs;          /* Previous neurons */
  double (*activation)(double);   /* Activation function */
  double output;                  /* Neuron output */
  double z_derivative;            /* Activation Derivative applied to weighted inputs */
  double error;                   /* Quadratic error */
  double* acc_grad_w;             /* Weight gradient accumulator */
  double acc_grad_b;              /* Bias gradient accumulator */
} Neuron;



Neuron* neuron_create(unsigned int, double*, Neuron**);
int neuron_delete(Neuron*);
int neuron_print(Neuron*);
double neuron_sigmoid(double);
int neuron_dump(int, Neuron*);
int neuron_restore(int, Neuron*);

#endif
