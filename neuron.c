/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "neuron.h"



/* 

   Creation

*/



Neuron* neuron_create(unsigned int n, double (*activation)(double))
{
  Neuron* N;
  unsigned int i;

  if (activation == NULL)
    {
      return NULL;
    }

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
  
  if (N == NULL)
    {
      return EXIT_FAILURE;
    }

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
  if (N == NULL)
    {
      return EXIT_FAILURE;
    }

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
