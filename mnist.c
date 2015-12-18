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
#include "mnist.h"


/*

  Read images file

*/

int mnist_images(char* filename)
{
  int fd;
  int n,rows,cols,i,j,k;
  unsigned char c;

  assert(filename != NULL);
  fd = open(filename,O_RDONLY);
  assert(fd != -1);

  if (read(fd,&n,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if ( ntohl(n) != MNIST_IMAGES_MAGIC )
    {
      goto err0;
    } 

  if (read(fd,&n,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 

  n = ntohl(n);
  printf("Number of images : %d\n",n);

  if (read(fd,&rows,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if (read(fd,&cols,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  rows = ntohl(rows);
  cols = ntohl(cols);

  printf("Image size : %dx%d\n",rows,cols);

  for(k=0;k<n;k++)
    {
      printf("Image %d :\n",k);
      for(i=0;i<rows;i++)
	{
	  for(j=0;j<cols;j++)
	    {
	      if (read(fd,&c,sizeof(unsigned char)) != sizeof(unsigned char))
		{
		  goto err0;
		} 
	      if (c==0)
		{
		  printf(" ");
		}
	      else if (c==255)
		{
		  printf(".");
		}
	      else
		{
		  printf("*");
		}
	    }
	  printf("\n");
	}
    }

  close(fd);

  return EXIT_SUCCESS;

 err0:

  close(fd);

  return EXIT_FAILURE;

}


/*

  Read labels file

*/

int mnist_labels(char* filename)
{
  int fd;
  int n,i;
  unsigned char c;

  assert(filename != NULL);
  fd = open(filename,O_RDONLY);
  assert(fd != -1);

  if (read(fd,&n,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 
  if ( ntohl(n) != MNIST_LABELS_MAGIC )
    {
      goto err0;
    } 

  if (read(fd,&n,sizeof(int)) != sizeof(int))
    {
      goto err0;
    } 

  n = ntohl(n);
  printf("Number of labels : %d\n",n);


  

  for(i=0;i<n;i++)
    {
      if (read(fd,&c,sizeof(unsigned char)) != sizeof(unsigned char))
	{
	  goto err0;
	} 
      printf("%u\n",c);
	
    }

  close(fd);

  return EXIT_SUCCESS;

 err0:

  close(fd);

  return EXIT_FAILURE;

}
