CC = gcc
CFLAGS = -Wall
EXEC = neuralnet

all : $(EXEC)

neuralnet: main.o neuron.o layer.o nnet.o
	$(CC) -o $@ $^

main.o: main.c neuron.h layer.h nnet.h

nnet.o : nnet.c nnet.h layer.h

layer.o : layer.c layer.h neuron.h

neuron.o: neuron.c neuron.h

clean:
	rm -f *.o
	rm -f $(EXEC)