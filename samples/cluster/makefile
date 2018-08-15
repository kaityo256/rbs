all: a.out
CC=g++
CPPFLAGS=-O3 -march=native

-include makefile.opt

a.out: cluster.cpp
	$(CC) $(CPPFLAGS) $< -o $@ -I../../

serial.dat: a.out
	./a.out

msc.dat: a.out
	./a.out

graph: serial.dat msc.dat
	gnuplot cluster.plt

.PHONY: clean
clean:
	rm -f a.out cluster.pdf cluster.eps serial.dat msc.dat
