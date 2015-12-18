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
  unsigned int i,n,epoch;

  srand(time(NULL));
  n = 4;
  epoch = 10;


  DS = dataset_create(n,2,1);
  assert( DS != NULL );

  if (dataset_add(DS,3,1.0,1.0,1.0) != EXIT_SUCCESS)
    {
      goto err0;
    }
  if (dataset_add(DS,3,1.0,0.0,0.0) != EXIT_SUCCESS)
    {
      goto err0;
    }
  if (dataset_add(DS,3,0.0,1.0,0.0) != EXIT_SUCCESS)
    {
      goto err0;
    }
  if (dataset_add(DS,3,0.0,0.0,0.0) != EXIT_SUCCESS)
    {
      goto err0;
    }

 
  DS_Test = dataset_create(n,2,1);
  if( DS_Test == NULL )
    {
      goto err0;
    }
  if (dataset_add(DS_Test,3,1.0,1.0,1.0) != EXIT_SUCCESS)
    {
      goto err1;
    }
  if (dataset_add(DS_Test,3,1.0,0.0,0.0) != EXIT_SUCCESS)
    {
      goto err1;
    }
  if (dataset_add(DS_Test,3,0.0,1.0,0.0) != EXIT_SUCCESS)
    {
      goto err1;
    }
  if (dataset_add(DS_Test,3,0.0,0.0,0.0) != EXIT_SUCCESS)
    {
      goto err1;
    }



  NN = nnet_create(3,2,2,1);
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
      nnet_minibatch(NN,DS,2,0.5);
      printf("Epoch %u/%u : %u/%u\n",i+1,epoch, nnet_evaluate(NN,DS_Test),DS_Test->len);
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
