/*

  Convolutional Layer

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "convol.h"


/*

  Creation

*/

CONVOL* convol_create(unsigned int in_w, unsigned int in_h, unsigned int cv_w, unsigned int cv_h, Neuron** prev)
{
  assert(in_w);
  assert(in_h);
  assert(cv_w);
  assert(cv_h);
  assert(prev != NULL);

  CONVOL* cv;

  cv = (CONVOL*)malloc(sizeof(CONVOL));
  if (cv == NULL)
    {
      return NULL;
    }

  return cv;
}


/*

  Deletion

*/

int convol_delete(CONVOL* cv)
{
  if (cv == NULL)
    {
      return EXIT_FAILURE;
    }

  free(cv);

  return EXIT_SUCCESS;

}
