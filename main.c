/*

  Main

*/


#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>  
#include <time.h>
#include <assert.h>
#include "dataset.h"
#include "ffnnet.h"
#include "cnnet.h"
#include "mnist.h"


int main( int argc, char* argv[])
{
  
  CNNet* CNN;
  

  CNN = cnnet_create(10,10,3,5,5);
  assert(CNN != NULL);

  cnnet_delete(CNN);

  return EXIT_FAILURE;

}
