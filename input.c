/*

  Input layer

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "input.h"



/*

  Creation

*/

INPUT* input_create(unsigned int i)
{
  assert(i);

  INPUT* in;
  unsigned int j,k;

  in = (INPUT*)malloc(sizeof(INPUT));
  if (in == NULL)
    {
      return NULL;
    }

  in->neurons = (Neuron**)malloc(i*sizeof(Neuron*));
  if (in->neurons == NULL)
    {
      goto err1;
    }

  for(j=0;j<i;j++)
    {
      in->neurons[j] = neuron_create(0,ACT_NONE,NULL,NULL);
      if (in->neurons[j] == NULL)
	{
	  goto err2;
	}
    }
  in->n_neurons = i;

  return in;

 err2:
  for(k=0;k<j;k++)
    {
      neuron_delete(in->neurons[k]);
    }

 err1:
  free(in);

  return NULL;
}



/*

  Deletion

*/

int input_delete(INPUT* in)
{
  unsigned int i;

  if (in == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<in->n_neurons;i++)
    {
      neuron_delete(in->neurons[i]);
    }

  free(in);

  return EXIT_SUCCESS;
}



/*

  FeedForward (initialization in fact)

*/

int input_feedforward(INPUT* in, double* a)
{
  assert(in != NULL);
  assert(a != NULL);

  unsigned int i;

  #pragma omp parallel for
  for(i=0;i<in->n_neurons;i++)
    {
      in->neurons[i]->output = a[i];
    }

  return EXIT_SUCCESS;

}
