/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
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



Neuron* neuron_create(unsigned int n, double* a)
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

     
      if (a == NULL)
	{
	  for(i=0;i<n;i++)
	    {
	      N->weights[i] = ((double)(rand()-RAND_MAX/2)/(double)RAND_MAX);
	      N->acc_grad_w[i] = 0.0;
	    }
	}
      else
	{
	  for(i=0;i<n;i++)
	    {
	      N->weights[i] = a[i];
	      N->acc_grad_w[i] = 0.0;
	    }
	}
      
      /* Bias */
      N->weights[n] = 1.0;
      
    }
 
  N-> n_in = n;  
  N->activation = neuron_sigmoid;
  N->output = 0;
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


int neuron_dump(int fd, Neuron* N)
{
  unsigned int i;

  assert(N != NULL);
  assert(fd != -1);

  assert(write(fd,&(N->n_in),sizeof(unsigned int)) != -1);

  if (N->n_in)
    {
      for(i=0;i<N->n_in;i++)
	{
	  assert(write(fd,&(N->weights[i]),sizeof(double)) != -1);
	}
      
      assert(write(fd,&(N->weights[i]),sizeof(double)) != -1);
    }

  return EXIT_SUCCESS;
}


/*

  Restore

*/


int neuron_restore(int fd, Neuron* N)
{
  unsigned int i;

  assert(N != NULL);
  assert(fd != -1);

  assert(read(fd,&i,sizeof(unsigned int)) != -1);
  assert( N->n_in == i);

  if (N->n_in)
    {
      for(i=0;i<N->n_in;i++)
	{
	  assert(read(fd,&(N->weights[i]),sizeof(double)) != -1);
	}
      
      assert(read(fd,&(N->weights[i]),sizeof(double)) != -1);
    }
  
  return EXIT_SUCCESS;
}


/*

  Copy

*/

int neuron_copy(Neuron* N0, Neuron* N1)
{
  unsigned int i;

  assert(N0 != NULL);
  assert(N1 != NULL);
  assert(N0->n_in == N1->n_in);
  
  for(i=0;i<N0->n_in;i++)
    {
      N1->weights[i] = N0->weights[i];
      N1->acc_grad_w[i] = N0->acc_grad_w[i];
    }
  /* bias */
  N1->weights[i] = N0->weights[i];
  N1->acc_grad_b = N0->acc_grad_b;

  return EXIT_SUCCESS;
}


/* 

   Merge

*/

int neuron_merge(Neuron* N0, Neuron* N1)
{
  unsigned int i;

  assert(N0 != NULL);
  assert(N1 != NULL);
  assert(N0->n_in == N1->n_in);
  
  for(i=0;i<N0->n_in;i++)
    {
      N0->weights[i] += N1->weights[i];
      N0->acc_grad_w[i] += N1->acc_grad_w[i];
    }
  /* bias */
  N0->weights[i] += N1->weights[i];
  N0->acc_grad_b += N1->acc_grad_b;

  return EXIT_SUCCESS;
}
