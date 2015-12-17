/*

  Main

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "dataset.h"
#include "nnet.h"


int main( int argc, char* argv[])
{
  
  Dataset* DS;
  unsigned int i,n;

  srand(time(NULL));
  n = 4000;


  DS = dataset_create(n,1,1);

  for(i=0;i<n;i++)
    {
      dataset_add(DS,2,(double)i,(double)(i+1));
    }

  dataset_print(DS);

  printf(" --- \n");

  dataset_shuffle(DS);
  dataset_print(DS);
  dataset_delete(DS);

  return EXIT_SUCCESS;
}
