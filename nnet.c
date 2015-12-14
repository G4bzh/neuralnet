/*

  Neural Network

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "nnet.h"



/*

  Creation

*/

NNet* nnet_create(unsigned int n, ...)
{
  va_list valist;
  NNet* NN;
  unsigned int i,j,k,l;

  assert(n > 1);
  
  NN = (NNet*)malloc(sizeof(NNet));
  if (NN == NULL)
    {
      return NULL;
    }

  NN->n_neurons = (unsigned int*)malloc(n*sizeof(unsigned int));
  if (NN->n_neurons == NULL)
    {
      goto err0;
    }

  va_start(valist, n);
  
  for(i=0;i<n;i++)
    {
      NN->n_neurons[i] = va_arg(valist,unsigned int);
    }

  va_end(valist);
  

  NN->biases = (double*)malloc(n*sizeof(double));
  if (NN->biases == NULL)
    {
      goto err1;
    }

  for(i=0;i<n;i++)
    {
      NN->biases[i] = 1;
    }
  
  NN->layers = (Neuron***)malloc(n*sizeof(Neuron***));
  if (NN->layers == NULL)
    {
      goto err2;
    }			    

  j=0;
  for(i=0;i<n;i++)
    {
 
      NN->layers[i] = (Neuron**)malloc(NN->n_neurons[i]*sizeof(Neuron*));
      if (NN->layers[i] == NULL)
      	{
      	  goto err3;
      	}
     
      for(j=0;j<NN->n_neurons[i];j++)
      {
      	NN->layers[i][j] = neuron_create((i==0?0:NN->n_neurons[i-1]),neuron_sigmoid);
      	if (NN->layers[i][j] == NULL)
      	  {
      	    goto err3;
      	  }
      }
    }

  NN->n_layers = n;

  return NN;

 err3:
  for(k=0;k<i;k++)
    {
      for(l=0;l<j;l++)
	{
	  neuron_delete(NN->layers[k][l]);
	}
      free(NN->layers[k]);
    }

  free(NN->layers);

 err2:
  free(NN->biases);

 err1:
  free(NN->n_neurons);

 err0:
  free(NN);

  return NULL;

}


/*

  Deletion

*/

int nnet_delete(NNet* NN)
{
  unsigned int i,j;

  if (NN == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<NN->n_layers;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_delete(NN->layers[i][j]);
	}
      free(NN->layers[i]);
    }
  free(NN->layers);
  free(NN->n_neurons);
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

      printf("subgraph cluster_%u {\n",i);
      printf("color=white;\n");
      printf("node [style=solid,color=blue4, shape=circle];\n");
      
      if (NN->n_neurons[i])
	{
	  for(j=0;j<NN->n_neurons[i];j++)
	    {
	      printf("n%u%u ",i,j);
	    }
	  printf(";\n");
	}
      
      printf("label = \"layer %u\"\n",i);
      printf("}\n");
      
    }


  for(i=0;i<NN->n_layers-1;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  for(k=0;k<NN->n_neurons[i+1];k++)
	    {
	      printf("n%u%u -> n%u%u\n",i,j,i+1,k);
	    }
	}
    }

  printf("}\n");

  return EXIT_SUCCESS;
}
