/*

  Neural Network

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "dataset.h"
#include "nnet.h"



/*

  Creation

*/

NNet* nnet_create(unsigned int n, ...)
{
  va_list valist;
  NNet* NN;
  unsigned int i,j,k,l;

  /* First parameter is the number of layers */
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
      /* Variable parameters are the numbers of neurons per layer */
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
  
  NN->layers = (Neuron***)malloc(n*sizeof(Neuron**));
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
      	NN->layers[i][j] = neuron_create((i==0?0:NN->n_neurons[i-1]));
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

  Print (Graphviz/Dot format)

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


/*

  Feedforward

*/

int nnet_feedforward(NNet* NN, double* in)
{
  unsigned int i,j,k;

  assert(NN != NULL);
  assert(in != NULL);

  /* Set input as layer 0 neurons output */
  for(i=0;i<NN->n_neurons[0];i++)
    {
      NN->layers[0][i]->output = in[i];
    }


  /* Feed forward */
  for(i=1;i<NN->n_layers;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  double sum=0;
	  for(k=0;k<NN->n_neurons[i-1];k++)
	    {
	      sum += NN->layers[i-1][k]->output *  NN->layers[i][j]->weights[k] ;
	    }
	  sum += NN->biases[i-1] * NN->layers[i][j]->weights[k];
	  NN->layers[i][j]->z = sum;
	  NN->layers[i][j]->output = NN->layers[i][j]->activation(sum);
	  NN->layers[i][j]->z_derivative = NN->layers[i][j]->output*(1-NN->layers[i][j]->output); /* sigmoid' = sigmoid*(1-sigmoid) */
	}
      
    }
 
  return EXIT_SUCCESS;
}


int nnet_backpropagation(NNet* NN, double* out)
{
  unsigned int i,j,k;
  double sum;

  assert(NN != NULL);
  assert(out != NULL);

  /* Last layer error */
  for(j=0;j<NN->n_neurons[NN->n_layers-1];j++)
    {
      /* derivative of quadratic cost : (target - output) */
      NN->layers[NN->n_layers-1][j]->error = (NN->layers[NN->n_layers-1][j]->output - out[j]) * NN->layers[NN->n_layers-1][j]->z_derivative;
    }

  
  /* Error backpropagation */
  for(i=NN->n_layers-2;i>0;i--)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  sum = 0;
	  for(k=0;k<NN->n_neurons[i+1];k++)
	    {
	      sum += NN->layers[i+1][k]->weights[k] * NN->layers[i+1][k]->error;
	    }
	  NN->layers[i][j]->error = sum * NN->layers[i][j]->z_derivative;
	}
    }

  /* Gradient Accumulation */
  for(i=NN->n_layers-1;i>0;i--)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  for(k=0;k<NN->layers[i][j]->n_in;k++)
	    {
	      NN->layers[i][j]->acc_grad_w[k] += NN->layers[i][j]->error *  NN->layers[i-1][k]->output;
	    }
	  NN->layers[i][j]->acc_grad_b += NN->layers[i][j]->error;
	}
    }

  return EXIT_SUCCESS;
}


/*

  Update (gradient descent)

*/

int nnet_update(NNet* NN, double l)
{
  unsigned int i,j,k;

  assert(NN != NULL);
  assert(l);

  for(i=NN->n_layers-1;i>0;i--)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  for(k=0;k<NN->layers[i][j]->n_in;k++)
	    {
	      NN->layers[i][j]->weights[k] -= NN->layers[i][j]->acc_grad_w[k] * l;
	      NN->layers[i][j]->acc_grad_w[k] = 0; 
	    }
	  NN->layers[i][j]->weights[k] -= NN->layers[i][j]->acc_grad_b * l;
	  NN->layers[i][j]->acc_grad_b = 0;
	}
    }

  return EXIT_SUCCESS;
}


/*

  Mini-batch

*/

int nnet_minibatch(NNet* NN, Dataset* ds, unsigned int n, double eta)
{
  assert( NN != NULL );
  assert( ds != NULL );
  assert( n < ds->len );
  assert( n );

  unsigned int i,j;

  i=0;
  while( i+n < ds->len )
    {
      for(j=i;j<i+n;j++)
	{
	  assert( nnet_feedforward(NN,ds->in[j]) == EXIT_SUCCESS );
	  assert( nnet_backpropagation(NN,ds->out[j])  == EXIT_SUCCESS );
	}
      assert( nnet_update(NN, eta/(double)n) == EXIT_SUCCESS );
      i += n;
    }
  
  return EXIT_SUCCESS;
}

/*

  Evaluate

*/

unsigned int nnet_evaluate(NNet* NN, Dataset* ds)
{
  unsigned int i,j;
  unsigned int n=0;

  assert( NN != NULL );
  assert( ds != NULL );

  for(i=0;i<ds->len;i++)
    {
      assert( nnet_feedforward(NN,ds->in[i]) == EXIT_SUCCESS );

      for(j=0;j<ds->out_len;j++)
	{
	  printf(" %f (expected: %f) ",NN->layers[NN->n_layers - 1][j]->output,ds->out[i][j]);
	}
    }

  return n;
  
}
