/*

  Main

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>  
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
  int e,max = 0;
  double elapsed = 0.0;
  struct timeval end_tv, start_tv;

  srand(time(NULL));
  epoch = 30;


  printf("Loading train images...");
  fflush(stdout);
  DS = mnist_load("../mnist/train-images-idx3-ubyte","../mnist/train-labels-idx1-ubyte");
  if (DS == NULL)
    {
      return EXIT_FAILURE;
    }
  printf("Done !\n");



  printf("Loading test images...");
  fflush(stdout);
  DS_Test = mnist_load("../mnist/t10k-images-idx3-ubyte","../mnist/t10k-labels-idx1-ubyte");
  if ( DS_Test == NULL)
    {
      goto err0;
    }
  printf("Done !\n");


  unsigned int layers[] = {DS->in_len,100,DS->out_len};
  NN = nnet_create(COST_CROSSENTROPY,REG_L2,3,layers);
  if (NN == NULL)
    {
      goto err1;
    }

  printf("Training started\n");
  gettimeofday(&start_tv, NULL);

  for(i=0;i<epoch;i++)
    {
      if (dataset_shuffle(DS) != EXIT_SUCCESS)
  	{
  	  goto err2;
  	}
      nnet_minibatch(NN,DS,10,0.1,5.0);
           
      e = mnist_evaluate(NN,DS_Test);
      printf("epoch %d: %u/%u", i,e,DS_Test->len);
      if (e > max)
      	{
      	  max =  e;
      	  assert(nnet_dump(NN,"dump") == EXIT_SUCCESS);
      	  printf(" (*)");
      	}
      printf("\n");
    }

  gettimeofday(&end_tv, NULL);
  elapsed = (end_tv.tv_sec - start_tv.tv_sec) + (end_tv.tv_usec - start_tv.tv_usec) / 1000000.0;
  printf("Training ended (%f s.)\n",elapsed);

  nnet_delete(NN);
  dataset_delete(DS);

  NN = nnet_restore("dump");
  assert(NN != NULL);
  printf("Restored : %u/%u\n",mnist_evaluate(NN,DS_Test),DS_Test->len);
  
  nnet_delete(NN);
  dataset_delete(DS_Test);

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
