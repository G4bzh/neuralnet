/*

  Neuron stuff

*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "functions.h"
#include "neuron.h"

/* 

   Creation

*/



Neuron* neuron_create(unsigned int n, ACTIVATION act, double* a, Neuron** p)
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
      assert(p != NULL);

      /* (n+1) in malloc : bias */
      N->weights = (double*)malloc((n+1)*sizeof(double));
      if (N->weights == NULL)
	{
	  free(N);
	  return NULL;
	}

      N->prevs = (Neuron**)malloc(n*sizeof(Neuron*));
      if (N->prevs == NULL)
	{
	  free(N->weights);
	  free(N);
	  return NULL;
	}

      for(i=0;i<n;i++)
	{
	  N->prevs[i] = p[i];
	}

      N->acc_grad_w = (double*)malloc((n)*sizeof(double));
      if (N->acc_grad_w == NULL)
	{
	  free(N->prevs);
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
 
  switch(act)
    {
    case ACT_TANH:
      {
	N->activation = func_tanh;
	N->activation_prime = func_tanh_prime;
      }
    case ACT_SIGMOID:
    default:
      {
	N->activation = func_sigmoid;
	N->activation_prime = func_sigmoid_prime;
      }
    }
  
  N-> n_in = n;  
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
      free(N->prevs);
      free(N->acc_grad_w);
    }

  free(N);

  return EXIT_SUCCESS;
}



/*

  Dump

*/


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

  FeedForward

*/

inline int neuron_feedforward(Neuron* N)
{
  assert( N != NULL );
  assert( N->n_in );

  double sum = 0.0;
  unsigned int i;

  for(i=0;i<N->n_in;i++)
    {
      sum += N->prevs[i]->output * N->weights[i];
    }
  sum += N->weights[i];

  N->output = N->activation(sum);
  N->z_derivative = N->activation_prime(sum);
  N->error = 0;

  return EXIT_SUCCESS;
}


/*

  Backpropagation

*/

inline int neuron_backpropagation(Neuron* N, double* e)
{
  assert( N != NULL );
  assert( N->n_in );

  unsigned int i;

  /* Error */
  if (e == NULL)
    {
      N->error *= N->z_derivative;
    }
  else
    {
      N->error = *e;
    }

  for(i=0;i<N->n_in;i++)
    {
      /* Backpropagate error */
      N->prevs[i]->error += N->error * N->weights[i];
      /* Gradient Accumulation */
      N->acc_grad_w[i] +=  N->error * N->prevs[i]->output;
    }
  N->acc_grad_b +=  N->error;


  return EXIT_SUCCESS;
  
}

/*

  Update

*/

inline int neuron_update(Neuron* N, double l, double r, double (*reg)(double,double))
{
  assert( N != NULL );
  assert( N->n_in );

  unsigned int i;

  for(i=0;i<N->n_in;i++)
    {
      N->weights[i] -= N->acc_grad_w[i]*l + reg(r,N->weights[i]);
      N->acc_grad_w[i] = 0;
    }
  N->weights[i] -= N->acc_grad_b * l;
  N->acc_grad_b = 0;

  
  return EXIT_SUCCESS;
}
