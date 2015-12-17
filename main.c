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
  Dataset* DS_Test;
  NNet* NN;
  unsigned int i,n,ntest,epoch;

  srand(time(NULL));
  n = 4000;
  ntest = 100;
  epoch = 1000;


  DS = dataset_create(n,1,1);
  assert( DS != NULL );

  for(i=1;i<n+1;i++)
    {
      if (dataset_add(DS,2,(double)i/(double)(n+1),(double)(i-1)/(double)(n+1)) != EXIT_SUCCESS)
	{
	  goto err0;
	}
    }


  DS_Test = dataset_create(ntest,1,1);
  if( DS_Test == NULL )
    {
      goto err0;
    }

  for(i=0;i<ntest;i++)
    {
      if (dataset_add(DS_Test,2,(double)(i+n+1)/(double)(ntest+n),(double)(i+n-1)/(double)(ntest+n+1)) != EXIT_SUCCESS)
	{
	  goto err1;
	}
    }


  NN = nnet_create(3,1,2,1);
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
      nnet_minibatch(NN,DS,10,3.0);
      printf("Epoch %u/%u : %u/%u\n",i,epoch, nnet_evaluate(NN,DS_Test),DS_Test->len);
    }

  nnet_delete(NN);
  dataset_delete(DS_Test);
  dataset_delete(DS);


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
