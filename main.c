/*

  Main

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "nnet.h"


int main( int argc, char* argv[])
{
  
  NNet* NN;

  srand(time(NULL));
  
  NN = nnet_create(4,3,4,5,2);
  nnet_print(NN);
  nnet_delete(NN);

  return EXIT_SUCCESS;
}
