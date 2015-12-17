CC = gcc
CFLAGS = -Wall -lm
EXEC = neuralnet

all : $(EXEC)

neuralnet: main.o neuron.o nnet.o dataset.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c neuron.h nnet.h

dataset.o: dataset.c dataset.h

nnet.o : nnet.c nnet.h neuron.h

neuron.o: neuron.c neuron.h

clean:
	rm -f *.o
	rm -f $(EXEC)