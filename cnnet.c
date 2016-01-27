/*

  Convolutional Neural Network

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "neuron.h"
#include "cnnet.h"



/*

  CVLayer creation

*/

CVLayer* cvlayer_create(unsigned int w, unsigned int h, CNNet* parent)
{
  assert(w);
  assert(h);
  assert(parent != NULL);
  assert(parent->in_w);
  assert(parent->in_h);
 

  CVLayer* cv;
  Neuron** p;
  unsigned int i,j,n,col,row,x,y;
  
  cv = (CVLayer*)malloc(sizeof(CVLayer));
  if (cv == NULL)
  {
    return NULL;
  }

  /* Shared weights (+ bias) */
  cv->weights = (double*)malloc((w*h+1)*sizeof(double));
  if (cv->weights == NULL)
    {
      goto err1;
    }

  for(i=0;i<w*h;i++)
    {
      cv->weights[i] = ((double)(rand()-RAND_MAX/2)/(double)RAND_MAX);
    }
  cv->weights[i] = 1.0; /* Bias */
  cv->n_weights = w*h;


  /* Convolution matrix */
  cv->cv_w = w;
  cv->cv_h = h;


  /* Neurons */
  n = (parent->in_w - w + 1)*(parent->in_h - h + 1);
  cv->neurons = (Neuron**)malloc(n*sizeof(Neuron*));
  if(cv->neurons == NULL)
    {
      goto err2;
    }
  
  /* Helper */
  i=0;
  p = (Neuron**)malloc(h*w*sizeof(Neuron*));
  if (p == NULL)
    {
      goto err3;
    }

  for(i=0;i<n;i++)
    {
      col = i % (parent->in_h - h + 1);
      row = i / (parent->in_w - w + 1);
      
      /* Convolutional predecessors */
      for(y=row;y<row+w;y++)
	{
	  for(x=col;x<col+w;x++)
	    {
	      p[(x-col)+(y-row)*w] = parent->inlayer[x+y*parent->in_w];
	    }
	}

      cv->neurons[i] = neuron_create(h*w,cv->weights,p);
      if (cv->neurons[i] == NULL)
	{
	  goto err4;
	}
    }

  free(p);
  cv->n_neurons = n;

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

  fflush(stdout);
  return NULL;
  
}


/*

  CVLayer deletion

*/

int cvlayer_delete(CVLayer* cv)
{
  unsigned int i;

  if (cv == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<cv->n_neurons;i++)
    {
      neuron_delete(cv->neurons[i]);
    }

  free(cv->neurons);
  free(cv->weights);
  free(cv);

  return EXIT_SUCCESS;
}


/*

  Creation

*/


CNNet* cnnet_create(unsigned int in_w, unsigned int in_h, unsigned int cv, unsigned int cv_w, unsigned int cv_h)
{
  assert(cv);
  assert(in_w);
  assert(in_h);
  assert(cv_w);
  assert(cv_h);

  CNNet* CNN;
  unsigned int i_in,i_cv,j;

  CNN = (CNNet*)malloc(sizeof(CNNet));
  if (CNN == NULL)
  {
    return NULL;
  }

  /* Input layer */
  CNN->inlayer = (Neuron**)malloc(in_w*in_h*sizeof(Neuron*));
  if (CNN->inlayer == NULL)
    {
      goto err1;
    }

  for(i_in=0;i_in<in_w*in_h;i_in++)
    {
      CNN->inlayer[i_in] = neuron_create(0,NULL,NULL);
      if (CNN->inlayer[i_in] == NULL)
	{
	  goto err2;
	}

    }
  CNN->in_w = in_w;
  CNN->in_h = in_h;


  /* Convolutional layers */
  CNN->cvlayers = (CVLayer**)malloc(cv*sizeof(CVLayer*));
  if (CNN->inlayer == NULL)
    {
      goto err2;
    }

  for(i_cv=0;i_cv<cv;i_cv++)
    {
      CNN->cvlayers[i_cv] = cvlayer_create(cv_w,cv_h,CNN);
      if (CNN->cvlayers[i_cv] == NULL)
	{
	  goto err3;
	}
    }
  CNN->n_conv = cv;



  return CNN;



 err3:
  for(j=0;j<i_cv;j++)
    {
      cvlayer_delete(CNN->cvlayers[j]);
    }
  free(CNN->cvlayers);

 err2:
  for(j=0;j<i_in;j++)
    {
      neuron_delete(CNN->inlayer[j]);
    }
  free(CNN->inlayer);

 err1:
  free(CNN);

  return NULL;

}



/* 

   Deletion

*/

int cnnet_delete(CNNet* CNN)
{
  unsigned int i;

  if (CNN == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<CNN->n_conv;i++)
    {
      cvlayer_delete(CNN->cvlayers[i]);
    }

  free(CNN->cvlayers);

  for(i=0;i<CNN->in_h*CNN->in_w;i++)
    {
      neuron_delete(CNN->inlayer[i]);
    }

  free(CNN->inlayer);
  free(CNN);

  return EXIT_SUCCESS;
}
