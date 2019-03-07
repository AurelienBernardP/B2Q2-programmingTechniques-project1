CC=gcc
LD=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=-lm

all: main tries dictionary board

.c .o:
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o

main: main.o board.o dictionary.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)
	
tries: tries.o
	$(LD) -o $@ $^ $(LDFLAGS)

dictionary: dictionary.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)

board: board.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)