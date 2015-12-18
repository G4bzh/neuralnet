/*

  Dataset header

*/


#ifndef DATASET_H
#define DATASET_H


typedef struct
{
  double** in;             /* Array of input arrays */
  unsigned int in_len;     /* Len of one input array */
  double** out;            /* Array of ouput arrays */
  unsigned int out_len;    /* Len of one output array */
  unsigned int len;        /* Len of in and out */
  unsigned int cursor;     /* Current index (helper) */
} Dataset;


Dataset* dataset_create(unsigned int, unsigned int, unsigned int);
int dataset_delete(Dataset*);
int dataset_print(Dataset*);
int dataset_add(Dataset*, unsigned int, ...);
int dataset_shuffle(Dataset*);

#endif