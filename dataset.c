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
  unsigned int i,j;

  assert(len);
  assert(in_len);
  assert(out_len);

  ds = (Dataset*)malloc(sizeof(Dataset));
  if (ds == NULL)
    {
      return NULL;
    }

  ds->in = (double**)malloc(len*sizeof(double*));
  if (ds->in == NULL)
    {
      goto err0;
    }

  ds->out = (double**)malloc(len*sizeof(double*));
  if (ds->out == NULL)
    {
      goto err1;
    }

  for(i=0;i<len;i++)
    {
      ds->in[i] = (double*)malloc(in_len*sizeof(double));
      ds->out[i] = (double*)malloc(out_len*sizeof(double));

      if ( (ds->in == NULL) || (ds->out == NULL) )
	{
	  goto err2;
	}
    }

  return ds;

 err2:

  for (j=0;j<i;j++)
    {
      free(ds->in[i]);
      free(ds->out[i]);     
    }
  free(ds->out);

 err1:
  free(ds->in);

 err0:
  free(ds);

  return NULL;
}


int dataset_delete(Dataset* ds)
{
  unsigned int i;

  if (ds == NULL)
    {
      return EXIT_FAILURE;
    }

  for(i=0;i<ds->len;i++)
    {
      free(ds->in[i]);
      free(ds->out[i]);
    }
  
  free(ds->in);
  free(ds->out);
  free(ds);

  return EXIT_SUCCESS;
}
