CC=g++
CPPFLAGS=-O3 -march=native
-include makefile.opt

all: a.out

a.out: performance.cpp ../randbit.hpp ../walker.hpp
	$(CC) $(CPPFLAGS) $< -o $@ -I../

clean:
	rm -f a.out *.dat
