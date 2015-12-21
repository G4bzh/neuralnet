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
  unsigned int i,epoch;
 

  srand(time(NULL));
  epoch = 3000; /* Must be great */


  printf("Loading train images...");
  fflush(stdout);
  DS = mnist_load("../mnist/train-images-idx3-ubyte","../mnist/train-labels-idx1-ubyte");
  if (DS == NULL)
    {
      goto err0;
    }
  printf("Done !\n");

  printf("Loading test images...");
  fflush(stdout);
  DS_Test = mnist_load("../mnist/t10k-images-idx3-ubyte","../mnist/t10k-labels-idx1-ubyte");
  if ( DS_Test == NULL)
    {
      goto err1;
    }
  printf("Done !\n");



  NN = nnet_create(3,DS->in_len,30,DS->out_len);
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
      printf("epoch %d : %u/%u\n", i,mnist_evaluate(NN,DS_Test),DS_Test->len);
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
