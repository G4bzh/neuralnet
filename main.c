/*

  Main

*/


#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "nnet.h"


double identity(double x)
{
  return x;
}

int main( int argc, char* argv[])
{
  Neuron* N;
  NNet* NN;

  N = neuron_create(5,identity);
  if (N==NULL)
    {
      printf("Unable to create neuron !\n");
      return EXIT_FAILURE;
    }

  neuron_init(N);

  neuron_print(N);
  neuron_delete(N);


  NN = nnet_create(4);
  if (NN==NULL)
    {
      printf("Unable to create neuron network !\n");
      return EXIT_FAILURE;
    }

  nnet_delete(NN);
  

  return EXIT_SUCCESS;
}
