/*

  Feed Forward Neural Network

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
#include "ffnnet.h"



/*

  Cost Functions

*/

double ffnnet_quadratic(FFNNet* NN, double* a, unsigned int i)
{
  assert(NN != NULL);
  assert(a != NULL);

  return  (NN->layers[NN->n_layers-1][i]->output - a[i])* NN->layers[NN->n_layers-1][i]->z_derivative;
}

double ffnnet_crossentropy(FFNNet* NN, double* a, unsigned int i)
{
  assert(NN != NULL);
  assert(a != NULL);

  return  (NN->layers[NN->n_layers-1][i]->output - a[i]);
}


double (*costs[2])(FFNNet*, double*, unsigned int)= {ffnnet_quadratic, ffnnet_crossentropy};


/*

  Regularization Functions

 */


double ffnnet_regL2(double r, double w)
{
  return r*w;
}

double ffnnet_regL1(double r, double w)
{
  if (w)
    {
      return r*(w>0?1:-1);
    }

  return 0.0;
}

double ffnnet_regNone(double r, double w)
{
  return 0.0;
}

double (*regs[3])(double,double) = {ffnnet_regNone, ffnnet_regL1, ffnnet_regL2};



/*

  Creation

*/

FFNNet* ffnnet_create(Cost c, Reg r, unsigned int n, unsigned int* a)
{
  FFNNet* NN;
  unsigned int i,j,k,l;

  /* First parameter is the number of layers */
  assert(n > 1);
  assert(a != NULL);

  NN = (FFNNet*)malloc(sizeof(FFNNet));
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
  
  
  NN->layers = (Neuron***)malloc(n*sizeof(Neuron**));
  if (NN->layers == NULL)
    {
      goto err1;
    }			    

  j=0;
  for(i=0;i<n;i++)
    {
 
      NN->layers[i] = (Neuron**)malloc(NN->n_neurons[i]*sizeof(Neuron*));
      if (NN->layers[i] == NULL)
      	{
      	  goto err2;
      	}
     
      for(j=0;j<NN->n_neurons[i];j++)
      {
      	NN->layers[i][j] = neuron_create((i==0?0:NN->n_neurons[i-1]),NULL,(i==0?NULL:NN->layers[i-1]));
      	if (NN->layers[i][j] == NULL)
      	  {
      	    goto err2;
      	  }
      }
    }

  NN->n_layers = n;
  NN->cost = c;
  NN->reg = r;

  return NN;

 err2:
  for(k=0;k<i;k++)
    {
      for(l=0;l<j;l++)
	{
	  neuron_delete(NN->layers[k][l]);
	}
      free(NN->layers[k]);
    }

  free(NN->layers);


 err1:
  free(NN->n_neurons);

 err0:
  free(NN);

  return NULL;

}


/*

  Deletion

*/

int ffnnet_delete(FFNNet* NN)
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
  free(NN);
  
  return EXIT_SUCCESS;

}


/*

  Feedforward

*/

int ffnnet_feedforward(FFNNet* NN, double* in)
{
  unsigned int i,j;

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
      #pragma omp parallel for
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_feedforward( NN->layers[i][j] );
	}
      
    }
 
  return EXIT_SUCCESS;
}


int ffnnet_backpropagation(FFNNet* NN, double* out)
{
  unsigned int i,j;
  double e;

  assert(NN != NULL);
  assert(out != NULL);

  /* Last layer error */
  for(j=0;j<NN->n_neurons[NN->n_layers-1];j++)
    {
      e = (costs[NN->cost])(NN,out,j);
      neuron_backpropagation(NN->layers[NN->n_layers-1][j], &e); 
    }
  
  /* Error backpropagation */
  for(i=NN->n_layers-2;i>0;i--)
    {
      #pragma omp parallel for
      for(j=0;j<NN->n_neurons[i];j++)
	{

	  neuron_backpropagation(NN->layers[i][j], NULL);

	}
    }

 
  return EXIT_SUCCESS;
}


/*

  Update (gradient descent)

*/

int ffnnet_update(FFNNet* NN, double l, double r)
{
  unsigned int i,j;

  assert(NN != NULL);
  assert(l);

  for(i=NN->n_layers-1;i>0;i--)
    {
      #pragma omp parallel for
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_update(NN->layers[i][j],l,r,regs[NN->reg]);
	}
    }

  return EXIT_SUCCESS;
}


/*

  Mini-batch

*/

int ffnnet_minibatch(FFNNet* NN, Dataset* ds, unsigned int n, double eta, double lambda)
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
	  assert( ffnnet_feedforward(NN,ds->in[j]) == EXIT_SUCCESS );
	  assert( ffnnet_backpropagation(NN,ds->out[j])  == EXIT_SUCCESS );
	}
      assert( ffnnet_update(NN, eta/(double)n, eta*lambda/(double)ds->len) == EXIT_SUCCESS );
      i += n;
    }
  
  return EXIT_SUCCESS;
}


/*

  Dump

*/


int ffnnet_dump(FFNNet* NN, char* filename)
{
  int fd;
  unsigned int i,j;

  assert(NN != NULL);
  assert(filename != NULL);

  fd = open(filename,O_CREAT | O_WRONLY | O_TRUNC, 0666);
  assert(fd != -1);

  assert(write(fd,&(NN->n_layers),sizeof(unsigned int)) != -1);
  assert(write(fd,&(NN->cost),sizeof(unsigned int)) != -1);
  assert(write(fd,&(NN->reg),sizeof(unsigned int)) != -1);

  for(i=0;i<NN->n_layers;i++)
    {
      assert(write(fd,&(NN->n_neurons[i]),sizeof(unsigned int)) != -1);
    }

  for(i=0;i<NN->n_layers;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_dump(fd,NN->layers[i][j]);
	}
    }

  close(fd);

  return EXIT_SUCCESS;
}


/*
  
  Restore

*/

FFNNet* ffnnet_restore(char* filename)
{
  FFNNet* NN;
  int fd;
  unsigned int i,j, n,c,r;
  unsigned int* a;

  assert(filename != NULL);

  fd = open(filename,O_RDONLY);
  assert(fd != -1);

  assert(read(fd,&n,sizeof(unsigned int)) != -1);
  assert(read(fd,&c,sizeof(unsigned int)) != -1);
  assert(read(fd,&r,sizeof(unsigned int)) != -1);

  a = (unsigned int*)malloc(n*sizeof(unsigned int));
  assert(a != NULL);

  for(i=0;i<n;i++)
    {
      if (read(fd,&(a[i]),sizeof(unsigned int)) == -1)
	{
	  goto err0;
	}
    }
  
  NN = ffnnet_create(c,r,n,a);
  if (NN == NULL)
    {
      goto err0;
    }
 
  for(i=0;i<n;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  if (neuron_restore(fd,NN->layers[i][j]) != EXIT_SUCCESS)
	    {
	      goto err1;
	    }
	}
    }


  free(a);

  return NN;
  
 err1:
  
  ffnnet_delete(NN);

 err0:

  free(a);
  
return NULL;
}
