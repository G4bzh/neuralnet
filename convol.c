/*

  Convolutional Layer

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "functions.h"
#include "neuron.h"
#include "convol.h"


/*

  Creation

*/

CONVOL* convol_create(ACTIVATION act, unsigned int in_w, unsigned int in_h, unsigned int cv_w, unsigned int cv_h, unsigned int depth, Neuron** in[])
{
  assert(in_w);
  assert(in_h);
  assert(cv_w);
  assert(cv_h);
  assert(depth);
  assert(in != NULL);

  CONVOL* cv;
  Neuron** p;
  unsigned int d,i,j,n,col,row,x,y;

  cv = (CONVOL*)malloc(sizeof(CONVOL));
  if (cv == NULL)
    {
      return NULL;
    }

  /* Shared weights (+ bias) */
  cv->weights = (double*)malloc((depth*cv_w*cv_h+1)*sizeof(double));
  if (cv->weights == NULL)
    {
      goto err1;
    }

  for(i=0;i<cv_w*cv_h;i++)
    {
      cv->weights[i] = ((double)(rand()-RAND_MAX/2)/(double)RAND_MAX);
    }
  cv->weights[i] = 1.0; /* Bias */
  cv->n_weights = depth*cv_w*cv_h;

  /* Convolution matrix */
  cv->cv_w = cv_w;
  cv->cv_h = cv_h;


 /* Neurons */
  n = (in_w - cv_w + 1)*(in_h - cv_h + 1);
  cv->neurons = (Neuron**)malloc(n*sizeof(Neuron*));
  if(cv->neurons == NULL)
    {
      goto err2;
    }


  /* Convolutional Helper */
  i=0;
  p = (Neuron**)malloc(depth*cv_h*cv_w*sizeof(Neuron*));
  if (p == NULL)
    {
      goto err3;
    }
  
  /* Convolutions */
 
  for(i=0;i<n;i++)
    {
      for(d=0;d<depth;d++)
	{
	  col = i % (in_w - cv_w + 1);
	  row = i / (in_w - cv_w + 1);
	  
	  
	  /* Convolutional predecessors */
	  for(y=row;y<row+cv_h;y++)
	    {
	      for(x=col;x<col+cv_w;x++)
		{
		  p[((x-col)+(y-row)*cv_w)+(d*cv_w*cv_h)] = in[d][x+y*in_w];
		}
	    }
	}  
	 
      cv->neurons[i] = neuron_create(cv->n_weights,act,cv->weights,p);
      if (cv->neurons[i] == NULL)
	{
	  goto err4;
	} 
    }

  free(p);
  cv->n_neurons = n;

  /* Predecessor geometry */
  cv->in_w = in_w;
  cv->in_h = in_h;


  return cv;

 err4:
  free(p);
  
 err3:
  for(j=0;j<i;j++)
    {
      neuron_delete(cv->neurons[j]);
    }
  free(cv->neurons);

 err2:
  free(cv->weights);

 err1:
  free(cv);

  return NULL;


}


/*

  Deletion

*/

int convol_delete(CONVOL* cv)
{
  unsigned int i;

  if (cv == NULL)
    {
      return EXIT_FAILURE;
    }
  
  for(i=0;i<cv->n_neurons;i++)
    {
      cv->neurons[i]->weights = NULL;
      neuron_delete(cv->neurons[i]);
    }
  
  free(cv->neurons);
  free(cv->weights);
  free(cv);

  return EXIT_SUCCESS;

}


/*

  Feedforward

*/

int convol_feedforward(CONVOL* cv)
{
  unsigned int i;

  if (cv == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<cv->n_neurons;i++)
    {
      neuron_feedforward(cv->neurons[i]);
    }
        
  return EXIT_SUCCESS;
}


/*

  Backpropagation

*/

int convol_backpropagation(CONVOL* cv)
{
  unsigned int i;

  if (cv == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<cv->n_neurons;i++)
    {
      neuron_backpropagation(cv->neurons[i],NULL);
    }

  return EXIT_SUCCESS;
}


/*

  Update

*/

int convol_update(CONVOL* cv, double l, double r, double (*reg)(double,double))
{
  unsigned int i;

  if ( (cv == NULL) || (reg == NULL) )
    {
      return EXIT_FAILURE;
    }

  /* Update weights */
  for(i=0;i<cv->n_neurons;i++)
    {
      neuron_update(cv->neurons[i],l/cv->n_weights,r/cv->n_weights,reg);
    }
  
  return EXIT_SUCCESS;
}
