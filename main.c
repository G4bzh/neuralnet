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


int main( int argc, char* argv[])
{
  
  Dataset* DS;
  NNet* NN;
  unsigned int i,n, epoch;

  srand(time(NULL));
  n = 4000;
  epoch = 100;


  DS = dataset_create(n,1,1);
  assert( DS != NULL );

  for(i=0;i<n;i++)
    {
      if (dataset_add(DS,2,(double)i,(double)(i+1)) != EXIT_SUCCESS)
	{
	  goto err0;
	}
    }



  NN = nnet_create(3,1,10,1);
  if (NN == NULL)
    {
      goto err0;
    }

  
  for(i=0;i<epoch;i++)
    {
      if (dataset_shuffle(DS) != EXIT_SUCCESS)
	{
	  goto err1;
	}
      nnet_minibatch(NN,DS,10,3.0);
      printf("Epoch %u/%u done\n",i,epoch);
    }

  nnet_delete(NN);
  dataset_delete(DS);

  return EXIT_SUCCESS;

 err1:

  nnet_delete(NN);

 err0:

  dataset_delete(DS);

  printf("Something bad has happened ...\n");

  return EXIT_FAILURE;

}
