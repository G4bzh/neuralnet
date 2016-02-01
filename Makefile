CC = gcc
CFLAGS = -Wall -lm -g -fopenmp
EXEC = neuralnet

all : $(EXEC)

neuralnet: main.o neuron.o dataset.o mnist.o input.o fullconn.o functions.o convol.o maxpool.o linear.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c neuron.h dataset.h mnist.h input.h fullconn.h functions.h convol.h maxpool.h linear.h

mnist.o : mnist.c mnist.h dataset.h

linear.o : linear.c linear.h neuron.h

maxpool.o: maxpool.c maxpool.h neuron.h

convol.o : convol.c convol.h neuron.h 

fullconn.o : fullconn.c fullconn.h neuron.h 

input.o : input.c input.h neuron.h 

functions.o: functions.c functions.h neuron.h

neuron.o: neuron.c neuron.h

dataset.o: dataset.c dataset.h

clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXEC)