/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "neuron.h"



/* 

   Creation

*/



Neuron* neuron_create(unsigned int n, double (*activation)(double))
{
  Neuron* N;
  unsigned int i;

  assert( activation != NULL);

  N = (Neuron*)malloc(sizeof(Neuron));
  if (N == NULL)
    {
      return NULL;
    }

  if (n)
    {
      N->weights = (double*)malloc(n*sizeof(double));
      if (N->weights == NULL)
	{
	  free(N);
	  return NULL;
	}

      for(i=0;i<n;i++)
	{
	  N->weights[i] = 0;
	}

      N-> n_in = n;
    }
  
  N->activation = activation;

  return N;

}


/*

  Deletion

*/


int neuron_delete(Neuron* N)
{
  if (N == NULL)
    {
      return EXIT_FAILURE;
    }

  if (N->n_in)
    {
      free(N->weights);
    }

  free(N);

  return EXIT_SUCCESS;
}


/*

  Initialisation

*/

int neuron_init(Neuron* N)
{
  unsigned int i;
  
  assert( N != NULL);

  srand(time(NULL));

  for(i=0;i<N->n_in;i++)
    {
      N->weights[i] = ((double)rand()/(double)RAND_MAX);
    }

  return EXIT_SUCCESS;

}


/*

  Print

*/

int neuron_print(Neuron* N)
{

  assert(N != NULL);

  if (N->n_in)
    {
      unsigned int i;
      for(i=0;i<N->n_in;i++)
	{
	  printf("w_%u : %f\n",i,N->weights[i]);
	}
    }
  else
    {
      printf("Input\n");
    }

  return EXIT_SUCCESS;
}


/* Activations */

double neuron_identity(double x)
{
  return x;
}

double neuron_sigmoid(double x)
{
  return (1/(1+exp(-x)));
}

double neuron_tanh(double x)
{
  return tanh(x);
}


/*

  Encapsulation

*/

unsigned int neuron_get_n(Neuron* N)
{
  assert(N != NULL);

  return N->n_in;
}


double neuron_get_weight(Neuron* N, unsigned int i)
{

  assert(N != NULL);
  assert(i < N->n_in);
  
  return N->weights[i];

}

int neuron_set_weight(Neuron*N , unsigned int i, double x)
{
  assert(N != NULL);
  assert(i < N->n_in);
  
  N->weights[i] = x;
  return EXIT_SUCCESS;

}


int neuron_set_activation(Neuron* N, double (*activation)(double))
{
  assert(N != NULL);
  assert(activation != NULL);

  N->activation = activation;

  return EXIT_SUCCESS;

}


double neuron_get_output(Neuron* N)
{
  assert(N != NULL);
  
  return N->output;
}
