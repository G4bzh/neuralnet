/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
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
	  N->weights[i] = ((double)rand()/(double)RAND_MAX);
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

  Print

*/

int neuron_print(Neuron* N)
{

  assert(N != NULL);

  if (N->n_in)
    {
      unsigned int i;
      printf("[");
      for(i=0;i<N->n_in;i++)
	{
	  printf(" %f ", N->weights[i]);
	}
      printf("| %f ] ", N->output);
      
    }
  else
    {
      printf("[ input | %f ] ", N->output);
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
