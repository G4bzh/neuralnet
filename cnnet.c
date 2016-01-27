/*

  Convolutional Neural Network

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "neuron.h"
#include "cnnet.h"


CNNet* cnnet_create(unsigned int in, unsigned int cv, unsigned int w, unsigned int h)
{
  assert(in);
  assert(cv);
  assert(w);
  assert(h);

  return NULL;
}


int cnnet_delete(CNNet* CNN)
{
  if (CNN == NULL)
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
