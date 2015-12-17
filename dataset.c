/*

  Dataset manipulation

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dataset.h"


Dataset* dataset_create(unsigned int len, unsigned int in_len, unsigned int out_len)
{
  Dataset* ds;

  assert(len);
  assert(in_len);
  assert(out_len);

  ds = (Dataset*)malloc(sizeof(Dataset));
  if (ds == NULL)
    {
      return NULL;
    }

  return ds;
}


int dataset_delete(Dataset* ds)
{
  if (ds == NULL)
    {
      return EXIT_FAILURE;
    }

  free(ds);

  return EXIT_SUCCESS;
}
