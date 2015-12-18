/*

  Header

*/


#ifndef MNIST_H
#define MNIST_H

#define MNIST_IMAGES_MAGIC    0x803
#define MNIST_LABELS_MAGIC    0x801




int mnist_images(char* filename);
int mnist_labels(char* filename);

#endif
