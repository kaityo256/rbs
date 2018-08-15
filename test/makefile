CC=g++
CPPFLAGS=-O3 -march=native
-include makefile.opt

all: a.out

a.out: test.cpp ../randbit.hpp ../walker.hpp
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f a.out expected.dat
