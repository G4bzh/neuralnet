/*

  Neural Network

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "layer.h"
#include "nnet.h"



/*

  Creation

*/

NNet* nnet_create(unsigned int n)
{
  NNet* nnet;
  unsigned int i;

  assert(n > 1);
  
  nnet = (NNet*)malloc(sizeof(NNet));
  if (nnet == NULL)
    {
      return NULL;
    }

  nnet->layers = (Layer**)malloc(n*sizeof(Layer*));
  if (nnet->layers == NULL)
    {
      goto err0;
    }
  
  for(i=0;i<n;i++)
    {
      nnet->layers[i] = NULL;
    }
  nnet->n_layers = n;

  nnet->biases = (double*)malloc(n*sizeof(double));
  if (nnet->biases == NULL)
    {
      goto err1;
    }
  
  for(i=0;i<n;i++)
    {
      nnet->biases[i] = 1;
    }

  return nnet;

 err1:
  free(nnet->layers);

 err0:
  free(nnet);
  return NULL;

}


/*

  Deletion

*/

int nnet_delete(NNet* NN)
{
  unsigned int i;

  if (NN == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<NN->n_layers;i++)
    {
      layer_delete(NN->layers[i]);
    }
  free(NN->layers);
  free(NN->biases);
  free(NN);
  
  return EXIT_SUCCESS;

}


/*

  Print

*/

int nnet_print(NNet* NN)
{
  unsigned int i,j,k;

  assert (NN != NULL);
  
  printf("digraph G {\n");
  printf("rankdir=LR\n");
  printf("splines=line\n");
  printf("node [fixedsize=true, label=\"\"];\n");

  for(i=0;i<NN->n_layers;i++)
    {
      layer_print(NN->layers[i],i);
    }


  for(i=0;i<NN->n_layers-1;i++)
    {
      for(j=0;j<NN->layers[i]->n;j++)
	{
	  for(k=0;k<NN->layers[i+1]->n;k++)
	    {
	      printf("n%u%u -> n%u%u\n",i,j,i+1,k);
	    }
	}
    }

  printf("}\n");

  return EXIT_SUCCESS;
}

/*

  Encapsulation

*/

unsigned int nnet_get_n(NNet* NN)
{
  assert(NN != NULL);
  
  return NN->n_layers;
}

Layer* nnet_get_layer(NNet* NN, unsigned int i)
{
  assert(NN != NULL);
  assert(i < NN->n_layers);

  return NN->layers[i];

}

int nnet_set_layer(NNet* NN, unsigned int i, Layer* l)
{
  assert(NN != NULL);
  assert(i < NN->n_layers);
  
  NN->layers[i] = l;

  return EXIT_SUCCESS;
  
}

double nnet_get_bias(NNet* NN, unsigned int i)
{
  assert(NN != NULL);
  assert(i < NN->n_layers);

  return NN->biases[i];

}

int nnet_set_bias(NNet* NN, unsigned int i, double x)
{
  assert(NN != NULL);
  assert(i < NN->n_layers);

  NN->biases[i] = x;

  return EXIT_SUCCESS;

}
