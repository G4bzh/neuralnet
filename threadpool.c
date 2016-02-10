/*

  Thread pool

*/



#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
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
  /* Update tail if no more task */
  if (q->head == NULL)
    {
      q->tail = NULL;
    }

  pthread_mutex_unlock( &(q->mutex) );
 
  return t;
  
}


/*

  THPool : Exec

*/

void* thpool_exec(void* arg)
{
  THPOOL* tp = (THPOOL*)arg;
  TASK* t;

  if (tp == NULL)
    {
      return NULL;
    }

  while( (t = tqueue_pop(tp->queue)) != NULL )
    {
      t->function(t->arg);
      task_delete(t);
    }

  return NULL;
}


/*

  THPool : Creation

*/

THPOOL* thpool_create(unsigned int n_threads)
{
  THPOOL* tp;
  unsigned int i;

  if (n_threads == 0)
    {
      return NULL;
    }

  tp = (THPOOL*)malloc(sizeof(THPOOL));
  if (tp == NULL)
    {
      return NULL;
    }

  tp->threads = (pthread_t*)malloc(n_threads*sizeof(pthread_t));
  if (tp == NULL)
    {
      goto err0;
    }

  tp->queue = tqueue_create();
  if (tp->queue == NULL)
    {
      goto err1;
    }


  for(i=0;i<n_threads;i++)
    {
      if ( pthread_create(&(tp->threads[i]),NULL,thpool_exec,(void*)tp) != 0 )
      	{
      	  goto err2;
      	}
    }

  tp->n_threads = n_threads;

  return tp;

 err2:
  tqueue_delete(tp->queue);    

 err1:
  free(tp->threads);

 err0:
  free(tp);

  return NULL;

}


/*

  THPool : Delete

*/

int thpool_delete(THPOOL* tp)
{
  if (tp == NULL)
    {
      return EXIT_FAILURE;
    }

  tqueue_delete(tp->queue);    
  free(tp->threads);
  free(tp);

  return EXIT_SUCCESS;
}




/*

  THPool: Run

*/

int thpool_run(THPOOL* tp, void (*function)(void*), void* arg)
{
  TASK* t;

  if ( (tp == NULL) || (function == NULL) )
    {
      return EXIT_FAILURE;
    }

  t = task_create(function,arg);
  if (t == NULL)
    {
      return EXIT_FAILURE;
    }

  return tqueue_push(tp->queue,t);
   
}
