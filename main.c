/*

  Main

*/


#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"


double identity(double x)
{
  return x;
}

int main( int argc, char* argv[])
{
  Neuron* N;

  N = neuron_create(5,identity);
  if (N==NULL)
    {
      printf("Unable to create neuron !\n");
      return EXIT_FAILURE;
    }

  neuron_init(N);

  neuron_print(N);
  neuron_delete(N);

  return EXIT_SUCCESS;
}
