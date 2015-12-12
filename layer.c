/*

  Neurons Layer

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "layer.h"


/*

  Creation

*/

Layer* layer_create(unsigned int n)
{
  Layer* l;
  unsigned int i;

  assert(n);

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

  Deletion

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

   Print

*/

int layer_print(Layer* l, unsigned  int n)
{
  unsigned int i;
  
  assert(l != NULL);

  printf("subgraph cluster_%u {\n",n);
  printf("color=white;\n");
  printf("node [style=solid,color=blue4, shape=circle];\n");
  
  if (l->n)
    {
      for(i=0;i<l->n;i++)
	{
	  printf("n%u%u ",n,i);
	}
      printf(";\n");
    }
  
  printf("label = \"layer %u\"\n",n);
  printf("}\n");
  
  return EXIT_SUCCESS;
}

/*

  Encapsulation

*/

unsigned int layer_get_n(Layer* l)
{
  assert(l != NULL);
  return l->n;
}

Neuron* layer_get_neuron(Layer* l, unsigned int i)
{
  assert(l != NULL);
  assert(i < l->n);
  
  return l->neurons[i];
}

int layer_set_neuron(Layer* l, unsigned int i, Neuron* N)
{
  assert(l != NULL);
  assert(i < l->n);

  l->neurons[i] = N;

  return EXIT_SUCCESS;

}
