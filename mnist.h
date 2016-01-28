/*

  Header

*/


#ifndef MNIST_H
#define MNIST_H

#include "dataset.h"

Dataset* mnist_load(char*, char*);
int mnist_max_array(unsigned int n, double* a);

#endif
