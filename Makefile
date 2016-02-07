CC=gcc
CFLAGS=-I.
DEPS = particle_filter.h random_number_gen.h

all: particle_filter

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

particle_filter: particle_filter.o
	gcc -g -o particle_filter particle_filter.o random_number_gen.c -I. -lm

run:
	./particle_filter

clean: 
	rm particle_filter *.o
