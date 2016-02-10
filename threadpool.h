/*

  Threadpool header

*/

#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <pthread.h>


typedef struct TASK
{
  void (*function)(void*);
  void* arg;
  struct TASK* prev;
} TASK;


typedef struct TQUEUE
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  TASK* head;
  TASK* tail;
} TQUEUE;


typedef struct THPOOL
{
  pthread_t* threads;
  unsigned int n_threads;
  TQUEUE* queue;
} THPOOL;



#endif
