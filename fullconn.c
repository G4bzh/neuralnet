/*

  Fully Connected Layer 

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "fullconn.h"


/*

  Creation

*/

FULLCONN* fullconn_create(unsigned int n_neurons, ACTIVATION act, unsigned int n_prev, Neuron** prev)
{
  assert(n_prev);
  assert(n_neurons);
  assert(prev != NULL);

  FULLCONN* FC;
  unsigned int i,j;

  FC = (FULLCONN*)malloc(sizeof(FULLCONN));
  if (FC == NULL)
    {
      return NULL;
    }


  FC->neurons = (Neuron**)malloc(n_neurons*sizeof(Neuron*));
  if (FC->neurons == NULL)
    {
      goto err1;
    }

  for(i=0;i<n_neurons;i++)
    {
      FC->neurons[i] = neuron_create(n_prev,act,NULL,prev);
      if (FC->neurons[i] == NULL)
	{
	  goto err2;
	}
    }

  FC->n_prev = n_prev;
  FC->n_neurons = n_neurons;

  return FC;

 err2:
  for(j=0;j<i;j++)
    {
      neuron_delete(FC->neurons[j]);
    }

 err1:
  free(FC);

  return NULL;

}



/*

  Deletion

*/


int fullconn_delete(FULLCONN* FC)
{
  unsigned int i;

  if (FC == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<FC->n_neurons;i++)
    {
      neuron_delete(FC->neurons[i]);
    }
  
  free(FC);

  return EXIT_SUCCESS;
}


/*

  FeedForward

*/


int fullconn_feedforward(FULLCONN* FC)
{
  unsigned int i;

  if (FC == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<FC->n_neurons;i++)
    {
      neuron_feedforward(FC->neurons[i]);
    }
        
  return EXIT_SUCCESS;
 }


/*

  Backpropagation

*/

int fullconn_backpropagation(FULLCONN* FC, double* out, double (*cost)(Neuron*,double))
{
  unsigned int i;

  if (FC == NULL)
    {
      return EXIT_FAILURE;
    }

  if (out == NULL)
    {
      for(i=0;i<FC->n_neurons;i++)
	{
	  neuron_backpropagation(FC->neurons[i],NULL);
	}
    }
  else
    {
      /* Last layer case */
      assert(out != NULL);
      assert(cost != NULL);

      double e;

      for(i=0;i<FC->n_neurons;i++)
	{
	  e = cost(FC->neurons[i],out[i]);
	  neuron_backpropagation(FC->neurons[i],&e);
	}
    }

        
  return EXIT_SUCCESS;
}


/*

  Update (gradient descent)

*/

int fullconn_update(FULLCONN* FC, double l, double r, double (*reg)(double,double))
{
  unsigned int i;

  if ( (FC == NULL) || (reg == NULL) )
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<FC->n_neurons;i++)
    {
      neuron_update(FC->neurons[i],l,r,reg,1);
    }

  return EXIT_SUCCESS;
}
