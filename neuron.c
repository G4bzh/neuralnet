/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "neuron.h"


/*

  Sigmoid

*/

double neuron_sigmoid(double x)
{
  return (1/(1+exp(-x)));
}


/* 

   Creation

*/



Neuron* neuron_create(unsigned int n)
{
  Neuron* N;
  unsigned int i;

  N = (Neuron*)malloc(sizeof(Neuron));
  if (N == NULL)
    {
      return NULL;
    }

  if (n)
    {
      /* (n+1) in malloc : bias */
      N->weights = (double*)malloc((n+1)*sizeof(double));
      if (N->weights == NULL)
	{
	  free(N);
	  return NULL;
	}

      N->acc_grad_w = (double*)malloc((n)*sizeof(double));
      if (N->acc_grad_w == NULL)
	{
	  free(N->weights);
	  free(N);
	  return NULL;
	}

      for(i=0;i<n;i++)
	{
	  N->weights[i] = ((double)rand()/(double)RAND_MAX);
	  N->acc_grad_w[i] = 0;
	}
      
      /* Bias */
      N->weights[i] = 1;
    }
  
  N-> n_in = n;  
  N->activation = neuron_sigmoid;
  N->output = 0;
  N->z = 0;
  N->z_derivative = 0;
  N->error = 0;
  N->acc_grad_b = 0;

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
      free(N->acc_grad_w);
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
      /* Bias */
      printf(" %f ", N->weights[i]);
      printf("| %f ] ", N->output);
      
    }
  else
    {
      printf("[ input | %f ] ", N->output);
    }

  return EXIT_SUCCESS;
}
