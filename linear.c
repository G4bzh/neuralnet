/*

  Linear layer

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "linear.h"

/*

  Creation

*/

LINEAR* linear_create(unsigned int depth, unsigned int n_in, Neuron** in[])
{
  assert(depth);
  assert(n_in);
  assert( in != NULL);

  LINEAR* l;
  unsigned n,i,j;

  l = (LINEAR*)malloc(sizeof(LINEAR));
  if (l == NULL)
    {
      return NULL;
    }

  n = n_in*depth;

  l->neurons = (Neuron**)malloc(n*sizeof(Neuron*));
  if (l->neurons == NULL)
    {
      goto err1;
    }

  j=0;
  for(i=0;i<n;i++)
    {
      l->neurons[i] = neuron_create(1,ACT_NONE,NULL,&(in[i/n_in][i%n_in]));
      if(l->neurons[i] == NULL)
	{
	  goto err2;
	}
    }

  l->n_neurons = n;

  return l;

 err2:
  for(j=0;j<i;j++)
    {
      neuron_delete(l->neurons[j]);
    }
  free(l->neurons);
  
 err1:
  free(l);

  return NULL;
}


/*

  Deletion

*/

int linear_delete(LINEAR* l)
{
  unsigned int i;
  
  if (l==NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<l->n_neurons;i++)
    {
      neuron_delete(l->neurons[i]);
    }
  free(l->neurons);
  free(l);

  return EXIT_SUCCESS;
}


/*

  Feeforward

*/


int linear_feedforward(LINEAR* l)
{
  unsigned int i;

  if (l == NULL)
    {
      return EXIT_FAILURE;
    }

  #pragma omp parallel for
  for(i=0;i<l->n_neurons;i++)
    {
      l->neurons[i]->output =  l->neurons[i]->prevs[0]->output;
    }

  return EXIT_SUCCESS;

}


/*

  Backpropagation

*/

int linear_backpropagation(LINEAR* l)
{
  unsigned int i;

  if (l == NULL)
    {
      return EXIT_FAILURE;
    }

  #pragma omp parallel for
  for(i=0;i<l->n_neurons;i++)
    {
      /* Copy error to solo predecessor */
      l->neurons[i]->prevs[0]->error = l->neurons[i]->error;
    }
  
  return EXIT_SUCCESS;
}

