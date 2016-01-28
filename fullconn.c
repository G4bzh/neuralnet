/*

  Fully Connected Layer 

*/


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "neuron.h"
#include "fullconn.h"


/*

  Creation

*/

FULLCONN* fullconn_create(unsigned int prev, unsigned int n)
{
  FULLCONN* FC;

  return FC;

}



/*

  Deletion

*/


int fullconn_delete(FULLCONN* FC)
{
  if (FC == NULL)
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
