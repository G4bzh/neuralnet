/*

  Header

*/


#ifndef MNIST_H
#define MNIST_H

#include "dataset.h"
#include "ffnnet.h"

Dataset* mnist_load(char*, char*);
int mnist_evaluate(FFNNet*, Dataset*);

#endif
