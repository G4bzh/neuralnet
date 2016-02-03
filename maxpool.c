/*

  MaxPool Layer

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "maxpool.h"


/*

  Creation

*/

MAXPOOL* maxpool_create(unsigned int in_w, unsigned int in_h, unsigned int pool_w, unsigned int pool_h,  Neuron** in)
{
  assert(in_w);
  assert(in_h);
  assert(pool_w);
  assert(pool_h);
  assert(in != NULL);
  assert(!(in_w % pool_w));
  assert(!(in_h % pool_h));
  

  MAXPOOL* mp;
  Neuron** p;
  unsigned int i,j,k,u,v;

  mp = (MAXPOOL*)malloc(sizeof(MAXPOOL));
  if (mp == NULL)
    {
      return NULL;
    }

  mp->n_neurons = (in_w/pool_w)*(in_h/pool_h);
  
  mp->neurons = (Neuron**)malloc(mp->n_neurons*sizeof(Neuron*));
  if (mp->neurons == NULL)
    {
      goto err1;
    }

  /* Helper */
  p = (Neuron**)malloc(pool_w*pool_h*sizeof(Neuron*));
  if (p == NULL)
    {
      goto err2;
    }

  k=0;
  for(j=0;j<in_h;j+=pool_h)
    {
      for(i=0;i<in_w;i+=pool_w)
	{
	  for(v=0;v<pool_h;v++)
	    {
	      for(u=0;u<pool_w;u++)
		{
		  
		  p[v*pool_w+u] = in[(j+v)*in_w+(i+u)];
		}
	    }
	  mp->neurons[k] = neuron_create(pool_w*pool_h,ACT_NONE,NULL,p);
	  if(mp->neurons[k] == NULL)
	    {
	      goto err3;
	    }
	  
	  k++;
	}
    }

  free(p);

  mp->in_w = in_w;
  mp->in_h = in_h;
  mp->pool_w = pool_w;
  mp->pool_h = pool_h;

  return mp;

 err3:
  for(i=0;i<k;i++)
    {
      neuron_delete(mp->neurons[i]);
    }
  free(p);

 err2:
  free(mp->neurons);

 err1:
  free(mp);

  return NULL;
}


/*

  Deletion

*/

int maxpool_delete(MAXPOOL* mp)
{
  unsigned int i;

  if (mp == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<mp->n_neurons;i++)
    {
      neuron_delete(mp->neurons[i]);
    }
  
  free(mp->neurons);
  free(mp);

  return EXIT_SUCCESS;

}


/*

  Feedforward
 
*/

int maxpool_feedforward(MAXPOOL* mp)
{
  unsigned int i;

  if (mp == NULL)
    {
      return EXIT_FAILURE;
    }

   for(i=0;i<mp->n_neurons;i++)
    {
      neuron_maxprev(mp->neurons[i]);
    }

  return EXIT_SUCCESS;

}


/*

  Backpropagation

*/

int maxpool_backpropagation(MAXPOOL* mp)
{
  unsigned int i;

  if (mp == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<mp->n_neurons;i++)
    {
      /* Copy error to max predecessor */
      mp->neurons[i]->prevs[(unsigned int)(mp->neurons[i]->weights[0])]->error += mp->neurons[i]->error;
      mp->neurons[i]->error = 0;
    }
  
  return EXIT_SUCCESS;
}
