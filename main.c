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
  unsigned int i,j;
  double in[] = {0.5,0.5};

  srand(time(NULL));
  
  NN = nnet_create(4,2,3,2,1);
  
  for(i=0;i<NN->n_layers;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_print(NN->layers[i][j]);
	}
      printf("\n");
    }

      
  nnet_feedforward(NN,in);


  for(i=0;i<NN->n_layers;i++)
    {
      for(j=0;j<NN->n_neurons[i];j++)
	{
	  neuron_print(NN->layers[i][j]);
	}
      printf("\n");
    }


  nnet_delete(NN);

  return EXIT_SUCCESS;
}
