CC = gcc
CFLAGS = -Wall
EXEC = neuralnet

all : $(EXEC)

neuralnet: main.o neuron.o
	$(CC) -o $@ $^

main.o: main.c neuron.h

neuron.o: neuron.c neuron.h

clean:
	rm -f *.o
	rm -f $(EXEC)