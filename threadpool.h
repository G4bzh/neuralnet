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
  pthread_cond_t todo;
  TASK* head;
  TASK* tail;
} TQUEUE;


typedef struct THPOOL
{
  pthread_t* threads;
  unsigned int n_threads;
  unsigned int n_working;
  TQUEUE* queue;
  pthread_mutex_t mutex;
  pthread_cond_t idle;
} THPOOL;


THPOOL* thpool_create(unsigned int n_threads);
int thpool_delete(THPOOL*);
int thpool_run(THPOOL*, void (*function)(void*), void*);
int thpool_wait(THPOOL*);

#endif
