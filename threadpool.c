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

  TQueue : Push

*/

int tqueue_push(TQUEUE* q, TASK* t)
{
  if ( (t == NULL) || (q == NULL) )
    {
      return EXIT_FAILURE;
    }

  /* Critical section */
  pthread_mutex_lock( &(q->mutex) );
  
  if (q->tail == NULL)
    {
      q->head = t;
      q->tail = t;
    }
  else
    {
      q->tail->prev = t;
      q->tail = t;
    }

  /* Work todo now */
  pthread_cond_signal( &(q->todo) );

  pthread_mutex_unlock( &(q->mutex) );


  return EXIT_SUCCESS;
}


/*

  TQueue : Pop

*/


TASK* tqueue_pop(TQUEUE* q)
{
  TASK* t;

  if (q == NULL)
    {
      return NULL;
    }

  /* Critical section */
  pthread_mutex_lock( &(q->mutex) );

  /* Wait for a task to do */
  while( q->head == NULL )
    {
      pthread_cond_wait( &(q->todo), &(q->mutex) );
    }

  t = q->head;
  q->head = q->head->prev;

  pthread_mutex_unlock( &(q->mutex) );

  return t;

}



/*

  Dummy

*/


void func(void* i)
{
  
  printf("I'm %d\n", (int)i);
  return;
}


/*

  Main

*/

int main()
{
  TQUEUE* q;
  TASK *t0,*t1,*t2,*t;

  t0 = task_create(func,(void*)0);
  t1 = task_create(func,(void*)1);
  t2 = task_create(func,(void*)2);

  q = tqueue_create();

  tqueue_push(q,t0);
  tqueue_push(q,t1);
  tqueue_push(q,t2);
  
  for(t=tqueue_pop(q);t!=NULL;t=tqueue_pop(q))
    {
      t->function(t->arg);
    }

  task_delete(t0);
  task_delete(t1);
  task_delete(t2);
  tqueue_delete(q);

  return 0;
}
