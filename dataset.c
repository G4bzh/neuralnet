/*

  Dataset manipulation

*/


#include <stdlib.h>
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

int dataset_add(Dataset* ds, double* a)
{
   unsigned int i;

   assert(ds->cursor < ds->len);

   for(i=0;i<ds->in_len;i++)
    {     
      ds->in[ds->cursor][i] = a[i];
    }

   for(i=0;i<ds->out_len;i++)
    {     
      ds->out[ds->cursor][i] = a[ds->in_len+i];
    }

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

/*

  Convolution

*/

Dataset* dataset_2Dconvolution(Dataset* orig_DS, unsigned int orig_rows, unsigned int orig_cols, unsigned int conv_rows, unsigned int conv_cols)
{
  assert( orig_DS != NULL );
  assert( conv_cols );
  assert( conv_rows );
  assert( orig_cols > conv_cols );
  assert( orig_rows > conv_rows );
  

  Dataset* DS;
  unsigned int row,col,k,u,n,x,y;
  double* a;

  /* Number of convolutions for 1 input */
  n = (orig_rows - conv_rows + 1)*(orig_cols - conv_cols + 1);

  DS = dataset_create(n*orig_DS->len,conv_rows*conv_cols,orig_DS->out_len);
  if (DS == NULL)
    {
      return NULL;
    }

  a = (double*)malloc((conv_rows*conv_cols+orig_DS->out_len)*sizeof(double));
  if (a == NULL)
    {
      dataset_delete(DS);
      return NULL;
    }

  for(k=0;k<orig_DS->len;k++)
    {
      
      for(u=0;u<n;u++)
	{
	  
	  col = u % (orig_rows - conv_rows + 1);
	  row = u / (orig_cols - conv_cols + 1);

	  for(y=row;y<row+conv_cols;y++)
	    {
	      for(x=col;x<col+conv_cols;x++)
		{
		  a[(x-col)+(y-row)*conv_cols] = DS->in[k][x+y*orig_cols];
		}

	    }
	  
	  for(x=0;x<DS->out_len;x++)
	    {
	      a[conv_cols*conv_rows+x] = DS->out[k][x];
	    }
	  

	  if ( dataset_add(DS,a) != EXIT_SUCCESS )
	    {
	      free(a);
	      dataset_delete(DS);
	      return NULL;
	    }

	}

    }
  

  free(a);

  return DS;
}
