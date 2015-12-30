/*

  Neural Network

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "neuron.h"
#include "dataset.h"
#include "nnet.h"



/*

  Cost Functions

*/

double nnet_quadratic(NNet* NN, double* a, unsigned int i)
{
  assert(NN != NULL);
  assert(a != NULL);

  return  (NN->layers[NN->n_layers-1][i]->output - a[i])* NN->layers[NN->n_layers-1][i]->z_derivative;
}

double nnet_crossentropy(NNet* NN, double* a, unsigned int i)
{
  assert(NN != NULL);
  assert(a != NULL);

  return  (NN->layers[NN->n_layers-1][i]->output - a[i]);
}


double (*costs[2])(NNet*, double*, unsigned int)= {nnet_quadratic, nnet_crossentropy};


/*

  Regularization Functions

 */


double nnet_regL2(double r, double w)
{
  return r*w;
}

double nnet_regL1(double r, double w)
{
  if (w)
    {
      return r*(w>0?1:-1);
    }

  return 0.0;
}

double nnet_regNone(double r, double w)
{
  return 0.0;
}

double (*regs[3])(double,double) = {nnet_regNone, nnet_regL1, nnet_regL2};



/*

  Creation

*/

NNet* nnet_create(Cost c, Reg r, unsigned int n, unsigned int* a)
{
  NNet* NN;
  unsigned int i,j,k,l;

  /* First parameter is the number of layers */
  assert(n > 1);
  assert(a != NULL);

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

    
  for(i=0;i<n;i++)
    {
      /* Array contains  the numbers of neurons per layer */
      NN->n_neurons[i] = a[i];
    }
  

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
      	NN->layers[i][j] = neuron_create((i==0?0:NN->n_neurons[i-1]),NULL);
      	if (NN->layers[i][j] == NULL)
      	  {
      	    goto err3;
      	  }
      }
    }

  NN->n_layers = n;
  NN->cost = c;
  NN->reg = r;

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
      /* Error */
      NN->layers[NN->n_layers-1][j]->error = (costs[NN->cost])(NN,out,j);

      /* Gradient Accumulation */
      for(k=0;k<NN->layers[NN->n_layers-1][j]->n_in;k++)
	{
	  NN->layers[NN->n_layers-1][j]->acc_grad_w[k] += NN->layers[NN->n_layers-1][j]->error *  NN->layers[NN->n_layers-1-1][k]->output; 
	}
      NN->layers[NN->n_layers-1][j]->acc_grad_b += NN->layers[NN->n_layers-1][j]->error;

  }

  /* Error backpropagation */
  for(i=NN->n_layers-2;i>0;i--)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  sum = 0;
	  for(k=0;k<NN->n_neurons[i+1];k++)
	    {
	      sum += NN->layers[i+1][k]->weights[j] * NN->layers[i+1][k]->error;
	    }
	  NN->layers[i][j]->error = sum * NN->layers[i][j]->z_derivative;

	  /* Gradient accumulation */
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

int nnet_update(NNet* NN, double l, double r)
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
	      NN->layers[i][j]->weights[k] = NN->layers[i][j]->weights[k] - (regs[NN->reg])(r,NN->layers[i][j]->weights[k]) - NN->layers[i][j]->acc_grad_w[k] * l;
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

int nnet_minibatch(NNet* NN, Dataset* ds, unsigned int n, double eta, double lambda)
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
      assert( nnet_update(NN, eta/(double)n, eta*lambda/(double)ds->len) == EXIT_SUCCESS );
      i += n;
    }
  
  return EXIT_SUCCESS;
}


/*

  Dump

*/


int nnet_dump(NNet* NN, char* filename)
{
  int fd;
  unsigned int i;

  assert(NN != NULL);
  assert(filename != NULL);

  fd = open(filename,O_CREAT | O_WRONLY | O_TRUNC, 0666);
  assert(fd != -1);

  assert(write(fd,&(NN->n_layers),sizeof(unsigned int)) != -1);

  for(i=0;i<NN->n_layers;i++)
    {
      assert(write(fd,&(NN->n_neurons[i]),sizeof(unsigned int)) != -1);
    }

  close(fd);

  return EXIT_SUCCESS;
}
