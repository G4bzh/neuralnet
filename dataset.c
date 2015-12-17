/*

  Dataset manipulation

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "dataset.h"


/*

  Creation

*/


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


  ds->len = len;
  ds->in_len = in_len;
  ds->out_len = out_len;
  ds->cursor = 0;

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


/*

  Deletion

*/


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

/*

  Print

*/

int dataset_print(Dataset* ds)
{
  unsigned int i,j;

  assert(ds != NULL);

  for(i=0;i<ds->len;i++)
    {
      printf("[");
      for(j=0;j<ds->in_len;j++)
	{
	  printf(" %f ",ds->in[i][j]);
	}
      printf("] -> [");
      for(j=0;j<ds->out_len;j++)
	{
	  printf(" %f ",ds->out[i][j]);
	}
      printf("]\n");
    }
  
  return EXIT_SUCCESS;
}


/*

  Add

*/

int dataset_add(Dataset* ds, unsigned int n, ...)
{
   va_list valist;
   unsigned int i;

   assert(ds->cursor < ds->len);
   assert( n == ds->in_len+ds->out_len);

   va_start(valist, n);

   for(i=0;i<ds->in_len;i++)
    {     
      ds->in[ds->cursor][i] = va_arg(valist,double);
    }

   for(i=0;i<ds->out_len;i++)
    {     
      ds->out[ds->cursor][i] = va_arg(valist,double);
    }

  va_end(valist);
  ds->cursor++;

  return EXIT_SUCCESS;

}

/*

  Shuffle

*/


int dataset_shuffle(Dataset* ds)
{
  unsigned int i,j;
  
  assert(ds != NULL);

  for(i=0;i<ds->len;i++)
    {
      j = (unsigned int)(((double)rand()/(double)RAND_MAX)*ds->len);
      /* XOR swap */
      if ( (i != j) && (j < ds->len) )
	{
	  double* tmp;

	  tmp = ds->in[i];
	  ds->in[i] = ds->in[j];
	  ds->in[j] = tmp;

	  tmp = ds->out[i];
	  ds->out[i] = ds->out[j];
	  ds->out[j] = tmp;

	}
      
    }

  return EXIT_SUCCESS;
}
