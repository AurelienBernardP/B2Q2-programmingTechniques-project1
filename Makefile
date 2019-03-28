CC=gcc
LD=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=-lm

SOURCES=grid.c dictionary.c tries.c main.c

all: solver

solver: $(SOURCES)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f solver
