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
  

  double in[] ={1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0};


  CNN = cnnet_create(5,5,3,2,2);
  assert(CNN != NULL);
  cnnet_feedforward(CNN,in);

  cnnet_delete(CNN);

  return EXIT_SUCCESS;

}
