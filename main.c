/*

  Main

*/


#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "nnet.h"


int main( int argc, char* argv[])
{

  NNet* NN;


  NN = nnet_create(4);
  if (NN==NULL)
    {
      printf("Unable to create neuron network !\n");
      return EXIT_FAILURE;
    }

  NN->layers[0] = layer_create(3);
  if (NN->layers[0] == NULL)
    {
      printf("Unable to create layer 0\n");
      goto err;
    }

  NN->layers[1] = layer_create(5);
  if (NN->layers[1] == NULL)
    {
      printf("Unable to create layer 1\n");
      goto err;
    }

  NN->layers[2] = layer_create(5);
  if (NN->layers[2] == NULL)
    {
      printf("Unable to create layer 2\n");
      goto err;
    }

  NN->layers[3] = layer_create(4);
  if (NN->layers[3] == NULL)
    {
      printf("Unable to create layer 3\n");
      goto err;
    }


  nnet_print(NN);

 err:
  nnet_delete(NN);
  

  return EXIT_SUCCESS;
}
