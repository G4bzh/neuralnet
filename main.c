/*

  Main

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "dataset.h"
#include "nnet.h"
#include "mnist.h"


int main( int argc, char* argv[])
{
  
  Dataset* DS;
  Dataset* DS_Test;
  NNet* NN;
  unsigned int i,n,epoch;
  double data[4][4] = { {1.0,1.0,0.0} , {1.0,0.0,1.0}, {0.0,1.0,1.0}, {0.0,0.0,0.0} };

  srand(time(NULL));
  n = 4;
  epoch = 10000; /* Must be great */


  DS = dataset_create(n,2,1);
  assert( DS != NULL );

  for(i=0;i<n;i++)
    {
      if (dataset_add(DS,data[i]) != EXIT_SUCCESS)
	{
	  goto err0;
	}
    }
 

 
  DS_Test = dataset_create(n,2,1);
  if( DS_Test == NULL )
    {
      goto err0;
    }

  for(i=0;i<n;i++)
    {
      if (dataset_add(DS_Test,data[i]) != EXIT_SUCCESS)
	{
	  goto err1;
	}
    }


  NN = nnet_create(3,2,3,1);
  if (NN == NULL)
    {
      goto err1;
    }

  
  for(i=0;i<epoch;i++)
    {
      if (dataset_shuffle(DS) != EXIT_SUCCESS)
	{
	  goto err2;
	}
      nnet_minibatch(NN,DS,2,1.0);
     
    }

  nnet_evaluate(NN,DS_Test);

  nnet_delete(NN);
  dataset_delete(DS_Test);
  dataset_delete(DS);


  //mnist_images("../mnist/train-images-idx3-ubyte");
  //mnist_labels("../mnist/train-labels-idx1-ubyte");

  return EXIT_SUCCESS;

 err2:

  nnet_delete(NN);

 err1:
  
   dataset_delete(DS_Test);

 err0:

  dataset_delete(DS);

  printf("Something bad has happened ...\n");

  return EXIT_FAILURE;

}
