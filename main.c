/*

  Main

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "dataset.h"
#include "functions.h"
#include "input.h"
#include "fullconn.h"
#include "mnist.h"


int main( int argc, char* argv[])
{
  Dataset* DS;
  Dataset* DS_Test;
  unsigned int i,epoch,batch_size,run,j,k,max_test,max_out,guess;
  INPUT* input;
  double eta, lambda;;
  FULLCONN* hidden;
  FULLCONN* output;

  srand(time(NULL));
  epoch = 30;
  eta = 0.1;
  lambda = 5.0;
  batch_size = 10;


  printf("Loading train images...");
  fflush(stdout);
  DS = mnist_load("../mnist/train-images-idx3-ubyte","../mnist/train-labels-idx1-ubyte");
  if (DS == NULL)
    {
      return EXIT_FAILURE;
    }
  printf("Done !\n");



  printf("Loading test images...");
  fflush(stdout);
  DS_Test = mnist_load("../mnist/t10k-images-idx3-ubyte","../mnist/t10k-labels-idx1-ubyte");
  if ( DS_Test == NULL)
    {
      goto err0;
    }
  printf("Done !\n");


  input = input_create(DS->in_len);
  hidden = fullconn_create(100,input->n_neurons,input->neurons);
  output = fullconn_create(10,hidden->n_neurons,hidden->neurons);

  for(run=0;run<epoch;run++)
    {
      dataset_shuffle(DS);
      j=0;

      /* Mini batch */
      while( j+batch_size < DS->len)
	{
	  for(i=j;i<j+batch_size;i++)
	    {  
	      input_feedforward(input,DS->in[i]);
	      fullconn_feedforward(hidden);
	      fullconn_feedforward(output);
	      
	      fullconn_backpropagation(output,DS->out[i],func_crossentropy);
	      fullconn_backpropagation(hidden,NULL,NULL);
	    }
	  
	  fullconn_update(hidden,eta/(double)batch_size,eta*lambda/(double)DS->len,func_regL2);
	  fullconn_update(output,eta/(double)batch_size,eta*lambda/(double)DS->len,func_regL2);
	  
	  j+=batch_size;
	}

      /* Validation */

      for(i=0, guess=0 ;i<DS_Test->len;i++)
	{
	  input_feedforward(input,DS_Test->in[i]);
	  fullconn_feedforward(hidden);
	  fullconn_feedforward(output);

	  max_out = 0;
	  for(k=1;k<output->n_neurons;k++)
	    {
	      if (output->neurons[k]->output > output->neurons[max_out]->output)
		{
		  max_out = k;
		}
	    }

	  max_test = mnist_max_array(DS_Test->out_len, DS_Test->out[i]);

	  if (max_out == max_test)
	    {
	      guess++;
	    }
	}

      printf("epoch %d: %u/%u\n",run,guess,DS_Test->len);

    }

  fullconn_delete(output);
  fullconn_delete(hidden);
  input_delete(input);
  dataset_delete(DS);
  dataset_delete(DS_Test);

  return EXIT_SUCCESS;

 err0:

   dataset_delete(DS);
   
   printf("Something bad has happened ...\n");

  return EXIT_FAILURE;

}
