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
  
  NNet* NN;
  Dataset* DS;
  double in[] = {0.5,0.5};
  double out[] = {0,1};

  srand(time(NULL));

  DS = dataset_create(2,2,2);
  
  NN = nnet_create(4,2,3,3,2);
  
  nnet_feedforward(NN,in);

  nnet_backpropagation(NN,out);

  nnet_update(NN,3);

  nnet_delete(NN);

  dataset_delete(DS);

  return EXIT_SUCCESS;
}
