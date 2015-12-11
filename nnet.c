/*

  Neural Network

*/


#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "nnet.h"


/*

  Layer Creation

*/

Layer* layer_create(unsigned int n)
{
  Layer* l;
  unsigned int i;

  if (!n)
    {
      return NULL;
    }

  l = (Layer*)malloc(sizeof(Layer));
  if (l == NULL)
    {
      return NULL;
    }
  l->n = n;

  l->neurons = (Neuron**)malloc(n*sizeof(Neuron*));
  if (l->neurons == NULL)
    {
      free(l);
      return NULL;
    }

  for(i=0;i<n;i++)
    {
      l->neurons[i] = NULL;
    }

  return l;

}


/*

  Layer Deletion

*/

int layer_delete(Layer* l)
{
  unsigned int i;
  
  if (l == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<l->n;i++)
    {
      neuron_delete(l->neurons[i]);
    }

  free(l);

  return EXIT_SUCCESS;

}

/*

  Creation

*/

NNet* nnet_create(unsigned int n)
{
  NNet* nnet;
  unsigned int i;

  if (n < 2)
    {
      return NULL;
    }

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
      nnet->biases[i] = 0;
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
