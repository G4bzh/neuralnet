/*

  MaxPool Layer

*/


#include <stdlib.h>
#include "neuron.h"
#include "maxpool.h"


/*

  Creation

*/

MAXPOOL* maxpool_create(unsigned int in_w, unsigned int in_h, unsigned int pool_x, unsigned int pool_w,  Neuron** in)
{
  MAXPOOL* mp;

  mp = (MAXPOOL*)malloc(sizeof(MAXPOOL));
  if (mp == NULL)
    {
      return NULL;
    }

  mp->neurons = (Neuron**)malloc(sizeof(Neuron*));
  if (mp->neurons == NULL)
    {
      goto err1;
    }

  return mp;

 err1:
  free(mp);

  return NULL;
}


/*

  Deletion

*/

int maxpool_delete(MAXPOOL* mp)
{
  if (mp == NULL)
    {
      return EXIT_FAILURE;
    }

  free(mp);

  return EXIT_SUCCESS;

}
