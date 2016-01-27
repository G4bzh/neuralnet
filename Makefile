CC = gcc
CFLAGS = -Wall -lm -g -fopenmp
EXEC = neuralnet

all : $(EXEC)

neuralnet: main.o neuron.o ffnnet.o cnnet.o dataset.o mnist.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c neuron.h ffnnet.h cnnet.h dataset.h mnist.h

mnist.o : mnist.c mnist.h dataset.h

ffnnet.o : ffnnet.c ffnnet.h neuron.h dataset.h

cnnet.o : cnnet.c cnnet.h neuron.h

neuron.o: neuron.c neuron.h

dataset.o: dataset.c dataset.h

clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXEC)