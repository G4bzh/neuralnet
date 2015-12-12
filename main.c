/*

  Main

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "nnet.h"


int main( int argc, char* argv[])
{
  assert(argc > 3);

  NNet* NN;
  Layer* L;
  unsigned int i,n_layers;

  n_layers = (unsigned int)atoi(argv[1]);
  assert( n_layers == argc - 2 );

  NN = nnet_create(n_layers);
  assert(NN != NULL);

  for(i=0;i<n_layers;i++)
    {
      L = layer_create((unsigned int)atoi(argv[i+2]));
      if (L == NULL)
	{
	   nnet_delete(NN);
	   return EXIT_FAILURE;
	}

      nnet_set_layer(NN,i,L);

    }
  
  nnet_print(NN);


  nnet_delete(NN);

  return EXIT_SUCCESS;
}
