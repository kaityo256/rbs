all: a.out
CC=g++
CPPFLAGS=-I../../

a.out: simple.cpp
	$(CC) $(CPPFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f a.out
