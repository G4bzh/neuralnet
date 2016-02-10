/*

  Thread pool

*/



#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "threadpool.h"




/*

 Task : Creation

*/


TASK* task_create(void (*function)(void*), void* arg)
{
  TASK* t;

  if (function == NULL)
    {
      return NULL;
    }

  t = (TASK*)malloc(sizeof(TASK));
  if (t == NULL)
    {
      return NULL;
    }

  t->function = function;
  t->arg = arg;
  t->prev = NULL;

  return t;
}


/*

  Task : Deletion

*/


int task_delete(TASK* t)
{
  if (t==NULL)
    {
      return EXIT_FAILURE;
    }

  t->prev = NULL;
  free(t);
  
  return EXIT_SUCCESS;

}


/*

  TQueue : Creation

*/


TQUEUE* tqueue_create()
{
  TQUEUE* tq;
  
  tq = (TQUEUE*)malloc(sizeof(TQUEUE));
  if (tq == NULL)
    {
      return NULL;
    }
  
  pthread_mutex_init(&(tq->mutex),NULL);
  pthread_cond_init(&(tq->todo),NULL);
  tq->head = NULL;
  tq->tail = NULL;

  return tq;

}


/*

  TQueue : Deletion

*/

int tqueue_delete(TQUEUE* tq)
{
  if (tq == NULL)
    {
      return EXIT_FAILURE;
    }
  
  tq->head = NULL;
  tq->tail = NULL;

  free(tq);

  return EXIT_SUCCESS;
  
}


/*

  Main

*/

int main()
{
  return 0;
}
