/*

  Header

*/


#ifndef MNIST_H
#define MNIST_H

#include "dataset.h"
#include "nnet.h"

Dataset* mnist_load(char*, char*);
int mnist_evaluate(NNet*, Dataset*);

#endif
