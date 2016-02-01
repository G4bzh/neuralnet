/*

  MNIST Database

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include "dataset.h"
#include "mnist.h"


#define MNIST_IMAGES_MAGIC    0x803
#define MNIST_LABELS_MAGIC    0x801
#define MNIST_VALUES          10



/*

  Load mnist database

*/


Dataset* mnist_load(char* filename_i, char* filename_l)
{
  int fd_i,fd_l;
  int n_i,n_l,rows,cols,i,j,k,l;
  unsigned char c;
  double* a;
  Dataset* ds;

  assert(filename_i != NULL);
  assert(filename_l != NULL);
  fd_i = open(filename_i,O_RDONLY);
  assert(fd_i != -1);
  fd_l = open(filename_l,O_RDONLY);
  assert(fd_l != -1);




  if (read(fd_i,&n_i,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if ( ntohl(n_i) != MNIST_IMAGES_MAGIC )
    {
      goto err0;
    } 

  if (read(fd_l,&n_l,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if ( ntohl(n_l) != MNIST_LABELS_MAGIC )
    {
      goto err0;
    }


  if (read(fd_i,&n_i,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  n_i = ntohl(n_i);


  if (read(fd_l,&n_l,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  n_l = ntohl(n_l);
 
  if( n_i != n_l)
    {
      goto err0;
    }


  // n_i = n_l = 100; /* DEV ONLY */

  if (read(fd_i,&rows,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if (read(fd_i,&cols,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  rows = ntohl(rows);
  cols = ntohl(cols);

  a = (double*)malloc((rows*cols+MNIST_VALUES)*sizeof(double));
  if (a == NULL)
    {
      goto err0;
    }

  ds = dataset_create(n_i,rows*cols,MNIST_VALUES);
  if (ds == NULL)
    {
      goto err1;
    }

  for(k=0;k<n_i;k++)
    {
      for(i=0;i<rows;i++)
	{
	  for(j=0;j<cols;j++)
	    {
	      if (read(fd_i,&c,sizeof(unsigned char)) != sizeof(unsigned char))
		{
		  goto err2;
		}
	      
	      a[rows*i+j] = (double)c/(double)255;
	    }
	}

      if (read(fd_l,&c,sizeof(unsigned char)) != sizeof(unsigned char))
	{
	  goto err2;
	}

      for(l=0;l<MNIST_VALUES;l++)
	{
	  a[(rows*cols)+l] = (l==c?1:0);
	}

      if ( dataset_add(ds,a) == EXIT_FAILURE )
	{
	  goto err2;
	}

    }

  
  free(a);
  close(fd_l);
  close(fd_i);

  return ds;

 err2:
  dataset_delete(ds);

 err1:
  free(a);

 err0:
  close(fd_l);
  close(fd_i);
 
  return NULL;

}

/*

  Evaluate Helpers

*/

int mnist_max_array(unsigned int n, double* a)
{
  int max = 0;
  unsigned int i;

  assert(a != NULL);
  assert(n);
    
   for(i=1;i<n;i++)
    {

      if (a[i] > a[max])
	{
	  max = i;
	}
    }
 
  return max;
}
