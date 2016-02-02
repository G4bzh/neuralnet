/*

  Header file for neurons

*/


#ifndef NEURON_H
#define NEURON_H


typedef enum ACTIVATION
  {
    ACT_NONE,
    ACT_SIGMOID,
    ACT_TANH,
    ACT_IDENTITY,
  } ACTIVATION;


typedef struct Neuron
{
  unsigned int n_in;                   /* Number of inputs */
  double* weights;                     /* Input weights */
  struct Neuron** prevs;               /* Previous neurons */
  double (*activation)(double);        /* Activation function */
  double (*activation_prime)(double);  /* Activation function */
  double output;                       /* Neuron output */
  double z_derivative;                 /* Activation Derivative applied to weighted inputs */
  double error;                        /* Quadratic error */
  double* acc_grad_w;                  /* Weight gradient accumulator */
  double acc_grad_b;                   /* Bias gradient accumulator */
} Neuron;



Neuron* neuron_create(unsigned int, ACTIVATION, double*, Neuron**);
int neuron_delete(Neuron*);
inline int neuron_feedforward(Neuron*);
inline int neuron_backpropagation(Neuron*, double*);
inline int neuron_update(Neuron*, double, double, double (*reg)(double,double));
inline int neuron_maxprev(Neuron*);

#endif
